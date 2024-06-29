#include "cli.h"

#include <iostream>
#include <sstream>
#include <thread>

#include <dsys/colors.h>
#include <dsys/messaging/message_dump.h>
#include <dsys/messaging/message_stop.h>
#include <dsys/messaging/interconnect_proxy.h>

std::vector<std::string> CLI::_GetArgs(const std::string& line) const {
    std::stringstream s;
    s << line;
    std::vector<std::string> res;
    std::string arg;
    while (s >> arg) {
        res.push_back(std::move(arg));
    }
    return std::move(res);
}

CLI::CLI(uint32_t processCount)
    : _processCount(processCount)
{}

void CLI::Run() {
    while (true) {
        std::string line;
        std::getline(std::cin, line);

        std::vector<std::string> args = _GetArgs(line);

        if (args.empty()) {
            continue;
        } else if (args[0] == "exit" || args[0] == "quit" || args[0] == "q") {
            for (uint32_t i = 0; i < _processCount; ++i) {
                InterconnectProxy::GetLocal()->Send(MessageBoxId::NULL_ADDRESS, {0, i}, new MessageStop);
            }
            break;
        } else if (args[0] == "dump" || args[0] == "d" || args[0] == "print" || args[0] == "p") {
            try {
                if (args.size() < 2 || args.size() > 3) {
                    throw 1;
                }
                uint32_t id = std::stoi(args[1]);
                if (id > _processCount) {
                    throw 2;
                }
                bool toFile = (args[0] == "dump" || args[0] == "d");
                MessageDump::EDumpedInfo dumpedInfo;
                if (args.size() == 2 || args[2] == "pages" || args[2] == "pg" || args[2] == "p") {
                    dumpedInfo = MessageDump::EDumpedInfo::PAGES;
                } else if (args[2] == "memory" || args[2] == "mem" || args[2] == "m") {
                    dumpedInfo = MessageDump::EDumpedInfo::MEMORY;
                } else if (args[2] == "statistics" || args[2] == "stats" || args[2] == "s") {
                    dumpedInfo = MessageDump::EDumpedInfo::STATS;
                } else {
                    throw 3;
                }

                if (id == 0) {
                    for (uint32_t i = 0; i < _processCount; ++i) {
                        std::cout << RESET_COLOR << "===================== Core " << (i + 1) <<
                                " =====================" << std::endl;
                        InterconnectProxy::GetLocal()->Send(MessageBoxId::NULL_ADDRESS, {0, i},
                                new MessageDump(dumpedInfo, toFile));
                        {  // TODO Handle response
                            using namespace std::chrono_literals;
                            std::this_thread::sleep_for(100ms);
                        }
                    }
                } else {
                    InterconnectProxy::GetLocal()->Send(MessageBoxId::NULL_ADDRESS, {0, id - 1},
                            new MessageDump(dumpedInfo, toFile));
                }
            } catch (...) {
                std::cout << "Wrong arguments! Usage: print PROCESS_ID {pg, mem, stats}, where PROCESS_ID lies within range [0; "
                        << _processCount << "]" << std::endl;
            }
        } else if (args[0] == "info" || args[0] == "i") {
            std::cout << _processCount << " processes are now running" << std::endl;
        } else if (args[0] == "help" || args[0] == "h") {
            std::cout << "Available commands: exit, dump, info, help" << std::endl;
        }  else {
            std::cout << "Wrong command! Available commands: exit, dump, info, help" << std::endl;
        }
    }
}
