#include <dsys/arch.h>
#include <dsys/cli.h>
#include <dsys/process.h>
#include <dsys/messaging/interconnect_proxy.h>
#include <thread>
#include <vector>

int main() {
    // TODO(threads) multi-threaded
    std::vector<Process> processes;
    uint32_t max_threads = std::thread::hardware_concurrency();
    uint32_t threads = (max_threads > 3) ? max_threads - 3 : 1;

    InterconnectProxy icProxy(threads);
    for (uint32_t i = 0; i < threads; ++i) {
        processes.push_back(Process(RAMSize, i));
    }
    for (uint32_t i = 0; i < threads; ++i) {
        processes[i].Launch();
    }

    CLI cli(threads);
    cli.Run();

    for (uint32_t i = 0; i < threads; ++i) {
        processes.back().Join();
    }
}
