#include <utility>
#include <chrono>

#include <dsys/presets.h>
#include <dsys/os.h>

#include <dsys/colors.h>
#include <dsys/messaging/message.h>
#include <dsys/messaging/message_dump.h>
#include <dsys/messaging/message_fork.h>
#include <dsys/messaging/message_block_consumed.h>

#include <library/ext_math.h>

#include <chrono>
#include <fstream>
#include <thread>

EvOS::EvOS(const std::shared_ptr<CPU>& cpu, const std::shared_ptr<RAM>& ram, uint32_t coreId)
    : _cpu(cpu)
    , _ram(ram)
    , _coreId(coreId)
    , _pageCount(RAMSize / PageSize)
    , _allocator(_pageCount)
    , _messageProxy({0, coreId})
{
    std::srand(std::time({}));
    _pageClaims.resize(_pageCount);
    _lastPrint = {};
    _lastSpawnedRandom = Clock::now();
}

void EvOS::_HandleMessage(MessageWrapper::SPtr msg) {
    switch (msg->GetType()) {
    case MessageType::MSG_FORK: {
        MessageFork* msgFork = msg->As<MessageFork>();
        _CorruptCode(msgFork->sourceCode, _chanceToCorrupt);
        _Fork(std::move(msgFork->sourceCode), msgFork->ticksToLive, msgFork->color);
        break;
    }
    case MessageType::MSG_BLOCK_CONSUMED: {
        MessageBlockConsumed* msgBlockConsumed = msg->As<MessageBlockConsumed>();
        _BlockConsumed(msgBlockConsumed->creatureId, msgBlockConsumed->cellsConsumed);
        break;
    }

    case MessageType::MSG_DUMP: {
        MessageDump* msgDump = msg->As<MessageDump>();
        switch (msgDump->dumpedInfo) {
        case MessageDump::EDumpedInfo::MEMORY:
            if (msgDump->toFile) {
                std::ofstream fout("memory" + std::to_string(_coreId) + ".dump");
                _DumpMemory(fout);
            } else {
                _DumpMemory(std::cout);
            }
            break;
        case MessageDump::EDumpedInfo::PAGES:
            if (msgDump->toFile) {
                std::ofstream fout("pages" + std::to_string(_coreId) + ".dump");
                _DumpPages(fout);
            } else {
                _DumpPages(std::cout);
            }
            break;
        case MessageDump::EDumpedInfo::STATS:
            if (msgDump->toFile) {
                std::ofstream fout("stats" + std::to_string(_coreId) + ".dump");
                _stats.Print(fout);
            } else {
                _stats.Print(std::cout);
            }
            break;
        default:
            std::cout << "Bad enum!" << std::endl;
            abort();
        }
        break;
    }
    case MessageType::MSG_STOP: {
       _running = false;
       break;
    }
    default:
        abort();
    }
}

void EvOS::Run() {
    _running = true;
    while (_running) {
        if (_creatures.empty()) {
            _stats.creationTime = Clock::now();
            _CreateInitialPopulation();
        }

        if (!_creatures.empty()) {
            CreatureId creatureId = _executionOrder.front();
            _executionOrder.pop_front();

            if (auto it = _creatures.find(creatureId); it != _creatures.end()) {
                ExecutionContext& creature = it->second;
                if (creature.ticksToLive == 0) {
                    // delete if expired
                    for (PageIdx pageIdx : creature.pages) {
                        if (_pageClaims[pageIdx] == creature.id) {
                            _pageClaims[pageIdx] = 0;
                            _allocator.Free(pageIdx, 1);
                        }
                    }
                    _stats.deadCreatures++;
                    _creatures.erase(it);
                } else {
                    // if alive, load and do some ticks
                    _cpu->LoadRegisters(creature.registers);
                    for (size_t i = 0; i < _defaultTicksPerTurn && creature.ticksToLive > 0; ++i) {
                        _cpu->Tick(creature.ticksToLive, _defaultTicksToLive, creature.color);
                        --creature.ticksToLive;

                        if constexpr(false) {
                            _DumpMemory(std::cout);
                            _DumpRegisters(std::cout, _cpu->DumpRegisters());
                            using namespace std::literals::chrono_literals;
                            std::this_thread::sleep_for(100ms);
                        }
                    }
                    creature.registers = _cpu->DumpRegisters();
                    _executionOrder.push_back(creatureId);
                }
            }
        }

        // handle messages
        MessageWrapper::SPtr msg = _messageProxy.Receive();
        while (msg) {
            _HandleMessage(msg);
            msg = _messageProxy.Receive();
        }

        Timestamp now = Clock::now();

        // if (_lastSpawnedRandom == Timestamp{} || ToSec(now - _lastSpawnedRandom) > _secondsToSpawnRandom) {
        //     _MakeRandomCreature();
        //     _lastSpawnedRandom = now;
        // }
    }
}

void EvOS::_MakeRandomCreature() {
    std::vector<Page> code;
    code.push_back(Page{});
    for (size_t i = 0; i < PageCells; ++i) {
        code[0][i] = ExtMath::RandomUint32() & bitMaskDistribution[ExtMath::RandomInt(0, bitMaskDistribution.size())];
    }

    _Fork(std::move(code), _defaultTicksToLive, {});
}

void EvOS::_Fork(std::vector<Page> code, uint64_t ticksToLive, std::optional<uint32_t> parentColor) {
    std::optional<PageIdx> firstPage = _allocator.Allocate(code.size());
    if (!firstPage) {
        _stats.failedForks++;
        return;
    }

    size_t addr = *firstPage * PageCells;
    CreatureId newId = ++_creaturesForked;
    std::vector<PageIdx> pages;
    pages.reserve(code.size());
    for (size_t i = 0; i < code.size(); ++i) {
        PageIdx pageIdx = (*firstPage + i) % _pageCount;
        pages.push_back(pageIdx);
        _pageClaims[pageIdx] = newId;
    }

    _ram->Set(*firstPage * PageCells, (RamCellType*)code.data(), pages.size() * PageCells);

    CPU::Registers newRegs = {{0}};
    newRegs[Register::EIP] = addr;
    newRegs[Register::ESP] = addr;
    newRegs[Register::EID] = newId;

    if (parentColor && ExtMath::RandomDouble(0, 1) < _chanceToChangeColor) {
        parentColor.emplace(ExtMath::RandomInt(0, Colors.size()));
    }

    _creatures[newId] = ExecutionContext{
        .id = newId,
        .pages = std::move(pages),
        .registers = newRegs,
        .ticksToLive = ticksToLive,
        .color = parentColor ? *parentColor : ExtMath::RandomInt(0, Colors.size()),
    };

    _executionOrder.push_back(newId);
    _stats.forks++;
}

void EvOS::_BlockConsumed(CreatureId creatureId, uint32_t cellsConsumed) {
    if (auto it = _creatures.find(creatureId); it != _creatures.end()) {
        ExecutionContext& creature = it->second;
        creature.ticksToLive += _defaultTicksPerConsumedBlock;
        _stats.blocksConsumed++;
    } else {
        _stats.blocksConsumptionsFailed++;
    }
}

void EvOS::_CorruptCode(std::vector<Page>& code, float chanceToCorrupt) {
    for (Page& page : code) {
        for (RamCellType& cell : page) {
            if (ExtMath::RandomDouble(0, 1) < chanceToCorrupt) {
                cell |= 1 << ExtMath::RandomInt(0, 32);
                _stats.codeCorruptions++;
            }
        }
    }
}

void EvOS::_CreateInitialPopulation() {
    _ram->FillRandom();
    _stats.ResetLocals();
    _stats.recreations++;

    if constexpr(true) {  // TODO config
        _Fork({replicator2}, _defaultTicksToLive, {});
    } else {
        for (uint32_t i = 0; i < _defaultCreatures; ++i) {
            _MakeRandomCreature();
        }
    }
}
