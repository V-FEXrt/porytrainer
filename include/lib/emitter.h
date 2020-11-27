#ifndef EMITTER_H
#define EMITTER_H

#include <string>
#include <vector>

#include <QString>

#include "trainer_value.h"

namespace fex
{
    class Emitter
    {
    public:
        Emitter(std::string root, std::string trainer_data_path, std::string party_data_path);
        void EmitTrainers(const std::map<QString, std::unique_ptr<TrainerValue>>& trainers);

    private:
        std::string root_;
        std::string trainer_data_path_;
        std::string party_data_path_;
    };
} // namespace fex


#endif // EMITTER_H
