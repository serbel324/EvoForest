#pragma once

#include <cstdint>

#include <dsys/arch.h>

enum Register : uint8_t {
    // named general purpose registers
    EAX = 0,
    EBX,
    ECX,
    EDX,
    ESI,
    EDI,

    // stack pointer
    ESP,
    EBP,

    // instruction pointer
    EIP,

    // self process id
    EID,

    // EFLAGS
    EFLAGS,

    // operation offset
    EOFFS,
    // operation size
    ESIZE,

    // some unnamed registers
    // ...
    REG13,
    REG14,
    REG15,

    __REG_COUNT__ = RegisterCount,
};

enum EFlag : uint8_t {
    CF = 0,
    PF,
    ZF,
    SF,
    OF,
};

constexpr uint8_t RegisterModMask = Register::__REG_COUNT__ - 1;

inline std::string RegisterName(Register reg) {
    uint8_t r = reg & RegisterModMask;
    #define NAMED(reg, name) case reg: return name

    switch (r) {
        NAMED(EIP, "EIP");
        NAMED(EID, "EID");
        NAMED(EFLAGS, "EFLAGS");
        NAMED(EOFFS, "EOFFS");
        NAMED(ESIZE, "ESIZE");
        NAMED(EAX, "EAX");
        NAMED(EBX, "EBX");
        NAMED(ECX, "ECX");
        NAMED(EDX, "EDX");
        NAMED(ESI, "ESI");
        NAMED(EDI, "EDI");
        NAMED(ESP, "ESP");
        NAMED(EBP, "EBP");
    default:
        return "REG" + std::to_string(r);
    }
}
