#include "stats_tracker.h"

Statistics::Statistics() {
    ResetLocals();
}

void Statistics::ResetLocals() {
    creationTime = Clock::now();
    forks = 0;
    failedForks = 0;
    blocksConsumed = 0;
    blocksConsumptionsFailed = 0;
    codeCorruptions = 0;
    deadCreatures = 0;
}

void Statistics::Print(std::ostream& out) const {
    out << std::dec;
    out << "Time since creation = " << ToSec(Clock::now() - creationTime) << " [seconds]" << std::endl;
    out << "Recreations = " << recreations << std::endl;
    out << "Forks count = " << forks << std::endl;
    out << "Failed forks count = " << failedForks << std::endl;
    out << "Consumed block count = " << blocksConsumed << std::endl;
    out << "Failed block consumtion count = " << blocksConsumptionsFailed << std::endl;
    out << "Code corruptions = " << codeCorruptions << std::endl;
    out << "Creatures died = " << deadCreatures << std::endl;
    out << "Creatures alive = " << forks - deadCreatures << std::endl;
}
