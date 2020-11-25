#ifndef INCLUDE_CORE_POKEMON_ENTRY_H
#define INCLUDE_CORE_POKEMON_ENTRY_H

#include <string>
#include <vector>

namespace fex
{
    class PokemonEntry
    {
    public:
        PokemonEntry(std::string species, int iv, int level, std::string held_item = "", std::vector<std::string> moves = {}) : species_(species), iv_(iv), level_(level), held_item_(held_item), moves_(moves) {}

        const std::string &species() const { return species_; }
        int iv() const { return iv_; }
        int level() const { return level_; }
        const std::string &held_item() const { return held_item_; }
        const std::vector<std::string> &moves() const { return moves_; }

    private:
        std::string species_;
        int iv_;
        int level_;
        std::string held_item_;
        std::vector<std::string> moves_;
    };

} // namespace fex

#endif // INCLUDE_CORE_PARSER_H