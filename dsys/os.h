#pragma once

#include <dsys/cpu.h>
#include <dsys/execution_context.h>
#include <dsys/message.h>
#include <dsys/paging.h>

#include <deque>


static_assert(PageSize % sizeof(RamCellType) == 0);

class EvOS {
public:
    EvOS(const std::shared_ptr<CPU>& _cpu, const std::shared_ptr<RAM>& _ram);

public:
    void SendMessage(Message::SPtr);

    void HandleMessage(Message::SPtr msg);

    void Fork(std::vector<Page> code);

private:
    std::shared_ptr<CPU> _cpu;
    std::shared_ptr<RAM> _ram;
    std::deque<ExecutionContext> _ctxs;

    // TODO implement cross-process message service
    std::deque<Message> _messages;

    PageAllocator _allocator;

    uint64_t _creatureIdx = 0;

    // TODO config
    uint64_t _defaultTicksToLive = 1000;
};

thread_local EvOS* localOs = nullptr;
