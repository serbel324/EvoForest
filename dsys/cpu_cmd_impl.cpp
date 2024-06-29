#include <dsys/cpu.h>
#include <dsys/messaging/interconnect_proxy.h>
#include <dsys/messaging/message_fork.h>
#include <dsys/messaging/message_block_consumed.h>
#include <dsys/messaging/message_proxy.h>

#include <iomanip>
#include <iostream>

#define VERBOSE false

#define VOUT(out)                                   \
    if constexpr(VERBOSE) {                         \
        std::cout << std::dec << out << std::endl;  \
    }


// NOP
void CPU::_Nop() {
    VOUT("NOP");
    return;
}

// MOV
void CPU::_MovRR(RamCellType src, RamCellType dst) {
    _registers[dst & RegisterModMask] = _registers[src & RegisterModMask];
    VOUT("MOV %" << RegisterName((Register)src) << " %" << RegisterName((Register)dst));
}

void CPU::_MovRM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    (*_ram)[ip + dst] = _registers[src & RegisterModMask];
    VOUT("MOV %" << RegisterName((Register)src) << " " << ip + dst);
}

void CPU::_MovMR(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _registers[dst & RegisterModMask] = (*_ram)[ip + src];
    VOUT("MOV " << (*_ram)[ip + src] << " %" << RegisterName((Register)src));
}

void CPU::_MovMM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    (*_ram)[ip + dst] = (*_ram)[ip + src];
    VOUT("MOV " << (*_ram)[ip + src] << " " << ip + dst);
}

void CPU::_MovVR(RamCellType src, RamCellType dst) {
    _registers[dst & RegisterModMask] = src;
    VOUT("MOV $" << src << " %" << RegisterName((Register)dst));
}

void CPU::_MovVM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    (*_ram)[ip + dst] = src;
    VOUT("MOV $" << src << " " << ip + dst);
}


// MOV by offset to EAX
void CPU::_MovOffsR(RamCellType src) {
    size_t offs = _registers[src & RegisterModMask];
    _registers[EAX] = (*_ram)[_registers[Register::EIP] + offs];
    VOUT("MOVOFFS %" << RegisterName((Register)src));
}

void CPU::_MovOffsM(RamCellType src) {
    size_t offs = (*_ram)[_registers[Register::EIP] + src];
    _registers[EAX] = (*_ram)[_registers[Register::EIP] + offs];
    VOUT("MOVOFFS " << offs);
}

// MOV by offset from EAX
void CPU::_MovOffsToR(RamCellType src) {
    size_t offs = _registers[src & RegisterModMask];
    (*_ram)[_registers[Register::EIP] + offs] = _registers[EAX];
    VOUT("MOVOFFTO %" << RegisterName((Register)src));
}

void CPU::_MovOffsToM(RamCellType src) {
    size_t offs = (*_ram)[_registers[Register::EIP] + src];
    (*_ram)[_registers[Register::EIP] + offs] = _registers[EAX];
    VOUT("MOVOFFSTO " << offs);
}

// ADD
void CPU::_AddRR(RamCellType src, RamCellType dst) {
    _UpdateEFlagsAdd(_registers[src & RegisterModMask], _registers[dst & RegisterModMask]);
    _registers[dst & RegisterModMask] += _registers[src & RegisterModMask];
    VOUT("ADD %" << RegisterName((Register)src) << " %" << RegisterName((Register)dst));
}

void CPU::_AddRM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsAdd(_registers[src & RegisterModMask], (*_ram)[ip + dst]);
    (*_ram)[ip + dst] += _registers[src & RegisterModMask];
    VOUT("ADD %" << RegisterName((Register)src) << " " << ip + dst);
}

void CPU::_AddMR(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsAdd((*_ram)[ip + src], _registers[dst & RegisterModMask]);
    _registers[dst & RegisterModMask] += (*_ram)[ip + src];
    VOUT("ADD " << (*_ram)[ip + src] << " %" << RegisterName((Register)src));
}

void CPU::_AddMM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsAdd((*_ram)[ip + src], (*_ram)[ip + dst]);
    (*_ram)[ip + dst] += (*_ram)[ip + src];
    VOUT("ADD " << (*_ram)[ip + src] << " " << ip + dst);
}

void CPU::_AddVR(RamCellType src, RamCellType dst) {
    _UpdateEFlagsAdd(src, _registers[dst & RegisterModMask]);
    _registers[dst & RegisterModMask] += src;
    VOUT("ADD $" << src << " %" << RegisterName((Register)dst));
}

void CPU::_AddVM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsAdd(src, (*_ram)[ip + dst]);
    (*_ram)[ip + dst] += src;
    VOUT("ADD $" << src << " " << ip + dst);
}

// SUB
void CPU::_SubRR(RamCellType src, RamCellType dst) {
    _UpdateEFlagsSub(_registers[src & RegisterModMask], _registers[dst & RegisterModMask]);
    _registers[dst & RegisterModMask] -= _registers[src & RegisterModMask];
    VOUT("SUB %" << RegisterName((Register)src) << " %" << RegisterName((Register)dst));
}

void CPU::_SubRM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsSub(_registers[src & RegisterModMask], (*_ram)[ip + dst]);
    (*_ram)[ip + dst] -= _registers[src & RegisterModMask];
    VOUT("SUB %" << RegisterName((Register)src) << " " << ip + dst);
}

void CPU::_SubMR(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsSub((*_ram)[ip + src], _registers[dst & RegisterModMask]);
    _registers[dst & RegisterModMask] -= (*_ram)[ip + src];
    VOUT("SUB " << (*_ram)[ip + src] << " %" << RegisterName((Register)src));
}

void CPU::_SubMM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsSub((*_ram)[ip + src], (*_ram)[ip + dst]);
    (*_ram)[ip + dst] -= (*_ram)[ip + src];
    VOUT("ADD " << (*_ram)[ip + src] << " " << ip + dst);
}

void CPU::_SubVR(RamCellType src, RamCellType dst) {
    _UpdateEFlagsSub(src, _registers[dst & RegisterModMask]);
    _registers[dst & RegisterModMask] -= src;
    VOUT("SUB $" << src << " %" << RegisterName((Register)dst));
}

void CPU::_SubVM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsSub(src, (*_ram)[ip + dst]);
    (*_ram)[ip + dst] -= src;
    VOUT("SUB $" << src << " " << ip + dst);
}

// MUL
void CPU::_MulR(RamCellType src) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsMul(_registers[src & RegisterModMask], _registers[Register::EAX]);
    _registers[Register::EAX] *= _registers[src & RegisterModMask];
    VOUT("MUL %" << RegisterName((Register)src));
}

void CPU::_MulM(RamCellType src) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsMul((*_ram)[ip + src], _registers[Register::EAX]);
    _registers[Register::EAX] *= (*_ram)[ip + src];
    VOUT("MUL " << (*_ram)[ip + src]);
}

// CMP
void CPU::_CmpRR(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsCmp(_registers[src & RegisterModMask], _registers[dst & RegisterModMask]);
    VOUT("CMP %" << RegisterName((Register)src) << " %" << RegisterName((Register)dst));
}

void CPU::_CmpRM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsCmp(_registers[src & RegisterModMask], (*_ram)[ip + dst]);
    VOUT("CMP %" << RegisterName((Register)src) << " " << ip + dst);
}

void CPU::_CmpMR(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsCmp((*_ram)[ip + src], _registers[dst & RegisterModMask]);
    VOUT("CMP " << (*_ram)[ip + src] << " %" << RegisterName((Register)src));
}

void CPU::_CmpMM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsCmp((*_ram)[ip + src], (*_ram)[ip + dst]);
    VOUT("CMP " << (*_ram)[ip + src] << " " << ip + dst);
}

void CPU::_CmpVR(RamCellType src, RamCellType dst) {
    _UpdateEFlagsCmp(src, _registers[dst & RegisterModMask]);
    VOUT("CMP $" << src << " %" << RegisterName((Register)dst));
}

void CPU::_CmpVM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsCmp(src, (*_ram)[ip + dst]);
    VOUT("CMP $" << src << " " << ip + dst);
}

void CPU::_AndRR(RamCellType src, RamCellType dst) {
    _UpdateEFlagsAnd(_registers[src & RegisterModMask], _registers[dst & RegisterModMask]);
    _registers[dst & RegisterModMask] &= _registers[src & RegisterModMask];
    VOUT("AND %" << RegisterName((Register)src) << " %" << RegisterName((Register)dst));
}

void CPU::_OrRR(RamCellType src, RamCellType dst) {
    _UpdateEFlagsOr(_registers[src & RegisterModMask], _registers[dst & RegisterModMask]);
    _registers[dst & RegisterModMask] &= _registers[src & RegisterModMask];
    VOUT("OR %" << RegisterName((Register)src) << " %" << RegisterName((Register)dst));
}

void CPU::_XorRR(RamCellType src, RamCellType dst) {
    _UpdateEFlagsXor(_registers[src & RegisterModMask], _registers[dst & RegisterModMask]);
    _registers[dst & RegisterModMask] ^= _registers[src & RegisterModMask];
    VOUT("XOR %" << RegisterName((Register)src) << " %" << RegisterName((Register)dst));
}

void CPU::_LShiftRR(RamCellType src, RamCellType dst) {
    _registers[dst & RegisterModMask] <<= _registers[src & RegisterModMask];
    VOUT("LSHIFT %" << RegisterName((Register)src) << " %" << RegisterName((Register)dst));
}

void CPU::_RShiftRR(RamCellType src, RamCellType dst) {
    _registers[dst & RegisterModMask] <<= _registers[src & RegisterModMask];
    VOUT("RSHIFT %" << RegisterName((Register)src) << " %" << RegisterName((Register)dst));
}

// JMP
void CPU::_Jump(RamCellType reg) {
    _registers[Register::EIP] += _registers[reg & RegisterModMask];
}

// JMP
void CPU::_Jmp(RamCellType reg) {
    VOUT("JMP %" << RegisterName((Register)reg));
    _Jump(reg);
}

// Jcc
bool CPU::_JA(RamCellType reg) {
    VOUT("JA %" << RegisterName((Register)reg));
    if (!_GetEFlag(EFlag::CF) && !_GetEFlag(EFlag::ZF)) {
        _Jump(reg);
        return true;
    }
    return false;
}

bool CPU::_JC(RamCellType reg) {
    VOUT("JC %" << RegisterName((Register)reg));
    if (_GetEFlag(EFlag::CF)) {
        _Jump(reg);
        return true;
    }
    return false;
}

bool CPU::_JE(RamCellType reg) {
    VOUT("JE %" << RegisterName((Register)reg));
    if (_GetEFlag(EFlag::ZF)) {
        _Jump(reg);
        return true;
    }
    return false;
}

bool CPU::_JG(RamCellType reg) {
    VOUT("JG %" << RegisterName((Register)reg));
    if (!_GetEFlag(EFlag::ZF) && (_GetEFlag(EFlag::SF) == _GetEFlag(EFlag::OF))) {
        _Jump(reg);
        return true;
    }
    return false;
}

bool CPU::_JL(RamCellType reg) {
    VOUT("JL %" << RegisterName((Register)reg));
    if ((_GetEFlag(EFlag::SF) != _GetEFlag(EFlag::OF))) {
        _Jump(reg);
        return true;
    }
    return false;
}

bool CPU::_JO(RamCellType reg) {
    VOUT("JO %" << RegisterName((Register)reg));
    if (_GetEFlag(EFlag::OF)) {
        _Jump(reg);
        return true;
    }
    return false;
}

bool CPU::_JP(RamCellType reg) {
    VOUT("JP %" << RegisterName((Register)reg));
    if (_GetEFlag(EFlag::PF)) {
        _Jump(reg);
        return true;
    }
    return false;
}

bool CPU::_JS(RamCellType reg) {
    VOUT("JS %" << RegisterName((Register)reg));
    if (_GetEFlag(EFlag::SF)) {
        _Jump(reg);
        return true;
    }
    return false;
}

bool CPU::_JZ(RamCellType reg) {
    VOUT("JZ %" << RegisterName((Register)reg));
    if (_GetEFlag(EFlag::ZF)) {
        _Jump(reg);
        return true;
    }
    return false;
}

void CPU::_Fork(RamCellType reg, uint64_t ticksToLive, uint32_t color) {
    size_t offset = _registers[reg & RegisterModMask]; 
    size_t addr = _registers[Register::EIP] + offset;
    VOUT("FORK $" << offset);
    size_t realOffset = std::min((size_t)offset, _ram->GetAddressMask() - offset);
    if (realOffset >= PageCells) {
        // cannot FORK from near
        std::vector<Page> code(1, Page{});
        _ram->Read(addr, code[0].data(), PageCells);
        _ram->SetRandom(addr, PageCells);
        InterconnectProxy::GetLocal()->AnyCast(
                    MessageProxy::GetLocal()->GetId(),
                    new MessageFork(std::move(code), ticksToLive, color));
    }
}

void CPU::_ConsumeBlock(RamCellType reg) {
    size_t offset = _registers[reg & RegisterModMask]; 
    size_t addr = _registers[Register::EIP] + offset;
    VOUT("CONSUME_BLOCK $" << offset);
    uint32_t cellsConsumed = _ram->CountNulls(addr, ConsumedBlockCells);
    if (cellsConsumed >= ConsumedBlockCells / 2) {
        _ram->SetRandom(addr, ConsumedBlockCells);
        MessageProxy::GetLocal()->Send(
                MessageProxy::GetLocal()->GetId(),
                MessageProxy::GetLocal()->GetId(),
                new MessageBlockConsumed(_registers[Register::EID], cellsConsumed));
    }
}

constexpr RamCellType HIGHER_BIT = 1 << (sizeof(RamCellType) - 1);

// EFlags
void CPU::_UpdateEFlagsAdd(RamCellType src, RamCellType dst) {
    RamCellType res = src + dst;
    _SetEFlag(CF, res < dst);
    _SetEFlag(PF, res % 2 == 0);
    _SetEFlag(ZF, res == 0);
    _SetEFlag(OF, res < dst);
    _SetEFlag(SF, res & HIGHER_BIT);
}

void CPU::_UpdateEFlagsSub(RamCellType src, RamCellType dst) {
    RamCellType res = dst - src;
    _SetEFlag(CF, res > dst);
    _SetEFlag(PF, res % 2 == 0);
    _SetEFlag(ZF, res == 0);
    _SetEFlag(OF, res > dst);
    _SetEFlag(SF, res & HIGHER_BIT);
}

void CPU::_UpdateEFlagsAnd(RamCellType src, RamCellType dst) {
    RamCellType res = dst & src;
    _SetEFlag(CF, 0);
    _SetEFlag(OF, 0);
    _SetEFlag(PF, res % 2 == 0);
    _SetEFlag(ZF, res == 0);
    _SetEFlag(SF, res & HIGHER_BIT);
}

void CPU::_UpdateEFlagsOr(RamCellType src, RamCellType dst) {
    RamCellType res = dst | src;
    _SetEFlag(CF, 0);
    _SetEFlag(PF, res % 2 == 0);
    _SetEFlag(ZF, res == 0);
    _SetEFlag(OF, 0);
    _SetEFlag(SF, res & HIGHER_BIT);
}

void CPU::_UpdateEFlagsXor(RamCellType src, RamCellType dst) {
    RamCellType res = dst ^ src;
    _SetEFlag(CF, 0);
    _SetEFlag(PF, res % 2 == 0);
    _SetEFlag(ZF, res == 0);
    _SetEFlag(OF, 0);
    _SetEFlag(SF, res & HIGHER_BIT);
}

void CPU::_UpdateEFlagsCmp(RamCellType src, RamCellType dst) {
    _UpdateEFlagsSub(src, dst);
}

void CPU::_UpdateEFlagsMul(RamCellType src, RamCellType dst) {
    RamCellType res = dst * src;
    _SetEFlag(CF, res < dst);
    _SetEFlag(PF, res % 2 == 0);
    _SetEFlag(ZF, res == 0);
    _SetEFlag(OF, res < dst);
    _SetEFlag(SF, res & HIGHER_BIT);
}

bool CPU::_GetEFlag(EFlag flag) const {
    return _registers[Register::EFLAGS] & (1 << flag);
}

void CPU::_SetEFlag(EFlag flag, bool val) {
    if (val) {
        _registers[Register::EFLAGS] |= (1 << flag);
    } else {
        _registers[Register::EFLAGS] &= ~(1 << flag);
    }
}

