#pragma once

#include <memory>

namespace EvoLib {

// Gene is an abstract piece of information
class Gene {
public:
    virtual ~Gene() = default;

    virtual void Mutate() = 0;
};

} // namespace EvoLib
