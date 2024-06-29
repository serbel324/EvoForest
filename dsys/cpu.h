#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include <dsys/arch.h>
#include <dsys/commands.h>
#include <dsys/ram.h>
#include <dsys/registers.h>

class CPU {
public:
    CPU(const std::shared_ptr<RAM>& _ram);

    using Registers = std::array<RegisterType, RegisterCount>;

public:
    void LoadRegisters(const Registers& registers);
    Registers DumpRegisters() const;

    void Tick(uint64_t& ticksToLive, uint64_t ticksToFork, uint32_t color);

private:
    // cmds
    void _Nop();

    void _MovRR(RamCellType src, RamCellType dst);
    void _MovMR(RamCellType src, RamCellType dst);
    void _MovVR(RamCellType src, RamCellType dst);
    void _MovRM(RamCellType src, RamCellType dst);
    void _MovMM(RamCellType src, RamCellType dst);
    void _MovVM(RamCellType src, RamCellType dst);

    void _MovOffsR(RamCellType src);
    void _MovOffsM(RamCellType src);
    void _MovOffsToR(RamCellType src);
    void _MovOffsToM(RamCellType src);

    void _AddRR(RamCellType src, RamCellType dst);
    void _AddMR(RamCellType src, RamCellType dst);
    void _AddVR(RamCellType src, RamCellType dst);
    void _AddRM(RamCellType src, RamCellType dst);
    void _AddMM(RamCellType src, RamCellType dst);
    void _AddVM(RamCellType src, RamCellType dst);

    void _SubRR(RamCellType src, RamCellType dst);
    void _SubMR(RamCellType src, RamCellType dst);
    void _SubVR(RamCellType src, RamCellType dst);
    void _SubRM(RamCellType src, RamCellType dst);
    void _SubMM(RamCellType src, RamCellType dst);
    void _SubVM(RamCellType src, RamCellType dst);

    void _MulR(RamCellType src);
    void _MulM(RamCellType src);

    void _CmpRR(RamCellType src, RamCellType dst);
    void _CmpMR(RamCellType src, RamCellType dst);
    void _CmpVR(RamCellType src, RamCellType dst);
    void _CmpRM(RamCellType src, RamCellType dst);
    void _CmpMM(RamCellType src, RamCellType dst);
    void _CmpVM(RamCellType src, RamCellType dst);

    void _AndRR(RamCellType src, RamCellType dst);
    void _OrRR(RamCellType src, RamCellType dst);
    void _XorRR(RamCellType src, RamCellType dst);

    void _LShiftRR(RamCellType src, RamCellType dst);
    void _RShiftRR(RamCellType src, RamCellType dst);

    void _Jump(RamCellType reg);

    void _Jmp(RamCellType reg);

    bool _JA(RamCellType reg);
    bool _JC(RamCellType reg);
    bool _JE(RamCellType reg);
    bool _JG(RamCellType reg);
    bool _JL(RamCellType reg);
    bool _JO(RamCellType reg);
    bool _JP(RamCellType reg);
    bool _JS(RamCellType reg);
    bool _JZ(RamCellType reg);

    void _Fork(RamCellType reg, uint64_t ticksToLive, uint32_t color);
    void _ConsumeBlock(RamCellType reg);

private:
    void _UpdateEFlagsAdd(RamCellType src, RamCellType dst);
    void _UpdateEFlagsSub(RamCellType src, RamCellType dst);
    void _UpdateEFlagsMul(RamCellType src, RamCellType dst);
    void _UpdateEFlagsCmp(RamCellType src, RamCellType dst);
    void _UpdateEFlagsAnd(RamCellType src, RamCellType dst);
    void _UpdateEFlagsOr(RamCellType src, RamCellType dst);
    void _UpdateEFlagsXor(RamCellType src, RamCellType dst);
    void _UpdateEFlagsLShift(RamCellType src, RamCellType dst);
    void _UpdateEFlagsRShift(RamCellType src, RamCellType dst);

    void _SetEFlag(EFlag flag, bool val);
    bool _GetEFlag(EFlag flag) const;

private:
    Registers _registers;
    std::shared_ptr<RAM> _ram;
};
