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
//   * SRC  - source address type, R for register, M for memory, V for value
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
    GEN_DST(NAME##M),               \
    GEN_DST(NAME##V)

///////////////////////////////////////////////////////////////////////////////

    MOVRR = 0,
    MOVOFFSR,

    MOVVR,
    MOVOFFSM,
    MOVOFFSTOR,
    MOVOFFSTOM,

    MOVRM,
    MOVMR,
    MOVMM,
    MOVVM,

    // GEN_SRC_DST(ADD),
    // All binary operators are register-to-register only
    ADDRR,
    SUBRR,
    CMPRR,
    LSHIFTRR,
    RSHIFTRR,
    ANDRR,
    ORRR,
    XORRR,

    // GEN_SRC_DST(SUB),  // more or less same as add

    // MULR,
    // MULM,
    // DIVR,
    // DIVM,

    // PUSH,
    // POP,

    // GEN_SRC_DST(CMP),

    //  JMP argument is address of type AddressType
    JMP,

    //  conditional jumps
    // JA,     // above
    // JAE,    // above or equal
    // JB,     // below
    // JBE,    // below or equal
    // JC,     // carry
    // JE,     // equal
    // JG,     // greater
    // JL,     // less
    // JLE,    // less or equal
    // JO,     // overflow
    // JP,     // parity
    // JPE,    // parity even
    // JPO,    // parity even
    // JS,     // sign
    JZ,     // zero

    // Fork (address)
    FORK,
    CONSUME_BLOCK,
    NOP,

    // number of commands
    __CMD_COUNT__ = 32,

///////////////////////////////////////////////////////////////////////////////

#undef GEN_SRC_DST
#undef GEN_DST

};

inline size_t IpOffset(Command cmd) {
    switch (cmd) {
    case Command::NOP:
        return 1;

    // case Command::MULR:
    // case Command::MULM:
    case Command::JMP:
    // case Command::JA:
    // case Command::JC:
    // case Command::JE:
    // case Command::JG:
    // case Command::JL:
    // case Command::JO:
    // case Command::JP:
    // case Command::JS:
    case Command::JZ:
        return 2;

    case Command::FORK:
    case Command::CONSUME_BLOCK:

    case Command::MOVOFFSR:
    case Command::MOVOFFSM:
    case Command::MOVOFFSTOR:
    case Command::MOVOFFSTOM:
        return 2;

    case Command::MOVRR:
    case Command::MOVMR:
    case Command::MOVVR:
    case Command::MOVRM:
    case Command::MOVMM:
    case Command::MOVVM:

    case Command::ADDRR:
    // case Command::ADDMR:
    // case Command::ADDVR:
    // case Command::ADDRM:
    // case Command::ADDMM:
    // case Command::ADDVM:

    case Command::SUBRR:
    // case Command::SUBMR:
    // case Command::SUBRM:
    // case Command::SUBMM:

    case Command::CMPRR:
    // case Command::CMPMR:
    // case Command::CMPVR:
    // case Command::CMPRM:
    // case Command::CMPMM:
    // case Command::CMPVM:

    case Command::LSHIFTRR:
    case Command::RSHIFTRR:
    case Command::ANDRR:
    case Command::ORRR:
    case Command::XORRR:
        return 3;

    default:
        return 1; // NOP
    }
}

constexpr size_t CommandModMask = Command::__CMD_COUNT__ - 1;
