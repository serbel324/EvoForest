#pragma once

#include <cstdint>

// Credit: https://github.com/ydb-platform/ydb/blob/main/util/generic/size_literals.h

constexpr uint64_t operator"" _B(unsigned long long value) noexcept {
    return value;
}

constexpr uint64_t operator"" _KB(unsigned long long value) noexcept {
    return value * 1024;
}

constexpr uint64_t operator"" _MB(unsigned long long value) noexcept {
    return value * 1024_KB;
}

constexpr uint64_t operator"" _GB(unsigned long long value) noexcept {
    return value * 1024_MB;
}

constexpr uint64_t operator"" _TB(unsigned long long value) noexcept {
    return value * 1024_GB;
}
