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
        std::unique_ptr<PokemonValue> ParsePokemon(ArrayValue &value)
        {
            std::unique_ptr<PokemonValue> pvs;
            PokemonValue::FromValueList(value.release_values(), &pvs);
            return pvs;
        }

        std::unique_ptr<TrainerValue> ParseTrainer(const std::string &identifier, const ArrayValue &value)
        {
            std::unique_ptr<TrainerValue> tvs;
            TrainerValue::FromValueList(identifier, value.pair().second->release_values(), &tvs);
            return tvs;
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

    std::vector<std::unique_ptr<TrainerValue>> TrainerUtil::ReadTrainers()
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
                trainers.insert(std::pair<std::string, std::unique_ptr<TrainerValue>>(key, ParseTrainer(key, v)));
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
                out.push_back(std::move(pair.second));
                continue;
            }

            auto array_value = data->second.release_values();

            // This is either a list of list of ValuePairs if the party was larger than 1
            // or a list of ValuePairs if the party only had one pokemon
            std::vector<ArrayValue> pokemon_values = array_value[0].release_values();
            if (pokemon_values.front().type() == ArrayValue::Type::kValuePair) {
                auto value_list = ArrayValue::ValueList(std::move(pokemon_values));
                pair.second->AddPokemon(ParsePokemon(value_list));
            } else { // Add the entire team
                for (auto &pokemon : pokemon_values)
                {
                    if (pokemon.type() == ArrayValue::Type::kEmpty)
                    {
                        continue;
                    }

                    pair.second->AddPokemon(ParsePokemon(pokemon));
                }
            }

            out.push_back(std::move(pair.second));
        }

        return out;
    }
} // namespace fex
