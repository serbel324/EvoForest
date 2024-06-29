#include "process.h"

#include <cassert>

Process::Process(uint64_t ramSize, uint32_t coreId)
    : _ramSize(ramSize)
    , _coreId(coreId)
{}

void Process::Launch() {
    auto f = [this]() {
        std::shared_ptr<RAM> ram = std::make_shared<RAM>(_ramSize);
        ram->Clear();
        std::shared_ptr<CPU> cpu = std::make_shared<CPU>(ram);
        std::shared_ptr<EvOS> os = std::make_shared<EvOS>(cpu, ram, _coreId);
        os->Run();
    };
    _t = std::thread(f);
}

void Process::Join() {
    if (_t.joinable()) {
        _t.join();
    }
}
