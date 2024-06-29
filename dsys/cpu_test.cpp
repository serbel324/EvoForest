#include <dsys/cpu.h>
#include <dsys/ram.h>

#include <library/ext_math.h>

#include <iostream>

int main() {
    size_t ramSize = 256;
    std::shared_ptr<RAM> ram = std::make_shared<RAM>(ramSize);
    for (size_t i = 0; i < ramSize; ++i) {
        (*ram)[i] = ExtMath::RandomUint32();
    }

    CPU cpu(ram);

    for (size_t i = 0; i < 10000; ++i) {
        std::cout << " ------------------ iteration# " << i << " ------------------" << std::endl;
        std::cout << "RAM: " << std::endl;
        for (size_t i = 0; i < ramSize; ++i) {
            std::cout << "0x" << std::hex << (*ram)[i] << " ";
        }
        std::cout << std::endl;

        auto registers = cpu.DumpRegisters();

        std::cout << "REGISTERS: " << std::endl;
        for (auto reg : registers) {
            std::cout << std::dec << reg << " ";
        }
        std::cout << std::endl;

        // cpu.Tick();
    }
}
