#pragma once

#include <deque>
#include <dsys/os.h>
#include <dsys/message.h>

class MessageProxy {
public:
    MessageProxy(EvOS* os);
    void Send(Message::SPtr message);

private:
    EvOS* _os;
    std::deque<Message::SPtr> _messages;
};

thread_local MessageProxy* localProxy = nullptr;
