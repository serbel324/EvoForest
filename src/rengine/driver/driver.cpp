#include <rengine/driver/driver.h>

#include <cassert>
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>
#include <utility>

namespace REngine {

static Driver::UPtr __driverKing;

void Driver::Promote(Driver::UPtr&& newKing)
{
    __driverKing.reset(newKing.release());
}

Driver* Driver::King() {
    return __driverKing.get();
}


SingleFrameDriver::SingleFrameDriver(Frame::UPtr&& frame, Settings settings)
    : _frame(std::forward<Frame::UPtr>(frame))
    , _settings(settings)
{}

void SingleFrameDriver::Initialize() {
    _frame->Initialize();
}

void SingleFrameDriver::Run() {
    using SystemClock = std::chrono::system_clock;
    using Timestamp = std::chrono::time_point<SystemClock>;
    Timestamp startTime = SystemClock::now();

    while (true) {
        Timestamp currentTime = SystemClock::now();
        int64_t elapsedUs = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - startTime).count();
        float elapsedMs = 0.001 * elapsedUs;
        if (_settings.minimumUpdateDelayMs > elapsedMs) {
            std::this_thread::sleep_for(std::chrono::milliseconds((int)(_settings.minimumUpdateDelayMs - elapsedMs)));
            elapsedMs = _settings.minimumUpdateDelayMs;
        }
        startTime = SystemClock::now();

        if (!_frame->Update(elapsedMs)) {
            break;
        }
        _frame->Render();
    }
}

} // namespace REngine