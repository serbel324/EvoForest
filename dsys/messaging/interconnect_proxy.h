#pragma once

#include <mutex>
#include <vector>

#include <dsys/messaging/message_queue.h>

class InterconnectProxy {
public:
    InterconnectProxy(uint32_t localProxyCount);
    
    void Send(MessageBoxId sender, MessageBoxId recepient, MessageBase* message);
    void Send(MessageWrapper::SPtr msg);
    void AnyCast(MessageBoxId sender, MessageBase* message); // Send to random local proxy
    std::vector<MessageWrapper::SPtr> ReceiveAll(MessageBoxId id);

    static InterconnectProxy* GetLocal();

private:
    std::vector<MessageQueue::SPtr> _queues;
};
