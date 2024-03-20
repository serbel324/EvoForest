#pragma once

#include <evo_lib/dna.h>

#include <forest/gene.h>
#include <forest/transformer.h>

#include <array>
#include <memory>
#include <unordered_map>

class Phenotype {
public:
    using SPtr = std::shared_ptr<Phenotype>;
    using Dna = EvoLib::Dna<TreeGene>;

public:
    Phenotype(const Dna::SPtr& dna);

    const Dna::SPtr& GetDna() const;

public:
    // TODO cache transformed values somehow
    struct alignas(sizeof(double)) TraitAccessor {
#define TRAIT(name, transformer)                            \
        double __value##name;                               \
        double Get##name() const {                          \
            return transformer::Transform(__value##name);   \
        }

        TRAIT(SeedFoodDistribution, TransformLogistic<>);
        TRAIT(SeedFoodStorageConsumption, TransformLogistic<Int<1000>>);

        TRAIT(SproutChanceToTerminate, TransformLogistic<>);
    
        TRAIT(SproutAccumulatedToGrow, decltype(TransformSum<TransformLogistic<Int<10>>, Int<1>>{}));
        TRAIT(SproutAngleDeviation, decltype(TransformSum<TransformLogistic<Int<2>>, Int<1>>{}));

        TRAIT(BranchGrowthSpeed, TransformLogistic<Int<10>>);
        TRAIT(BranchFoodDistribution, TransformLogistic<>);

#undef TRAIT
    };

public:
    static constexpr size_t _traitsSize = sizeof(TraitAccessor) / sizeof(double);
    using TraitsRaw = double[_traitsSize];

    union Traits {
        TraitsRaw Raw;
        TraitAccessor Access;
    };

public:
    const TraitAccessor& AccessTraits(size_t/* depth*/) const;
    TraitsRaw& GetRawTraits(size_t/* depth*/);

private:
    Dna::SPtr _dna;
    Traits _traits;
};
