#pragma once

#include <cstddef>
#include <vector>

#include <dsys/arch.h>

class RAM {
public:
    // size must be a power of two
    RAM(size_t size);

    void Clear();

    const RamCellType& operator[](size_t addr) const;
    RamCellType& operator[](size_t addr);

private:
    std::vector<RamCellType> _memory;
    const size_t _size;
    const size_t _addrMask;
};
