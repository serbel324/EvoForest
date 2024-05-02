#pragma once

#include <cstddef>
#include <cstdint>

#include <dsys/arch.h>
#include <dsys/literals.h>


constexpr size_t RAMSize = 1_MB;
constexpr size_t RegisterCount = 32;

using RamCellType = uint32_t;
using RegisterType = uint32_t;
using AddressType = uint32_t;
using CmdType = uint8_t;
