#pragma once

#include <dsys/cpu.h>
#include <dsys/execution_context.h>
#include <dsys/messaging/message.h>
#include <dsys/messaging/message_proxy.h>
#include <dsys/paging.h>
#include <dsys/stats_tracker.h>
#include <dsys/timer.h>

#include <deque>
#include <iostream>
#include <unordered_map>

static_assert(PageSize % sizeof(RamCellType) == 0);

class EvOS {
public:
    EvOS(const std::shared_ptr<CPU>& _cpu, const std::shared_ptr<RAM>& _ram, uint32_t coreId);

public:
    void Run();  // Returns on terminate

private:
    std::shared_ptr<CPU> _cpu;
    std::shared_ptr<RAM> _ram;
    uint32_t _coreId;

    size_t _pageCount;
    PageAllocator _allocator;
    std::vector<CreatureId> _pageClaims;

    std::deque<CreatureId> _executionOrder;
    std::unordered_map<CreatureId, ExecutionContext> _creatures;

    MessageProxy _messageProxy;

    uint64_t _creaturesForked = 0;

    // TODO config
    uint32_t _defaultCreatures = RAMSize / PageSize / 4;
    uint64_t _defaultTicksToLive = ConsumedBlockSize * 128;
    uint64_t _defaultTicksPerTurn = 10;
    uint64_t _defaultTicksPerConsumedBlock = ConsumedBlockSize * 128;
    float _secondsToSpawnRandom = 1;
    float _chanceToCorrupt = 0.01;
    float _chanceToChangeColor = 0.001;
    
    bool _running;

    // observability
    Timestamp _lastPrint;
    Timestamp _lastSpawnedRandom;

    Statistics _stats;

private:
    void _HandleMessage(MessageWrapper::SPtr msg);

    void _CorruptCode(std::vector<Page>& code, float chanceToCorrupt);

    void _Fork(std::vector<Page> code, uint64_t ticksToLive, std::optional<uint32_t> parentColor);
    void _BlockConsumed(CreatureId creatureId, uint32_t cellsConsumed);

    void _MakeRandomCreature();

    void _DumpMemory(std::ostream& out);
    void _DumpPages(std::ostream& out);
    void _DumpRegisters(std::ostream& out, const CPU::Registers& registers);

    void _CreateInitialPopulation();
};
