#include <forest/phenotype.h>


// TODO: parse values from genes
Phenotype::Phenotype(const Dna::SPtr& dna)
    : _dna(dna)
{
    for (size_t i = 0; i < _traitsSize; ++i) {
        GetRawTraits(0)[i] = ExtMath::RandomDouble(-5, 5);
    }
}

const Phenotype::TraitAccessor& Phenotype::AccessTraits(size_t/* depth*/) const {
    return _traits.Access;
}

Phenotype::TraitsRaw& Phenotype::GetRawTraits(size_t/* depth*/) {
    return _traits.Raw;
}
