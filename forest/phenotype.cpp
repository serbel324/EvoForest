#include <forest/phenotype.h>


// TODO: parse values from genes
Phenotype::Phenotype(const Dna::SPtr& dna)
    : _dna(dna)
{
    _traitSeedFoodDistribution = 0.5;
    _traitSeedFoodStorageConsumption = 100;

    _traitSproutChanceToTerminate = 0.3;
    _traitSproutAccumulatedToGrow = 50;
    _traitSproutAngleDeviation = 1;

    _traitBranchGrowthCost = 1;
    _traitBranchFoodDistribution = 0.01;
}
