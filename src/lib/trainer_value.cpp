#include "trainer_value.h"

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

#include "pokemon_value.h"

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
    } // namespace

    // static
    bool TrainerValue::FromValueList(std::string identifier, std::vector<ArrayValue> values, std::unique_ptr<TrainerValue>* out)
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

        *out = std::unique_ptr<TrainerValue>(new TrainerValue(identifier, std::move(map)));
        return true;
    }

    std::vector<std::string> TrainerValue::party_flags() const
    {
        auto it = values_.find("partyFlags");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested party flags, but value missing" << std::endl;
            return {};
        }

        return ParseBitOrFlags(it->second);
    }

    std::string TrainerValue::trainer_class() const
    {
        auto it = values_.find("trainerClass");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested trainer class, but value missing" << std::endl;
            return "";
        }

        return ParseIdentifer(it->second);
    }

    std::vector<std::string> TrainerValue::encounter_music_gender() const
    {
        auto it = values_.find("encounterMusic_gender");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested encounter music/gender, but value missing" << std::endl;
            return {};
        }

        return ParseBitOrFlags(it->second);
    }

    std::string TrainerValue::trainer_pic() const
    {
        auto it = values_.find("trainerPic");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested trainer pic, but value missing" << std::endl;
            return "";
        }

        return ParseIdentifer(it->second);
    }

    std::string TrainerValue::trainer_name() const
    {
        auto it = values_.find("trainerName");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested trainer name, but value missing" << std::endl;
            return "";
        }

        return ParseIdentifer(it->second);
    }

    std::vector<std::string> TrainerValue::items() const
    {
        auto it = values_.find("items");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested items, but value missing" << std::endl;
            return {};
        }

        return ParseBitOrFlags(it->second);
    }

    bool TrainerValue::double_battle() const
    {
        auto it = values_.find("doubleBattle");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested double battle, but value missing" << std::endl;
            return "";
        }

        return ParseIdentifer(it->second) == "TRUE";
    }

    std::vector<std::string> TrainerValue::ai_flags() const
    {
        auto it = values_.find("aiFlags");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested ai flags, but value missing" << std::endl;
            return {};
        }

        return ParseBitOrFlags(it->second);
    }

    std::string TrainerValue::party_variable_type() const
    {
        auto it = values_.find("party");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested party var type, but value missing" << std::endl;
            return "";
        }

        return it->second.pair().second->pair().first;
    }

    std::string TrainerValue::party_variable_value() const
    {
        auto it = values_.find("party");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Requested party var value, but value missing" << std::endl;
            return "";
        }

        if (it->second.type() != ArrayValue::Type::kValuePair)
        {
            std::cout << "[WARNING] Requested party var value, but value was not a kvp" << std::endl;
            return "";
        }

        if (it->second.pair().second->type() != ArrayValue::Type::kValuePair)
        {
            std::cout << "[WARNING] Requested party var value, but value was not a kvp" << std::endl;
            return "";
        }

        return it->second.pair().second->pair().second->string_value();
    }

    void TrainerValue::SetTrainerPic(const std::string& value)
    {
        auto it = values_.find("trainerPic");
        if (it == values_.end())
        {
            // TODO: add value
            std::cout << "[WARNING] Attempted to set trainer pic, but value missing" << std::endl;
            return;
        }

        it->second.pair().second->set_string_value(value);
    }

    void TrainerValue::SetTrainerClass(const std::string& value)
    {
        auto it = values_.find("trainerClass");
        if (it == values_.end())
        {
            // TODO: add value
            std::cout << "[WARNING] Attempted to set trainer class, but value missing" << std::endl;
            return;
        }

        it->second.pair().second->set_string_value(value);
    }

    void TrainerValue::SetTrainerName(const std::string& value)
    {
        auto it = values_.find("trainerName");
        if (it == values_.end())
        {
            // TODO: add value
            std::cout << "[WARNING] Attempted to set trainer name, but value missing" << std::endl;
            return;
        }

        it->second.pair().second->set_string_value(value);
    }

    void TrainerValue::SetDoubleBattle(bool value)
    {
        auto it = values_.find("doubleBattle");
        if (it == values_.end())
        {
            // TODO: add value
            std::cout << "[WARNING] Attempted to set double battle, but value missing" << std::endl;
            return;
        }

        it->second.pair().second->set_string_value((value) ? "TRUE" : "FALSE");
    }

    void TrainerValue::SetEncounterMusicGender(const std::string& value, bool is_female)
    {
        auto it = values_.find("encounterMusic_gender");
        if (it == values_.end())
        {
            // TODO: add value
            std::cout << "[WARNING] Attempted to set encounter music/gender, but value missing" << std::endl;
            return;
        }

        std::vector<ArrayValue> value_list = {};
        value_list.push_back(ArrayValue::Identifier(value));
        if (is_female)
        {
            value_list.push_back(ArrayValue::Identifier("F_TRAINER_FEMALE"));
        }

        it->second.pair().second->set_type(ArrayValue::Type::kValueList);
        it->second.pair().second->set_values(std::move(value_list));
    }

    void TrainerValue::SetAIScripts(std::vector<std::string> values)
    {
        auto it = values_.find("aiFlags");
        if (it == values_.end())
        {
            // TODO: add value
            std::cout << "[WARNING] Attempted to set ai scripts, but value missing" << std::endl;
            return;
        }

        std::vector<ArrayValue> value_list = {};
        for(const auto& value: values)
        {
            value_list.push_back(ArrayValue::Identifier(value));
        }

        it->second.pair().second->set_type(ArrayValue::Type::kValueList);
        it->second.pair().second->set_values(std::move(value_list));
    }

    void TrainerValue::RemoveItem(const std::string &value)
    {
        auto it = values_.find("items");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Attempted to remove item, but value missing" << std::endl;
            return;
        }

        std::vector<ArrayValue> values = it->second.pair().second->release_values();
        int index = 0;
        for (const auto& v : values)
        {
            if (v.type() == ArrayValue::Type::kIdentifier && v.string_value() == value)
            {
                break;
            }
            index++;
        }
        values.erase(values.begin() + index);
        it->second.pair().second->set_values(std::move(values));
    }

    void TrainerValue::AddItem(const std::string &value)
    {
        auto it = values_.find("items");
        if (it == values_.end())
        {
            std::cout << "[WARNING] Attempted to remove item, but value missing" << std::endl;
            return;
        }

        std::vector<ArrayValue> values = it->second.pair().second->release_values();
        values.push_back(ArrayValue::Identifier(value));

        it->second.pair().second->set_type(ArrayValue::Type::kValueList);
        it->second.pair().second->set_values(std::move(values));
    }

} // namespace fex
