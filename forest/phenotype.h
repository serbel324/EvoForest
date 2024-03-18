#pragma once

#include <evo_lib/dna.h>

#include <forest/gene.h>

#include <memory>

class Phenotype {
public:
    using SPtr = std::shared_ptr<Phenotype>;
    using Dna = EvoLib::Dna<TreeGene>;

public:
    Phenotype(const Dna::SPtr& dna);

    const Dna::SPtr& GetDna() const;

private:
    Dna::SPtr _dna;

#define TRAIT(name)                                             \
    private: float _trait##name;                                \
    public: float Get##name() const { return _trait##name; } 

    TRAIT(SeedFoodDistribution);
    TRAIT(SeedFoodStorageConsumption);

    TRAIT(SproutChanceToTerminate);
    TRAIT(SproutAccumulatedToGrow);
    TRAIT(SproutAngleDeviation);

    TRAIT(BranchGrowthCost);
    TRAIT(BranchFoodDistribution);

#undef TRAIT
};
