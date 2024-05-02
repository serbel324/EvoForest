#pragma once

#include <dsys/message.h>

class MessageFork : public Message{
public:
    MessageFork(std::vector<Page> sourceCode);

    std::vector<Page> sourceCode;
};
