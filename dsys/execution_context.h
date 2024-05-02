#pragma once

#include <cstddef>

#include <dsys/arch.h>
#include <dsys/cpu.h>
#include <dsys/paging.h>
#include <dsys/program.h>

struct ExecutionContext {
    uint64_t id;
    std::vector<PageIdx> pages;
    CPU::Registers registers;
    uint64_t ticksToLive;
};
