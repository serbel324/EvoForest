#pragma once

#include <dsys/messaging/message.h>

class MessageFork : public Message<MSG_FORK> {
public:
    MessageFork(std::vector<Page> sourceCode, uint64_t ticksToLive, uint32_t color);

    std::vector<Page> sourceCode;
    uint32_t color;
    uint64_t ticksToLive;
};
