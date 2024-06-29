#include <dsys/cpu.h>
#include <dsys/messaging/message_proxy.h>

#include <iostream>

CPU::CPU(const std::shared_ptr<RAM>& ram)
    : _ram(ram)
{
    _registers = {{0}};
}

void CPU::LoadRegisters(const Registers& registers) {
    _registers = registers;
}

CPU::Registers CPU::DumpRegisters() const {
    return _registers;
}

void CPU::Tick(uint64_t& ticksToLive, uint64_t ticksToFork, uint32_t color) {
    uint32_t ip = _registers[Register::EIP];
    RamCellType arg1 = (*_ram)[ip + 1];
    RamCellType arg2 = (*_ram)[ip + 2];

    Command cmd = Command((*_ram)[ip] & CommandModMask);

    bool jumped = false;

    switch (cmd) {
    case Command::MOVRR:
        _MovRR(arg1, arg2);
        break;
    case Command::MOVRM:
        _MovRM(arg1, arg2);
        break;
    case Command::MOVMR:
        _MovMR(arg1, arg2);
        break;
    case Command::MOVMM:
        _MovMM(arg1, arg2);
        break;
    case Command::MOVVR:
        _MovVR(arg1, arg2);
        break;
    case Command::MOVVM:
        _MovVM(arg1, arg2);
        break;

    case Command::MOVOFFSR:
        _MovOffsR(arg1);
        break;
    case Command::MOVOFFSM:
        _MovOffsR(arg1);
        break;
    case Command::MOVOFFSTOR:
        _MovOffsToR(arg1);
        break;
    case Command::MOVOFFSTOM:
        _MovOffsToM(arg1);
        break;

    case Command::ADDRR:
        _AddRR(arg1, arg2);
        break;
/*
    case Command::ADDRM:
        _AddRM(arg1, arg2);
        break;
    case Command::ADDMR:
        _AddMR(arg1, arg2);
        break;
    case Command::ADDMM:
        _AddMM(arg1, arg2);
        break;
    case Command::ADDVR:
        _AddVR(arg1, arg2);
        break;
    case Command::ADDVM:
        _AddVM(arg1, arg2);
        break;
*/

    case Command::SUBRR:
        _SubRR(arg1, arg2);
        break;
/*
    case Command::SUBRM:
        _SubRM(arg1, arg2);
        break;
    case Command::SUBMR:
        _SubMR(arg1, arg2);
        break;
    case Command::SUBMM:
        _SubMM(arg1, arg2);
        break;
*/
    case Command::CMPRR:
        _CmpRR(arg1, arg2);
        break;
/*
    case Command::CMPRM:
        _CmpRM(arg1, arg2);
        break;
    case Command::CMPMR:
        _CmpMR(arg1, arg2);
        break;
    case Command::CMPMM:
        _CmpMM(arg1, arg2);
        break;
    case Command::CMPVR:
        _CmpVR(arg1, arg2);
        break;
    case Command::CMPVM:
        _CmpVM(arg1, arg2);
        break;
*/

    case Command::LSHIFTRR:
        _LShiftRR(arg1, arg2);
        break;

    case Command::RSHIFTRR:
        _RShiftRR(arg1, arg2);
        break;

    case Command::ANDRR:
        _AndRR(arg1, arg2);
        break;

    case Command::ORRR:
        _OrRR(arg1, arg2);
        break;

    case Command::XORRR:
        _XorRR(arg1, arg2);
        break;

/*
    case Command::MULR:
        _MulR(arg1);
        break;
    case Command::MULM:
        _MulM(arg1);
        break;
*/
    case Command::JMP:
        _Jmp(arg1);
        break;
/*
    case Command::JA:
        _JA(arg1);
        break;
    case Command::JC:
        _JC(arg1);
        break;
    case Command::JE:
        _JE(arg1);
        break;
    case Command::JG:
        _JG(arg1);
        break;
    case Command::JL:
        _JL(arg1);
        break;
    case Command::JO:
        _JO(arg1);
        break;
    case Command::JP:
        _JP(arg1);
        break;
    case Command::JS:
        _JS(arg1);
        break;
*/
    case Command::JZ:
        jumped = _JZ(arg1);
        break;
    
    case Command::FORK:
        if (ticksToLive > ticksToFork) {
            _Fork(arg1, ticksToLive / 2, color);
            ticksToLive /= 2;
        }
        break;
    
    case Command::CONSUME_BLOCK:
        _ConsumeBlock(arg1);
        break;

    default:
        _Nop();
    }

    if (!jumped) {
        _registers[Register::EIP] += IpOffset(cmd);
    }
}
