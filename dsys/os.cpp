#include <utility>

#include <dsys/os.h>

#include <dsys/message_fork.h>

EvOS::EvOS(const std::shared_ptr<CPU>& cpu, const std::shared_ptr<RAM>& ram)
    : _cpu(cpu)
    , _ram(ram)
    , _allocator(RAMSize / PageSize)
{
    localOs = this;
}

void EvOS::SendMessage(Message::SPtr msg) {
    _messages.emplace_back(std::move(msg));
}

void EvOS::HandleMessage(Message::SPtr msg) {
    switch (msg->GetType()) {
    case MessageType::FORK: {
        MessageFork* msgFork = msg->As<MessageFork>();
        Fork(std::move(msgFork->sourceCode));
        break;
    }
    case MessageType::SELF_KILL:
        break;
    }
}

void EvOS::Fork(std::vector<Page> code) {
    std::optional<PageIdx> firstPage = _allocator.Allocate(code.size());
    if (!firstPage) {
        return;
    }

    size_t addr = *firstPage * PageCells;
    std::vector<PageIdx> pages;
    for (size_t i = 0; i < code.size(); ++i) {
        pages.push_back((*firstPage + i) % (RAMSize / PageSize));
        for (size_t j = 0; j < PageCells; ++i) {
            (*_ram)[addr + i * PageCells + j] = code[i][j];
        }
    }

    CPU::Registers newRegs = {{0}};
    newRegs[Register::EIP] = addr;
    newRegs[Register::ESP] = addr;

    ExecutionContext newCtx{
        .id = ++_creatureIdx,
        .pages = std::move(pages),
        .registers = _cpu->DumpRegisters(),
        .ticksToLive = _defaultTicksToLive,
    };

    _ctxs.push_back(std::move(newCtx));
}
