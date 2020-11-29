#include "pokemon_value.h"

#include <map>
#include <vector>
#include <iostream>

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
    } // namespace

    // static
    bool PokemonValue::FromValueList(std::vector<ArrayValue> values, std::unique_ptr<PokemonValue>* out)
    {
        std::map<std::string, ArrayValue> map;

        for (unsigned int i = 0; i < values.size(); i++)
        {
            if (values[i].type() != ArrayValue::Type::kValuePair)
            {
                continue;
            }

            const auto& key = values[i].pair().first;
            map.insert(std::pair<std::string, ArrayValue>(key, std::move(values[i])));
        }

        *out = std::unique_ptr<PokemonValue>(new PokemonValue(std::move(map)));
        return true;
    }


    std::string PokemonValue::species() const
    {
        auto it = values_.find("species");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested species, but value missing" << std::endl;
            return "";
        }

        return ParseIdentifer(it->second);
    }

    int PokemonValue::iv() const
    {
        auto it = values_.find("iv");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested iv, but value missing" << std::endl;
            return 0;
        }

        return ParseInt(it->second);
    }

    int PokemonValue::level() const
    {
        auto it = values_.find("lvl");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested level, but value missing" << std::endl;
            return 0;
        }

        return ParseInt(it->second);
    }

    std::string PokemonValue::held_item() const
    {
        auto it = values_.find("heldItem");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested held item, but value missing" << std::endl;
            return "";
        }

        return ParseIdentifer(it->second);
    }

    std::vector<std::string> PokemonValue::moves() const
    {
        auto it = values_.find("moves");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested moves, but value missing" << std::endl;
            return {};
        }

        return ParseBitOrFlags(it->second);
    }

    void PokemonValue::SetSpecies(const std::string &value)
    {
        auto it = values_.find("species");
        if (it == values_.end())
        {
            // TODO: add value
            std::cout << "[WARNING] Attempted to set species, but value missing" << std::endl;
            return;
        }

        it->second.pair().second->set_string_value(value);
    }

    void PokemonValue::SetLevel(int value)
    {
        auto it = values_.find("lvl");
        if (it == values_.end())
        {
            // TODO: add value
            std::cout << "[WARNING] Attempted to set level, but value missing" << std::endl;
            return;
        }

        it->second.pair().second->set_int_value(value);
    }

    void PokemonValue::SetIv(int value)
    {
        auto it = values_.find("iv");
        if (it == values_.end())
        {
            // TODO: add value
            std::cout << "[WARNING] Attempted to set iv, but value missing" << std::endl;
            return;
        }

        it->second.pair().second->set_int_value(value);
    }

    void PokemonValue::SetHeldItem(const std::string& value)
    {
        auto it = values_.find("heldItem");
        if (it == values_.end())
        {
            std::unique_ptr<ArrayValue> v = std::unique_ptr<ArrayValue>(new ArrayValue(ArrayValue::Type::kIdentifier, value));
            ArrayValue pair = ArrayValue::ValuePair(std::pair<std::string, std::unique_ptr<ArrayValue>>("heldItem", std::move(v)));

            values_.insert(std::pair<std::string, ArrayValue>("heldItem", std::move(pair)));
            return;
        }

        it->second.pair().second->set_string_value(value);
    }
} // namespace fex
