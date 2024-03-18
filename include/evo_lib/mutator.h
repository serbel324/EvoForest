#pragma once

namespace EvoLib {

class Mutator {
public:
    enum class MutationType {
        OK = 0,
        CORRUPTION,
        ABSENCE,
        DUPLICATION,
    };

    Mutator(float corruptionChance = 0, float abscenceChance = 0, float duplicationChance = 0);
    MutationType operator()() const;

private:
    float _corruptionChance;
    float _abscenceChance;
    float _duplicationChance;
};

} // namespace EvoLib
