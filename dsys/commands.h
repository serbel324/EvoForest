#pragma once

#include <cassert>
#include <csignal>
#include <cstdint>

#include <dsys/arch.h>

///////////////////////////////////////////////////////////////////////////////
//
//  Comand naming:
//
//  CMD = NAME[[[SRC]DST]SIZE]
//   * NAME - unique string
//   * SRC  - source address type, R for register, M for memory
//   * DST  - destination address type, R for register, M for memory
//   * SIZE - size of argument in bytes: 8, 16 or 32
//
//  e.g. MOV16RM - move 16-bit value from register to memory
//
///////////////////////////////////////////////////////////////////////////////


enum Command : CmdType {
#define GEN_DST(NAME_SRC)           \
    NAME_SRC##R,                    \
    NAME_SRC##M

#define GEN_SRC_DST(NAME)           \
    GEN_DST(NAME##R),               \
    GEN_DST(NAME##M)

///////////////////////////////////////////////////////////////////////////////

    NOP = 0,

    GEN_SRC_DST(MOV),

    GEN_SRC_DST(ADD),
    GEN_SRC_DST(SUB),

    MULR,
    MULM,
    // DIVR,
    // DIVM,

    // PUSH,
    // POP,

    GEN_SRC_DST(CMP),

    //  JMP argument is address of type AddressType
    JMP,

    //  conditional jumps
    JA,     // above
    // JAE,    // above or equal
    // JB,     // below
    // JBE,    // below or equal
    JC,     // carry
    JE,     // equal
    JG,     // greater
    JL,     // less
    // JLE,    // less or equal
    JO,     // overflow
    JP,     // parity
    // JPE,    // parity even
    // JPO,    // parity even
    JS,     // sign
    // JZ,     // zero

    FORK,
    KILL_SELF,

    // number of commands
    __CMD_COUNT__ = 32,

///////////////////////////////////////////////////////////////////////////////

#undef GEN_SRC_DST
#undef GEN_DST

};

inline size_t CmdArgCount(Command cmd) {
    switch (cmd) {


    case Command::NOP:
    case Command::FORK:
    case Command::KILL_SELF:
        return 0;

    case Command::MULR:
    case Command::MULM:
    case Command::JMP:
    case Command::JA:
    case Command::JC:
    case Command::JE:
    case Command::JG:
    case Command::JL:
    case Command::JO:
    case Command::JP:
    case Command::JS:
        return 1;

    case Command::MOVRR:
    case Command::MOVMR:
    case Command::MOVRM:
    case Command::MOVMM:
    case Command::ADDRR:
    case Command::ADDMR:
    case Command::ADDRM:
    case Command::ADDMM:
    case Command::SUBRR:
    case Command::SUBMR:
    case Command::SUBRM:
    case Command::SUBMM:
    case Command::CMPRR:
    case Command::CMPMR:
    case Command::CMPRM:
    case Command::CMPMM:
        return 2;

    default:
        return 0;
    }
}

constexpr size_t CommandModMask = Command::__CMD_COUNT__ - 1;
