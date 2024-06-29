#include "message_proxy.h"
#include <dsys/messaging/interconnect_proxy.h>

#include <iostream>

thread_local MessageProxy* localMessageProxy = nullptr;

MessageProxy::MessageProxy(MessageBoxId id)
    : _id(id)
{
    localMessageProxy = this;
}

void MessageProxy::Send(MessageBoxId sender, MessageBoxId recepient, MessageBase* message) {
    Send(std::make_shared<MessageWrapper>(sender, recepient, MessageBase::UPtr(message)));
}

void MessageProxy::Send(MessageWrapper::SPtr message) {
    _queue.Push(std::move(message));
}

MessageWrapper::SPtr MessageProxy::Receive() {
    std::vector<MessageWrapper::SPtr> messages = InterconnectProxy::GetLocal()->ReceiveAll(_id);
    for (MessageWrapper::SPtr& msg : messages) {
        _queue.Push(std::move(msg));
    }

    return _queue.PullOne();
}

MessageProxy* MessageProxy::GetLocal() {
    return localMessageProxy;
}

MessageBoxId MessageProxy::GetId() const {
    return _id;
}
