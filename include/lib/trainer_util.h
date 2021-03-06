#ifndef INCLUDE_CORE_TRAINER_UTIL_H
#define INCLUDE_CORE_TRAINER_UTIL_H

#include <string>

#include "trainer_value.h"

namespace fex
{
    class TrainerUtil
    {
    public:
        TrainerUtil(std::string trainer_data_path, std::string party_data_path);
        std::vector<std::unique_ptr<TrainerValue>> ReadTrainers();

    private:
        std::string trainer_data_path_;
        std::string party_data_path_;
    };
} // namespace fex

#endif // INCLUDE_CORE_TRAINER_UTIL_H
