#pragma once

#include <chrono>

using Clock = std::chrono::steady_clock;
using Timestamp = std::chrono::time_point<Clock>;
using Duration = std::chrono::duration<float>;

using Sec = std::chrono::seconds;
using Ms = std::chrono::milliseconds;
using Us = std::chrono::microseconds;
using Ns = std::chrono::nanoseconds;

inline float ToSec(Duration duration) {
    return std::chrono::duration_cast<Sec>(duration).count();
}

inline float ToMs(Duration duration) {
    return std::chrono::duration_cast<Ms>(duration).count();
}

inline float ToUs(Duration duration) {
    return std::chrono::duration_cast<Us>(duration).count();
}

inline float ToNs(Duration duration) {
    return std::chrono::duration_cast<Ns>(duration).count();
}
