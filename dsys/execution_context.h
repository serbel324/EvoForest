#pragma once

#include <cstddef>

#include <dsys/arch.h>
#include <dsys/cpu.h>
#include <dsys/paging.h>

using CreatureId = uint32_t;

struct ExecutionContext {
    CreatureId id;
    std::vector<PageIdx> pages;
    CPU::Registers registers;

    uint32_t color;
    uint64_t ticksToLive;
};
