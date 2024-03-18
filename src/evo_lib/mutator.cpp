#include <evo_lib/mutator.h>
#include <library/ext_math.h>

#include <cassert>

namespace EvoLib {

Mutator::Mutator(float corruptionChance, float abscenceChance, float duplicationChance)
    : _corruptionChance(corruptionChance)
    , _abscenceChance(abscenceChance)
    , _duplicationChance(duplicationChance)
{
    assert(_corruptionChance + _abscenceChance + _duplicationChance < 1 + 1e-9);
}

Mutator::MutationType Mutator::operator()() const
{
    float r = ExtMath::RandomDouble(0, 1);
    if (r < _corruptionChance) {
        return MutationType::CORRUPTION;
    }
    r -= _corruptionChance;

    if (r < _abscenceChance) {
        return MutationType::ABSENCE;
    }
    r -= _abscenceChance;

    if (r < _duplicationChance) {
        return MutationType::DUPLICATION;
    }
    
    return MutationType::OK;
}

} // namespace EvoLib
