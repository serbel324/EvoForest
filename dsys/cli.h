#pragma once

#include <string>
#include <vector>

#include <dsys/messaging/message.h>

class CLI {
public:
    CLI(uint32_t processCount);
    void Run();

private:
    uint32_t _processCount;
    std::vector<std::string> _GetArgs(const std::string& line) const;
};
