#ifndef TRAINER_VALUE_H
#define TRAINER_VALUE_H

#include <string>
#include <vector>

#include "pokemon_value.h"

namespace fex
{
    class TrainerValue
    {
    public:
        static bool FromValueList(std::string identifier, std::vector<ArrayValue> values, std::unique_ptr<TrainerValue>* out);

        std::vector<std::string> party_flags() const;
        std::string trainer_class() const;
        std::vector<std::string> encounter_music_gender() const;
        std::string trainer_pic() const;
        std::string trainer_name() const;
        std::vector<std::string> items() const;
        bool double_battle() const;
        std::vector<std::string> ai_flags() const;
        std::string party_variable_type() const;
        std::string party_variable_value() const;

        const std::string& identifier() const
        {
            return identifier_;
        }

        const std::vector<std::unique_ptr<PokemonValue>>& party() const
        {
            return party_;
        }

        int party_size() const
        {
            return party_.size();
        }

        void AddPokemon(std::unique_ptr<PokemonValue> pokemon)
        {
            party_.push_back(std::move(pokemon));
        }

    private:
        TrainerValue(std::string identifier, std::map<std::string, ArrayValue> values): identifier_(identifier), values_(std::move(values))  {}

        std::string identifier_;
        std::map<std::string, ArrayValue> values_;
        std::vector<std::unique_ptr<PokemonValue>> party_;
    };

} // namespace fex

#endif // TRAINER_VALUE_H
