#include <rengine/driver/driver.h>
#include <forest/rengine_impl/main_frame.h>

using namespace REngine;

int main() {
    Driver::Promote(std::make_unique<SingleFrameDriver>(
            std::make_unique<MainFrame>(),
            SingleFrameDriver::Settings{ .minimumUpdateDelayMs = 100 } ));
    Driver::King()->Initialize();
    Driver::King()->Run();
}
