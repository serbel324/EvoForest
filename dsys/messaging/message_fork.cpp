#include "message_fork.h"

MessageFork::MessageFork(std::vector<Page> sourceCode, uint64_t ticksToLive, uint32_t color)
    : sourceCode(std::move(sourceCode))
    , ticksToLive(ticksToLive)
    , color(color)
{}
