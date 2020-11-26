#include "trainer_util.h"

#include <map>
#include <vector>
#include <iostream>

#include "lexer.h"
#include "parser.h"

namespace fex
{
    namespace
    {
        std::vector<std::string> ParseBitOrFlags(const ArrayValue &value)
        {
            std::vector<std::string> flags = {};
            if (value.pair().second->type() == ArrayValue::Type::kNumber)
            {
                return flags;
            }
            if (value.pair().second->type() == ArrayValue::Type::kIdentifier)
            {
                flags.push_back(value.pair().second->string_value());
                return flags;
            }
            if (value.pair().second->type() == ArrayValue::Type::kValueList)
            {
                for (const ArrayValue &v : value.pair().second->values())
                {
                    flags.push_back(v.string_value());
                }
                return flags;
            }
            return flags;
        }

        std::string ParseIdentifer(const ArrayValue &value)
        {
            return value.pair().second->string_value();
        }

        int ParseInt(const ArrayValue &value)
        {
            return value.pair().second->int_value();
        }

        std::string ParsePartyVarName(const ArrayValue &value)
        {
            return value.pair().second->pair().second->string_value();
        }

        PokemonEntry ParsePokemon(const ArrayValue &value)
        {
            int iv = 0;
            int level = 0;
            std::string species = "";
            std::string held_item = "";
            std::vector<std::string> moves = {};
            for (const auto &item : value.values())
            {
                if (item.type() != ArrayValue::Type::kValuePair)
                {
                    continue;
                }

                std::string var_name = item.pair().first;
                if (var_name == "iv")
                {
                    iv = ParseInt(item);
                }
                if (var_name == "lvl")
                {
                    level = ParseInt(item);
                }
                if (var_name == "species")
                {
                    species = ParseIdentifer(item);
                }
                if (var_name == "heldItem")
                {
                    held_item = ParseIdentifer(item);
                }

                if (var_name == "moves")
                {
                    moves = ParseBitOrFlags(item);
                }
            }

            return PokemonEntry(species, iv, level, held_item, moves);
        }

        std::unique_ptr<PokemonValue> ParsePokemonV(ArrayValue &value)
        {
            std::unique_ptr<PokemonValue> pvs;
            PokemonValue::FromValueList(value.release_values(), &pvs);
            return pvs;
        }

        std::unique_ptr<TrainerValue> ParseTrainerV(const std::string &identifier, const ArrayValue &value)
        {
            std::unique_ptr<TrainerValue> tvs;
            TrainerValue::FromValueList(identifier, value.pair().second->release_values(), &tvs);
            return tvs;
        }

        std::pair<TrainerEntry, std::string> ParseTrainer(const std::string &identifier, const ArrayValue &value)
                {
                    std::vector<std::string> party_flags;
                    std::string trainer_class;
                    std::vector<std::string> encounter_music_gender;
                    std::string trainer_pic;
                    std::string trainer_name;
                    std::vector<std::string> items;
                    std::string double_battle;
                    std::vector<std::string> ai_flags;
                    std::string party_var_name;

                    for (const ArrayValue &item : value.pair().second->values())
                    {
                        if (item.type() != ArrayValue::Type::kValuePair)
                        {
                            continue;
                        }

                        std::string var_name = item.pair().first;
                        if (var_name == "partyFlags")
                        {
                            party_flags = ParseBitOrFlags(item);
                        }

                        if (var_name == "trainerClass")
                        {
                            trainer_class = ParseIdentifer(item);
                        }

                        if (var_name == "encounterMusic_gender")
                        {
                            encounter_music_gender = ParseBitOrFlags(item);
                        }

                        if (var_name == "trainerPic")
                        {
                            trainer_pic = ParseIdentifer(item);
                        }

                        if (var_name == "trainerName")
                        {
                            trainer_name = ParseIdentifer(item);
                        }

                        if (var_name == "items")
                        {
                            items = ParseBitOrFlags(item);
                        }

                        if (var_name == "doubleBattle")
                        {
                            double_battle = ParseIdentifer(item);
                        }

                        if (var_name == "aiFlags")
                        {
                            ai_flags = ParseBitOrFlags(item);
                        }

                        if (var_name == "party")
                        {
                            if (item.pair().second->type() == ArrayValue::Type::kValuePair)
                            {
                                party_var_name = ParsePartyVarName(item);
                            }
                        }
                    }

                    return std::pair<TrainerEntry, std::string>(
                        TrainerEntry(
                            identifier,
                            party_flags,
                            trainer_class,
                            encounter_music_gender,
                            trainer_pic,
                            trainer_name,
                            items,
                            (double_battle == "TRUE"),
                            ai_flags),
                        party_var_name);
                }

        bool FindGTrainersArray(const std::vector<Array> &arrays, const Array **out)
        {
            for (const Array &array : arrays)
            {
                if (array.name() == "gTrainers")
                {
                    *out = &array;
                    return true;
                }
            }
            return false;
        }

    } // namespace

    TrainerUtil::TrainerUtil(std::string trainer_data_path, std::string party_data_path) : trainer_data_path_(trainer_data_path), party_data_path_(party_data_path) {}

    std::vector<std::unique_ptr<TrainerValue>> TrainerUtil::ReadTrainersV()
    {
        Lexer trainer_lexer;
        Parser trainer_parser;

        auto tokens = trainer_lexer.LexFile(trainer_data_path_);
        std::vector<Array> trainer_arrays = trainer_parser.ParseTopLevelArrays(tokens);

        const Array *g_trainers_array;
        if (!FindGTrainersArray(trainer_arrays, &g_trainers_array))
        {
            return {};
        }

        std::map<std::string, std::unique_ptr<TrainerValue>> trainers;

        for (const ArrayValue &value : g_trainers_array->values())
        {
            for (const ArrayValue &v : value.values())
            {
                if (v.type() != ArrayValue::Type::kValuePair)
                {
                    continue;
                }

                std::string key = v.pair().first;
                trainers.insert(std::pair<std::string, std::unique_ptr<TrainerValue>>(key, ParseTrainerV(key, v)));
            }
        }

        Lexer party_lexer;
        Parser party_parser;

        auto party_tokens = party_lexer.LexFile(party_data_path_);
        std::vector<Array> party_array = party_parser.ParseTopLevelArrays(party_tokens);
        std::map<std::string, Array> name_value_map;

        for (unsigned int i = 0; i < party_array.size(); i++)
        {
            name_value_map.insert(std::pair<std::string, Array>(party_array[i].name(), std::move(party_array[i])));
        }

        std::vector<std::unique_ptr<TrainerValue>> out;
        for (auto& pair : trainers)
        {
            auto data = name_value_map.find(pair.second->party_variable_value());
            if (data == name_value_map.end() || data->second.values().size() == 0)
            {
                std::cout << "[WARNING] " << pair.first << " requires " << pair.second->party_variable_value() << " but it is not defined." << std::endl;
                continue;
            }

            auto array_value = data->second.release_values();

            // This is either a list of list of ValuePairs if the party was larger than 1
            // or a list of ValuePairs if the party only had one pokemon
            std::vector<ArrayValue> pokemon_values = array_value[0].release_values();
            if (pokemon_values.front().type() == ArrayValue::Type::kValuePair) {
                auto value_list = ArrayValue::ValueList(std::move(pokemon_values));
                pair.second->AddPokemon(ParsePokemonV(value_list));
            } else { // Add the entire team
                for (auto &pokemon : pokemon_values)
                {
                    if (pokemon.type() == ArrayValue::Type::kEmpty)
                    {
                        continue;
                    }

                    pair.second->AddPokemon(ParsePokemonV(pokemon));
                }
            }

            out.push_back(std::move(pair.second));
        }

        return out;
    }

    std::vector<TrainerEntry> TrainerUtil::ReadTrainers()
    {
        Lexer trainer_lexer;
        Parser trainer_parser;

        auto tokens = trainer_lexer.LexFile(trainer_data_path_);
        std::vector<Array> trainer_arrays = trainer_parser.ParseTopLevelArrays(tokens);

        const Array *g_trainers_array;
        if (!FindGTrainersArray(trainer_arrays, &g_trainers_array))
        {
            return {};
        }

        std::map<std::string, std::pair<TrainerEntry, std::string>> trainers;

        for (const ArrayValue &value : g_trainers_array->values())
        {
            for (const ArrayValue &v : value.values())
            {
                if (v.type() != ArrayValue::Type::kValuePair)
                {
                    continue;
                }

                std::string key = v.pair().first;
                trainers.insert(std::pair<std::string, std::pair<TrainerEntry, std::string>>(key, ParseTrainer(key, v)));
            }
        }

        Lexer party_lexer;
        Parser party_parser;

        auto party_tokens = party_lexer.LexFile(party_data_path_);
        std::vector<Array> party_array = party_parser.ParseTopLevelArrays(party_tokens);
        std::map<std::string, Array> name_value_map;

        for (unsigned int i = 0; i < party_array.size(); i++)
        {
            name_value_map.insert(std::pair<std::string, Array>(party_array[i].name(), std::move(party_array[i])));
        }

        std::vector<TrainerEntry> out;
        for (std::pair<std::string, std::pair<TrainerEntry, std::string>> pair : trainers)
        {
            auto data = name_value_map.find(pair.second.second);
            if (data == name_value_map.end() || data->second.values().size() == 0)
            {
                std::cout << "[WARNING] " << pair.first << " requires " << pair.second.second << " but it is not defined." << std::endl;
                continue;
            }

            const auto &array_value = data->second.values()[0];
            for (const auto &pokemon : array_value.values())
            {
                if (pokemon.type() == ArrayValue::Type::kEmpty)
                {
                    continue;
                }

                pair.second.first.AddPokemon(ParsePokemon(pokemon));
            }
            out.push_back(pair.second.first);
        }

        return out;
    }

} // namespace fex
