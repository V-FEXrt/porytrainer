#ifndef POKEMON_VALUE_H
#define POKEMON_VALUE_H

#include <string>
#include <vector>
#include <map>

#include "array_value.h"

namespace fex
{
    class PokemonValue
    {
    public:
        static bool FromValueList(std::vector<ArrayValue> values, std::unique_ptr<PokemonValue>* out);

        std::string species() const;
        int iv() const;
        int level() const;
        std::string held_item() const;
        std::vector<std::string> moves() const;

    private:
        PokemonValue(std::map<std::string, ArrayValue> values): values_(std::move(values))  {}
        std::map<std::string, ArrayValue> values_;
    };

} // namespace fex

#endif // POKEMON_VALUE_H
