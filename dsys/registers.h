#pragma once

#include <cstdint>

#include <dsys/arch.h>

enum Register : uint8_t {
    // instruction register
    EIP = 0,

    // self process id
    EID,

    // EFLAGS
    EFLAGS,

    // fork source code address
    EFADDR,
    // fork source code pages count
    EFSIZE,

    // named general purpose registers
    EAX,
    EBX,
    ECX,
    EDX,
    ESI,
    EDI,

    // stack pointer
    ESP,
    EBP,

    // some unnamed registers
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
