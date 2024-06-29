#pragma once

#include <dsys/cpu.h>
#include <dsys/os.h>
#include <dsys/ram.h>

#include <thread>

class Process {
public:
    Process(uint64_t ramSize, uint32_t coreId);

    void Launch();
    void Join();

private:
    uint64_t _ramSize;
    uint32_t _coreId;

    std::thread _t;
};
