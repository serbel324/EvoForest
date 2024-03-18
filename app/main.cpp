#include <app/cell.h>

#include <iostream>
#include <unordered_set>

int main() {
    uint32_t populationSize = 100;
    std::string target = "pidoras";
    size_t iterations = 1000;

    std::vector<WordCell> population;
    uint32_t clonesPerIter = 3;


    for (uint32_t iter = 0; iter < populationSize; ++iter) {
        population.emplace_back(WordCell::Random());
    }

    EvoLib::Mutator mutator(0.01, 0.01, 0.01);

    for (uint32_t iter = 0; iter < iterations; ++iter) {
        std::cout << " =========== Iteration# " << iter << " =========== " << std::endl;
        for (const WordCell& cell : population) {
            std::cout << " - " << cell.GetWord() << ' ' << cell.Score(target) << std::endl;
        }


        std::vector<WordCell> clones;
        std::vector<std::pair<uint64_t, size_t>> scores;

        for (const WordCell& cell : population) {
            for (uint32_t i = 0; i < clonesPerIter; ++i) {
                size_t idx = clones.size();
                WordCell newClone = (i == clonesPerIter) ? cell : cell.Clone(mutator);
                scores.emplace_back(newClone.Score(target), idx);
                clones.emplace_back(std::move(newClone));
            }
        }

        population.clear();
        std::sort(scores.begin(), scores.end());
        for (uint32_t i = 0; i < populationSize; ++i) {
            population.emplace_back(std::move(clones[scores[i].second]));
        }
    }
}
