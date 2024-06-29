#pragma once

#include "timer.h"
#include <iostream>

struct Statistics {
    Statistics();

    Timestamp creationTime;
    uint32_t recreations;

    uint32_t forks;
    uint32_t failedForks;
    uint32_t blocksConsumed;
    uint32_t blocksConsumptionsFailed;
    uint32_t codeCorruptions;
    uint32_t deadCreatures;

    void ResetLocals();

    void Print(std::ostream& out) const;
};
