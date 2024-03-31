#pragma once

#include <evo_lib/dna.h>

#include <forest/tree/gene.h>
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
    class alignas(sizeof(double)) TraitAccessor {
#define TRAIT(name, transformer)                            \
        private: double __value##name;                      \
        public: double Get##name() const {                  \
            return transformer::Transform(__value##name);   \
        }

        TRAIT(SeedFoodDistribution, TransformLogistic<>);
        TRAIT(SeedFoodStorageConsumption, TransformLogistic<Int<1000>>);

        TRAIT(SproutChanceToTerminate, TransformLogistic<>);
        TRAIT(SproutAccumulatedToGrow, decltype(TransformSum<TransformLogistic<Int<10>>, Int<1>>{}));
        TRAIT(SproutBranchAngleDeviationMin, TransformLogistic<Int<1>>);
        TRAIT(SproutBranchAngleDeviationRange, TransformLogistic<Int<3>>);
        TRAIT(SproutLeafAngleDeviationMin, TransformLogistic<Int<1>>);
        TRAIT(SproutLeafAngleDeviationRange, TransformLogistic<Int<3>>);
        TRAIT(SproutChanceToSpawnLeaf, TransformLogistic<>);

        TRAIT(BranchInitialLength, decltype(TransformSum<TransformLogistic<Int<9>>, Int<1>>{}));
        TRAIT(BranchGrowthSpeed, TransformLogistic<Int<10>>);
        TRAIT(BranchFoodDistribution, TransformLogistic<>);

        TRAIT(LeafStemLength, TransformLogistic<Int<10>>);
        TRAIT(LeafFoodGenerationEfficiency, TransformLogistic<Int<10>>);

        TRAIT(RootSproutChanceToTerminate, TransformLogistic<>);
        TRAIT(RootSproutAccumulatedToGrow, decltype(TransformSum<TransformLogistic<Int<10>>, Int<1>>{}));
        TRAIT(RootSproutAngleDeviationMin, TransformLogistic<Int<1>>);
        TRAIT(RootSproutAngleDeviationRange, TransformLogistic<Int<3>>);
        TRAIT(RootSproutChanceToSpawnMiner, TransformLogistic<>);
        TRAIT(RootSproutMinerAngleDeviationMin, TransformLogistic<Int<1>>);
        TRAIT(RootSproutMinerAngleDeviationRange, TransformLogistic<Int<3>>);

        TRAIT(RootInitialLength, decltype(TransformSum<TransformLogistic<Int<9>>, Int<1>>{}));
        TRAIT(RootGrowthSpeed, decltype(TransformSum<TransformLogistic<Int<1>>, Int<1>>{}));
        TRAIT(RootFoodDistribution, TransformLogistic<>);

        TRAIT(MinerStemLength, TransformLogistic<Int<10>>);
        TRAIT(MinerFoodGenerationEfficiency, TransformLogistic<Int<10>>);

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
