#include <dsys/ram.h>
#include <cassert>

RAM::RAM(size_t size)
    : _memory(size, 0)
    , _size(size)
    , _addrMask(size - 1)
{
    assert(_size > 0);
    assert((_size & _addrMask) == 0);
}

void RAM::Clear() {
    _memory.assign(_size, 0);
}

const RamCellType& RAM::operator[](size_t addr) const {
    return _memory[addr & _addrMask];
}

RamCellType& RAM::operator[](size_t addr) {
    return _memory[addr & _addrMask];
}
