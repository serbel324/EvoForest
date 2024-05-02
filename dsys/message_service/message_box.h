#pragma once

#include <deque>
#include <dsys/message.h>

class MessageBox {
public:
    Message::SPtr Get();

private:
    std::deque<Message::SPtr> _messages;
};
