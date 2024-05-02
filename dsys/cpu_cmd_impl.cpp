#include <dsys/cpu.h>

// NOP
void CPU::_Nop() {
    return;
}

// MOV
void CPU::_MovRR(RamCellType src, RamCellType dst) {
    _registers[dst & RegisterModMask] = _registers[src & RegisterModMask];
}

void CPU::_MovRM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    (*_ram)[ip + dst] = _registers[src & RegisterModMask];
}

void CPU::_MovMR(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _registers[dst & RegisterModMask] = (*_ram)[ip + src];
}

void CPU::_MovMM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    (*_ram)[ip + dst] = (*_ram)[ip + src];
}

// ADD
void CPU::_AddRR(RamCellType src, RamCellType dst) {
    _UpdateEFlagsAdd(_registers[src & RegisterModMask], _registers[dst & RegisterModMask]);
    _registers[dst & RegisterModMask] += _registers[src & RegisterModMask];
}

void CPU::_AddRM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsAdd(_registers[src & RegisterModMask], (*_ram)[ip + dst]);
    (*_ram)[ip + dst] += _registers[src & RegisterModMask];
}

void CPU::_AddMR(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsAdd((*_ram)[ip + src], _registers[dst & RegisterModMask]);
    _registers[dst & RegisterModMask] += (*_ram)[ip + src];
}

void CPU::_AddMM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsAdd((*_ram)[ip + src], (*_ram)[ip + dst]);
    (*_ram)[ip + dst] += (*_ram)[ip + src];
}

// SUB
void CPU::_SubRR(RamCellType src, RamCellType dst) {
    _UpdateEFlagsSub(_registers[src & RegisterModMask], _registers[dst & RegisterModMask]);
    _registers[dst & RegisterModMask] -= _registers[src & RegisterModMask];
}

void CPU::_SubRM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsSub(_registers[src & RegisterModMask], (*_ram)[ip + dst]);
    (*_ram)[ip + dst] -= _registers[src & RegisterModMask];
}

void CPU::_SubMR(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsSub((*_ram)[ip + src], _registers[dst & RegisterModMask]);
    _registers[dst & RegisterModMask] -= (*_ram)[ip + src];
}

void CPU::_SubMM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsSub((*_ram)[ip + src], (*_ram)[ip + dst]);
    (*_ram)[ip + dst] -= (*_ram)[ip + src];
}

// MUL
void CPU::_MulR(RamCellType src) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsMul(_registers[src & RegisterModMask], _registers[Register::EAX]);
    _registers[Register::EAX] *= _registers[src & RegisterModMask];
}

void CPU::_MulM(RamCellType src) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsMul((*_ram)[ip + src], _registers[Register::EAX]);
    _registers[Register::EAX] *= (*_ram)[ip + src];
}

// CMP
void CPU::_CmpRR(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsSub(_registers[src & RegisterModMask], _registers[dst & RegisterModMask]);
}

void CPU::_CmpRM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsSub(_registers[src & RegisterModMask], (*_ram)[dst]);
}

void CPU::_CmpMR(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsSub((*_ram)[src], _registers[dst & RegisterModMask]);
}

void CPU::_CmpMM(RamCellType src, RamCellType dst) {
    size_t ip = _registers[Register::EIP];
    _UpdateEFlagsSub((*_ram)[src], (*_ram)[dst]);
}

// JMP
void CPU::_Jmp(RamCellType offset) {
    _registers[Register::EIP] += offset;
}

// Jcc
void CPU::_JA(RamCellType offset) {
    if (!_GetEFlag(EFlag::CF) && !_GetEFlag(EFlag::ZF)) {
        _Jmp(offset);
    }
}

void CPU::_JC(RamCellType offset) {
    if (_GetEFlag(EFlag::CF)) {
        _Jmp(offset);
    }
}

void CPU::_JE(RamCellType offset) {
    if (_GetEFlag(EFlag::ZF)) {
        _Jmp(offset);
    }
}

void CPU::_JG(RamCellType offset) {
    if (!_GetEFlag(EFlag::ZF) && (_GetEFlag(EFlag::SF) == _GetEFlag(EFlag::OF))) {
        _Jmp(offset);
    }
}

void CPU::_JL(RamCellType offset) {
    if ((_GetEFlag(EFlag::SF) != _GetEFlag(EFlag::OF))) {
        _Jmp(offset);
    }
}

void CPU::_JO(RamCellType offset) {
    if (_GetEFlag(EFlag::OF)) {
        _Jmp(offset);
    }
}

void CPU::_JP(RamCellType offset) {
    if (_GetEFlag(EFlag::PF)) {
        _Jmp(offset);
    }
}

void CPU::_JS(RamCellType offset) {
    if (_GetEFlag(EFlag::SF)) {
        _Jmp(offset);
    }
}

void CPU::_Fork() {
    // TODO
}

void CPU::_KillSelf() {
    // TODO
}

// EFlags
void CPU::_UpdateEFlagsAdd(RamCellType src, RamCellType dst) {
    RamCellType res = src + dst;
    _SetEFlag(CF, res < dst);
    _SetEFlag(PF, res % 2 == 0);
    _SetEFlag(ZF, res == 0);
    _SetEFlag(OF, res < dst);
    _SetEFlag(SF, res >> (sizeof(RamCellType) - 1));
}

void CPU::_UpdateEFlagsSub(RamCellType src, RamCellType dst) {
    RamCellType res = dst - src;
    _SetEFlag(CF, res > dst);
    _SetEFlag(PF, res % 2 == 0);
    _SetEFlag(ZF, res == 0);
    _SetEFlag(OF, res > dst);
    _SetEFlag(SF, res >> (sizeof(RamCellType) - 1));
}

void CPU::_UpdateEFlagsMul(RamCellType src, RamCellType dst) {
    RamCellType res = dst * src;
    _SetEFlag(CF, res < dst);
    _SetEFlag(PF, res % 2 == 0);
    _SetEFlag(ZF, res == 0);
    _SetEFlag(OF, res < dst);
    _SetEFlag(SF, res >> (sizeof(RamCellType) - 1));
}

bool CPU::_GetEFlag(EFlag flag) const {
    return _registers[Register::EFLAGS] & (1 << flag);
}

void CPU::_SetEFlag(EFlag flag, bool val) {
    _registers[Register::EFLAGS] ^= ((!val) << flag);
}

