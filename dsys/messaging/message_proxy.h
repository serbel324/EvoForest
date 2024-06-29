#pragma once

#include <deque>
#include <dsys/messaging/message_queue.h>
#include <dsys/messaging/message.h>

class MessageProxy {
public:
    MessageProxy(MessageBoxId id);

    void Send(MessageBoxId sender, MessageBoxId recepient, MessageBase* message);
    void Send(MessageWrapper::SPtr message);
    MessageWrapper::SPtr Receive();

    MessageBoxId GetId() const;

    static MessageProxy* GetLocal();

private:
    LocalMessageQueue _queue;
    MessageBoxId _id;
};
