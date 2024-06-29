#pragma once

#include <dsys/commands.h>
#include <dsys/cpu.h>
#include <dsys/paging.h>

constexpr RamCellType FF = (RamCellType)(-1);

constexpr Page eater1 = {
    MOVRR, EIP, EAX,
    MOVVM, 0, PageCells * 2,  // 
    CONSUME_BLOCK, PageCells * 2 - 3,  //
    MOVRR, EAX, EIP,

};

constexpr Page replicator1 = {
    MOVRR, EIP, EBP,
    MOVVR, PageCells * 2, EBX,
    MOVVR, EIP, EOFFS,
    MOVVR, 5, ESI,
    NOP, NOP, NOP,
    MOVVR, PageCells, EDX,
    MOVVR, 1, ECX,
    MOVRR, EIP, REG13,
    // ip: 24

// consume cycle
    MOVVM, 0, PageCells * 2 + 3, 
    SUBRR, ECX, EDX,
    JZ, ESI, // to next
    MOVRR, REG13, EIP,
    CONSUME_BLOCK, EBX,
    // ip: 37

// next:
    MOVVR, 81, EDX,
    MOVVR, (uint32_t)-52, EDI,
    // ip: 43
    MOVRR, EBX, EOFFS,
    MOVRR, EBX, REG14,
    MOVRR, EIP, REG13,
    // ip: 52

// copy cycle
    MOVOFFSR, EDI,
    MOVOFFSTOR, REG14,
    ADDRR, ECX, EDI,
    ADDRR, ECX, REG14,
    SUBRR, ECX, EDX,
    JZ, ESI,
    MOVRR, REG13, EIP,
    // ip: 70

    MOVVR, 22, EBX,
    SUBRR, EBX, EOFFS,
    FORK, EOFFS,
    MOVRR, EBP, EIP
    // ip: 81
};

constexpr Page replicator2 = {
    MOVRR, EIP, EBP,
    MOVVR, PageCells * 2, EBX,
    MOVVR, PageCells - ConsumedBlockCells - 30, REG14,
    MOVVR, 37, REG15,
    SUBRR, REG15, REG14,
    MOVRR, REG14, EOFFS,
    MOVVR, 5, ESI,
    // ip: 18

    MOVVR, ConsumedBlockCells, EDX,
    MOVVR, 1, ECX,
    MOVRR, EIP, REG13,
    // ip: 30

// consume cycle
    MOVVR, 0, EAX,
    MOVOFFSTOR, EOFFS,
    ADDRR, ECX, EOFFS,
    SUBRR, ECX, EDX,
    JZ, ESI, // to next
    MOVRR, REG13, EIP,
    CONSUME_BLOCK, REG14,
    // ip: 48

// next:
    MOVVR, 92, EDX,
    MOVVR, (uint32_t)-63, EDI,
    // ip: 54
    MOVRR, EBX, EOFFS,
    MOVRR, EBX, REG14,
    MOVRR, EIP, REG13,
    // ip: 63

// copy cycle
    MOVOFFSR, EDI,
    MOVOFFSTOR, REG14,
    ADDRR, ECX, EDI,
    ADDRR, ECX, REG14,
    SUBRR, ECX, EDX,
    JZ, ESI,
    MOVRR, REG13, EIP,
    // ip: 81

    MOVVR, 22, EBX,
    SUBRR, EBX, EOFFS,
    FORK, EOFFS,
    MOVRR, EBP, EIP
    // ip: 92
};


constexpr Page replicator3 = {
    MOVRR, EIP, EBP,
    MOVVR, PageCells * 2, EBX,
    MOVRR, EBX, EOFFS,
    MOVVR, 5, ESI,
    // ip: 12

    MOVVR, ConsumedBlockCells + 30, EDX,
    MOVVR, 1, ECX,
    MOVVR, EBX, REG15,
    MOVRR, EIP, REG13,
    // ip: 24

// consume cycle
    MOVVR, 0, EAX,
    MOVOFFSTOR, EOFFS,
    ADDRR, ECX, EOFFS,
    SUBRR, ECX, EDX,
    JZ, ESI, // to next
    MOVRR, REG13, EIP,
    CONSUME_BLOCK, EBX,
    // ip: 42

// next:
    MOVVR, 86, EDX,
    MOVVR, (uint32_t)-57, EDI,
    // ip: 48
    MOVRR, EBX, EOFFS,
    MOVRR, EBX, REG14,
    MOVRR, EIP, REG13,
    // ip: 57

// copy cycle
    MOVOFFSR, EDI,
    MOVOFFSTOR, REG14,
    ADDRR, ECX, EDI,
    ADDRR, ECX, REG14,
    SUBRR, ECX, EDX,
    JZ, ESI,
    MOVRR, REG13, EIP,
    // ip: 75

    MOVVR, 22, EBX,
    SUBRR, EBX, EOFFS,
    FORK, EOFFS,
    MOVRR, EBP, EIP
    // ip: 86
};
