#include "emitter.h"

#include <map>
#include <vector>
#include <iostream>
#include <fstream>

#include "parser_util.h"

namespace fex
{

namespace
{

std::string BitOrFromVector(const std::vector<std::string>& items)
{
    if (items.size() == 0) {
        return "0";
    }

    std::string out = "";

    for (unsigned int i = 0; i < items.size() - 1; i++) {
        out += items[i] + " | ";
    }

    return out + items.back();
}

std::string ValueListFromVector(const std::vector<std::string>& items)
{
    if (items.size() == 0) {
        return "{ }";
    }

    std::string out = "{ ";

    for (unsigned int i = 0; i < items.size() - 1; i++) {
        out += items[i] + ", ";
    }

    return out + items.back() + " }";
}

} // namespace

    Emitter::Emitter(std::string root, std::string trainer_data_path, std::string party_data_path) : root_(root), trainer_data_path_(trainer_data_path), party_data_path_(party_data_path) {}

    void Emitter::EmitTrainers(const std::map<QString, std::unique_ptr<TrainerValue>>& trainers)
    {
        ParserUtil parser_util(QString::fromStdString(root_));

        QStringList trainer_list = parser_util.ReadDefinesValueSort("include/constants/opponents.h", "TRAINER_");

        // FRLG don't have an oppoents.h files
        if (trainer_list.empty()) {
            trainer_list = parser_util.ReadDefinesValueSort("include/constants/trainers.h", "TRAINER_");
        }

        std::ofstream trainer_file;
        trainer_file.open (root_ + "/" + trainer_data_path_);

        std::ofstream party_file;
        party_file.open (root_ + "/" + party_data_path_);

        trainer_file << "const struct Trainer gTrainers[] = {\n";

        for (auto key : trainer_list)
        {      
            auto it = trainers.find(key);
            if (it == trainers.end())
            {
                // key not in map. skip it.
                std::cout << "[WARNING] Key: " << key.toStdString() << " not found in trainers map." << std::endl;
                continue;
            }
            const auto& trainer = it->second;

            std::string party_var_type = "";

            // Determine Party Var type
            bool has_items = false;
            bool has_moves = false;
            for (const auto& pokemon : trainer->party())
            {
                if (pokemon->held_item() != "")
                {
                    has_items = true;
                }
                if (pokemon->moves().size() > 0)
                {
                    has_moves = true;
                }
            }

            if (has_items && has_moves) {
                party_var_type = "ItemCustomMoves";
            } else if (has_items) {
                party_var_type = "ItemDefaultMoves";
            } else if (has_moves) {
                party_var_type = "NoItemCustomMoves";
            } else {
                party_var_type = "NoItemDefaultMoves";
            }

            trainer_file << "\t" << "[" << key.toStdString() << "] = {\n";
            trainer_file << "\t\t" << "." << "partyFlags" << " = " << BitOrFromVector(trainer->party_flags()) << ",\n";
            trainer_file << "\t\t" << "." << "trainerClass" << " = " << trainer->trainer_class() << ",\n";
            trainer_file << "\t\t" << "." << "encounterMusic_gender" << " = " << BitOrFromVector(trainer->encounter_music_gender()) << ",\n";
            trainer_file << "\t\t" << "." << "trainerPic" << " = " << trainer->trainer_pic() << ",\n";
            trainer_file << "\t\t" << "." << "trainerName" << " = " << "_(\"" << trainer->trainer_name() << "\")" << ",\n";
            trainer_file << "\t\t" << "." << "items" << " = " << ValueListFromVector(trainer->items()) << ",\n";
            trainer_file << "\t\t" << "." << "doubleBattle" << " = " << ((trainer->double_battle()) ? "TRUE" : "FALSE") << ",\n";
            trainer_file << "\t\t" << "." << "aiFlags" << " = " << BitOrFromVector(trainer->ai_flags()) << ",\n";

            const auto& party_var_value = trainer->party_variable_value();
            if (party_var_value == "" || party_var_value == "NULL")
            {
                trainer_file << "\t\t" << "." << "partySize" << " = " << 0 << ",\n";
                trainer_file << "\t\t" << "." << "party" << " = " << "NULL" << ",\n";
            }
            else
            {
                trainer_file << "\t\t" << "." << "partySize" << " = " << "ARRAY_COUNT(" << trainer->party_variable_value() << ")" << ",\n";
                trainer_file << "\t\t" << "." << "party" << " = " << "{ " << "." << party_var_type << " = " << trainer->party_variable_value() << " }" << ",\n";
            }

            trainer_file << "\t" << "},\n";

            if (party_var_value != "" && party_var_value != "NULL")
            {
                // TODO: determine party type based on contents.

                party_file << "static const struct " << "TrainerMon" << party_var_type << " " << party_var_value << "[] = {\n";

                for (const auto& pokemon : trainer->party())
                {
                    party_file << "\t" << "{" << "\n";

                    party_file << "\t\t" << "." << "iv" << " = " << pokemon->iv() << ",\n";
                    party_file << "\t\t" << "." << "lvl" << " = " << pokemon->level() << ",\n";
                    party_file << "\t\t" << "." << "species" << " = " << pokemon->species() << ",\n";

                    if (party_var_type == "ItemDefaultMoves")
                    {
                        std::string item = "ITEM_NONE";
                        if (pokemon->held_item() != "")
                        {
                            item = pokemon->held_item();
                        }

                        party_file << "\t\t" << "." << "heldItem" << " = " << item << ",\n";
                    }

                    if (party_var_type == "NoItemCustomMoves")
                    {
                        std::vector<std::string> moves = {"MOVE_NONE", "MOVE_NONE", "MOVE_NONE", "MOVE_NONE"};
                        for (unsigned int i = 0; i < pokemon->moves().size(); i++)
                        {
                            moves[i] = pokemon->moves()[i];
                        }

                        party_file << "\t\t" << "." << "moves" << " = " << ValueListFromVector(moves) << ",\n";
                    }

                    if (party_var_type == "ItemCustomMoves")
                    {
                        std::string item = "ITEM_NONE";
                        if (pokemon->held_item() != "")
                        {
                            item = pokemon->held_item();
                        }

                        party_file << "\t\t" << "." << "heldItem" << " = " << item << ",\n";

                        std::vector<std::string> moves = {"MOVE_NONE", "MOVE_NONE", "MOVE_NONE", "MOVE_NONE"};
                        for (unsigned int i = 0; i < pokemon->moves().size(); i++)
                        {
                            moves[i] = pokemon->moves()[i];
                        }

                        party_file << "\t\t" << "." << "moves" << " = " << ValueListFromVector(moves) << ",\n";
                    }

                    party_file << "\t" << "}," << "\n";
                }

                party_file << "};" << "\n\n";
            }
        }

        trainer_file << "};" << "\n";

        trainer_file.close();
        party_file.close();

    }
} // namespace fex
