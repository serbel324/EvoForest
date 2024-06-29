#pragma once

#include <cstddef>
#include <vector>

#include <dsys/arch.h>


class RAM {
public:
    // size must be a power of two
    RAM(size_t size);

    void Clear();
    void FillRandom();

    const RamCellType& operator[](size_t addr) const;
    RamCellType& operator[](size_t addr);

    void Set(size_t addr, RamCellType* data, size_t dataSize);
    void SetRandom(size_t addr, size_t dataSize);
    void Reset(size_t addr, size_t cellsToReset);

    bool CheckIfNull(size_t addr, size_t length) const;
    uint32_t CountNulls(size_t addr, size_t length) const;
    void Read(size_t addr, RamCellType* dst, size_t cellsToRead) const;

    size_t GetAddressMask() const;

private:
    std::vector<RamCellType> _memory;
    const size_t _size;
    const size_t _addrMask;
};
