#include <dsys/ram.h>
#include <cassert>
#include <cstring>

#include <library/ext_math.h>

#include <iostream>
#include <iomanip>

RAM::RAM(size_t size)
    : _memory(size, 0)
    , _size(size / sizeof(RamCellType))
    , _addrMask(_size - 1)
{
    assert(_size > 0);
    assert((_size & _addrMask) == 0);
}

void RAM::Clear() {
    _memory.assign(_size, 0);
}

void RAM::FillRandom() {
    for (size_t i = 0; i < _size; ++i) {
        _memory[i] = ExtMath::RandomUint32();
    }
}

const RamCellType& RAM::operator[](size_t addr) const {
    return _memory[addr & _addrMask];
}

RamCellType& RAM::operator[](size_t addr) {
    return _memory[addr & _addrMask];
}

void RAM::Set(size_t addr, RamCellType* data, size_t dataSize) {
    addr &= _addrMask;
    assert(dataSize <= _size);
    if (addr + dataSize > _size) {
        size_t tail = addr + dataSize - _size;
        std::memcpy(_memory.data() + addr, data, (dataSize - tail) * sizeof(RamCellType));
        std::memcpy(_memory.data(), data + dataSize - tail, tail * sizeof(RamCellType));
    } else {
        std::memcpy(_memory.data() + addr, data, dataSize * sizeof(RamCellType));
    }
}

void RAM::SetRandom(size_t addr, size_t dataSize) {
    addr &= _addrMask;
    assert(dataSize <= _size);
    for (size_t cell = addr, i = 0; i < dataSize; ++i, cell = (cell + 1) & _addrMask) {
        _memory[cell] = ExtMath::RandomUint32();
    }
}

void RAM::Reset(size_t addr, size_t cellsToReset) {
    addr &= _addrMask;
    assert(cellsToReset <= _size);
    if (addr + cellsToReset > _size) {
        size_t tail = addr + cellsToReset - _size;
        std::memset(_memory.data() + addr, 0x0, (cellsToReset - tail) * sizeof(RamCellType));
        std::memset(_memory.data(), 0x0, tail * sizeof(RamCellType));
    } else {
        std::memset(_memory.data() + addr, 0x0, cellsToReset * sizeof(RamCellType));
    }
}

bool RAM::CheckIfNull(size_t addr, size_t length) const {
    addr &= _addrMask;
    assert(length <= _size);
    RamCellType res = 0x0;
    for (size_t i = 0; i < length; ++i) {
        res |= _memory[(addr + i) & _addrMask];
    }
    return res == 0;
}

uint32_t RAM::CountNulls(size_t addr, size_t length) const {
    uint32_t res = 0;
    addr &= _addrMask;
    assert(length <= _size);
    for (size_t i = 0; i < length; ++i) {
        res += (_memory[(addr + i) & _addrMask] == 0);
    }
    return res;
}

void RAM::Read(size_t addr, RamCellType* dst, size_t cellsToRead) const {
    addr &= _addrMask;
    assert(cellsToRead <= _size);
    if (addr + cellsToRead > _size) {
        size_t tail = addr + cellsToRead - _size;
        std::memcpy(dst, _memory.data() + addr, (cellsToRead - tail) * sizeof(RamCellType));
        std::memcpy(dst + cellsToRead - tail, _memory.data(), tail * sizeof(RamCellType));
    } else {
        std::memcpy(dst, _memory.data() + addr, cellsToRead * sizeof(RamCellType));
    }
}

size_t RAM::GetAddressMask() const {
    return _addrMask;
}
