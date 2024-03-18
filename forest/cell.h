#include <evo_lib/gene.h>
#include <evo_lib/dna.h>

#include <string>
#include <memory>

class WordGene : public EvoLib::Gene {
public:
    WordGene(char letter);

public:
    static WordGene Random();

    void Mutate() override;

    char GetLetter() const;

private:
    char _letter;
};

class WordCell {
public:
    WordCell(const std::string& word);
    WordCell(const EvoLib::Dna<WordGene>& dna);

    WordCell(WordCell&& other) = default;
    WordCell(const WordCell& other) = default;
    WordCell& operator=(WordCell&& other) = default;
    WordCell& operator=(const WordCell& other) = default;

public:
    static WordCell Random();

    WordCell Clone(const EvoLib::Mutator& mutator) const;

    uint64_t Score(const std::string& target, uint64_t letterLoss = 30) const;

    std::string GetWord() const;

private:
    using Dna = EvoLib::Dna<WordGene>;
    Dna _dna;
};
