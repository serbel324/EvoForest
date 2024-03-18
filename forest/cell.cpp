#include <app/cell.h>

#include <library/ext_math.h>

#include <algorithm>
#include <cmath>

WordGene::WordGene(char letter)
    : _letter(letter)
{}

WordGene WordGene::Random() {
    return WordGene(ExtMath::RandomInt('a', 'z' + 1));
}

void WordGene::Mutate() {
    char newLetter = _letter + ExtMath::RandomInt(-2, 3);
    newLetter = std::min(newLetter, 'z');
    newLetter = std::max(newLetter, 'a');
    _letter = newLetter;
}

char WordGene::GetLetter() const {
    return _letter;
}

WordCell::WordCell(const std::string& word) {
    Dna::Genome genome;
    for (char letter : word) {
        genome.push_back(WordGene(letter));
    }
    _dna = Dna(genome);
}

WordCell::WordCell(const Dna& dna)
    : _dna(dna)
{}

WordCell WordCell::Random() {
    size_t len = ExtMath::RandomInt(1, 10);
    std::string word(len, 'a');
    for (size_t i = 0; i < len; ++i) {
        word[i] = ExtMath::RandomInt('a', 'z' + 1);
    }
    return WordCell(word);
}

WordCell WordCell::Clone(const EvoLib::Mutator& mutator) const {
    Dna cloneDna = _dna.Replicate(mutator);
    return WordCell(cloneDna);
}

uint64_t WordCell::Score(const std::string& target, uint64_t letterLoss) const {
    uint64_t score = 0;
    const Dna::Genome& genome = _dna.GetGenome();
    size_t lenDiff = std::abs((int)genome.size() - (int)target.size());
    score += lenDiff * letterLoss;

    for (size_t i = 0; i < std::min(genome.size(), target.size()); ++i) {
        score += std::abs(genome[i].GetLetter() - target[i]);
    }
    return score;
}

std::string WordCell::GetWord() const {
    std::string res = "";
    const Dna::Genome& genome = _dna.GetGenome();

    for (const WordGene& gene : genome) {
        res.push_back(gene.GetLetter());
    }
    return res;
}