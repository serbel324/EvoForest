#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include <dsys/arch.h>
#include <dsys/commands.h>
#include <dsys/os.h>
#include <dsys/ram.h>
#include <dsys/registers.h>


class CPU {
public:
    CPU(const std::shared_ptr<RAM>& _ram);

    using Registers = std::array<RegisterType, RegisterCount>;

public:
    void LoadRegisters(const Registers& registers);
    Registers DumpRegisters() const;

    void Tick();

private:
    // cmds
    void _Nop();

    void _MovRR(RamCellType src, RamCellType dst);
    void _MovMR(RamCellType src, RamCellType dst);
    void _MovRM(RamCellType src, RamCellType dst);
    void _MovMM(RamCellType src, RamCellType dst);

    void _AddRR(RamCellType src, RamCellType dst);
    void _AddMR(RamCellType src, RamCellType dst);
    void _AddRM(RamCellType src, RamCellType dst);
    void _AddMM(RamCellType src, RamCellType dst);

    void _SubRR(RamCellType src, RamCellType dst);
    void _SubMR(RamCellType src, RamCellType dst);
    void _SubRM(RamCellType src, RamCellType dst);
    void _SubMM(RamCellType src, RamCellType dst);

    void _MulR(RamCellType src);
    void _MulM(RamCellType src);

    void _CmpRR(RamCellType src, RamCellType dst);
    void _CmpMR(RamCellType src, RamCellType dst);
    void _CmpRM(RamCellType src, RamCellType dst);
    void _CmpMM(RamCellType src, RamCellType dst);

    void _Jmp(RamCellType offset);

    void _JA(RamCellType offset);
    void _JC(RamCellType offset);
    void _JE(RamCellType offset);
    void _JG(RamCellType offset);
    void _JL(RamCellType offset);
    void _JO(RamCellType offset);
    void _JP(RamCellType offset);
    void _JS(RamCellType offset);

    void _Fork();
    void _KillSelf();

private:
    void _UpdateEFlagsAdd(RamCellType src, RamCellType dst);
    void _UpdateEFlagsSub(RamCellType src, RamCellType dst);
    void _UpdateEFlagsMul(RamCellType src, RamCellType dst);

    void _SetEFlag(EFlag flag, bool val);
    bool _GetEFlag(EFlag flag) const;

private:
    Registers _registers;
    std::shared_ptr<RAM> _ram;
};
