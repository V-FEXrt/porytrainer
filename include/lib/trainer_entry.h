#ifndef INCLUDE_CORE_TRAINER_ENTRY_H
#define INCLUDE_CORE_TRAINER_ENTRY_H

#include <string>
#include <vector>

#include "pokemon_entry.h"

namespace fex
{
    class TrainerEntry
    {
    public:
        TrainerEntry(
            std::string identifier,
            std::vector<std::string> party_flags,
            std::string trainer_class,
            std::vector<std::string> encounter_music_gender,
            std::string trainer_pic,
            std::string trainer_name,
            std::vector<std::string> items,
            bool double_battle,
            std::vector<std::string> ai_flags,
            std::vector<PokemonEntry> party = {})
            : identifier_(identifier),
              party_flags_(party_flags),
              trainer_class_(trainer_class),
              encounter_music_gender_(encounter_music_gender),
              trainer_pic_(trainer_pic),
              trainer_name_(trainer_name),
              items_(items),
              double_battle_(double_battle),
              ai_flags_(ai_flags),
              party_(party) {}

        void AddPokemon(PokemonEntry pokemon)
        {
            party_.push_back(pokemon);
        }

        const std::string &identifier() const { return identifier_; }
        const std::vector<std::string> &party_flags() const { return party_flags_; }
        const std::string &trainer_class() const { return trainer_class_; }
        const std::vector<std::string> &encounter_music_gender() const { return encounter_music_gender_; }
        const std::string &trainer_pic() const { return trainer_pic_; }
        const std::string &trainer_name() const { return trainer_name_; }
        const std::vector<std::string> &items() const { return items_; }
        bool double_battle() const { return double_battle_; }
        const std::vector<std::string> &ai_flags() const { return ai_flags_; }
        int party_size() const { return party_.size(); }
        const std::vector<PokemonEntry> &party() const { return party_; }

    private:
        std::string identifier_;
        std::vector<std::string> party_flags_;
        std::string trainer_class_;
        std::vector<std::string> encounter_music_gender_;
        std::string trainer_pic_;
        std::string trainer_name_;
        std::vector<std::string> items_;
        bool double_battle_;
        std::vector<std::string> ai_flags_;
        // int party_size --> Skipped. Calculate via party.size()
        std::vector<PokemonEntry> party_;
    };

} // namespace fex

#endif // INCLUDE_CORE_TRAINER_ENTRY_H