#pragma once

#include <memory>
#include <vector>

#include <evo_lib/gene.h>
#include <evo_lib/mutator.h>

namespace EvoLib {

template <class GeneType>
class Dna {
public:
    using Genome = std::vector<GeneType>;
    using SPtr = std::shared_ptr<Dna>;

public:
    Dna() = default;

    Dna(Dna&& other) = default;
    Dna(const Dna& other) = default;

    Dna& operator=(Dna&& other) = default;
    Dna& operator=(const Dna& other) = default;

    Dna(const Genome& genome)
        : _genome(genome)
    {}

public:
    Dna Replicate(const Mutator& mutator) const {
        Dna copy;

        for (const GeneType& gene : _genome) {
            Mutator::MutationType mutation = mutator();

            switch (mutation) {
            case Mutator::MutationType::OK:
                copy._genome.push_back(gene);
                break;

            case Mutator::MutationType::CORRUPTION: {
                GeneType corruptedGene = gene;
                corruptedGene.Mutate();
                copy._genome.push_back(std::move(corruptedGene));
                break;
            }

            case Mutator::MutationType::ABSENCE:
                break;

            case Mutator::MutationType::DUPLICATION:
                copy._genome.push_back(gene);
                copy._genome.push_back(gene);
                break;
            }
        }

        return copy;
    }

    const Genome& GetGenome() const {
        return _genome;
    }

private:
    Genome _genome;
};

} // namespace EvoLib
