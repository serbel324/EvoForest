#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include <dsys/arch.h>
#include <dsys/literals.h>


constexpr size_t RAMSize = 32_MB;
constexpr size_t RegisterCount = 32;

using RamCellType = uint32_t;
using RegisterType = uint32_t;
using AddressType = uint32_t;
using CmdType = uint8_t;

constexpr size_t PageSize = 1_KB;
constexpr size_t PageCells = PageSize / sizeof(RamCellType);

constexpr size_t ConsumedBlockSize = PageSize / 8; // <- TODO config
constexpr size_t ConsumedBlockCells = ConsumedBlockSize / sizeof(RamCellType); // <- TODO config

using Page = std::array<RamCellType, PageCells>;
