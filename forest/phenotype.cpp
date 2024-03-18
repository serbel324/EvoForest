#include <forest/phenotype.h>


// TODO: parse values from genes
Phenotype::Phenotype(const Dna::SPtr& dna)
    : _dna(dna)
{
    _traitSeedFoodDistribution = 0.5;
    _traitSeedFoodStorageConsumption = 1;

    _traitSproutChanceToTerminate = 0.5;
    _traitSproutAccumulatedToGrow = 100;
    _traitSproutAngleDeviation = 0.1;

    _traitBranchGrowthCost = 0.1;
    _traitBranchFoodDistribution = 0.01;
}
