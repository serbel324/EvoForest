#include <dsys/colors.h>
#include <dsys/os.h>

#include <iomanip>
#include <fstream>

void EvOS::_DumpMemory(std::ostream& out) {
    for (size_t i = 0; i < _pageCount; ++i) {
        out << std::dec << "Page " << i << ":" << std::endl;
        CreatureId owner = _pageClaims[i];
        if (owner) {
            out << Colors[_creatures[owner].color];
        }
        for (size_t j = 0; j < PageCells; ++j) {
            out << std::hex << std::setfill('0') << std::setw(sizeof(RamCellType) * 2) << (*_ram)[i * PageCells + j];
        }
        if (owner) {
            out << RESET_COLOR;
        }
        out << std::endl;
    }
    out << std::endl;
    out << WHITE_COLOR << "======================================================" << RESET_COLOR << std::endl;
}

void EvOS::_DumpPages(std::ostream& out) {
    for (size_t i = 0; i < _pageCount; ++i) {
        CreatureId owner = _pageClaims[i];
        if (owner) {
            out << Colors[_creatures[owner].color] << "#";
        } else {
            out << RESET_COLOR << "0";
        }
    }
    out << std::endl;
    out << WHITE_COLOR << "======================================================" << RESET_COLOR << std::endl;
}


void EvOS::_DumpRegisters(std::ostream& out, const CPU::Registers& registers) {
    for (uint32_t i = 0; i < RegisterCount; ++i) {
        out << std::dec << RegisterName((Register)i) << " = " << registers[i] << std::endl;
    }
    out << std::endl;
    out << WHITE_COLOR << "======================================================" << RESET_COLOR << std::endl;
}