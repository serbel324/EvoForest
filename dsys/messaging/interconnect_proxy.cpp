#include "interconnect_proxy.h"

#include <library/ext_math.h>

#include <cassert>

InterconnectProxy* localInterconnectProxy;

InterconnectProxy::InterconnectProxy(uint32_t localProxyCount) {
    for (uint32_t i = 0; i < localProxyCount; ++i) {
        _queues.push_back(std::make_shared<ProtectedMessageQueue>());
    }
    localInterconnectProxy = this;
}

void InterconnectProxy::Send(MessageBoxId sender, MessageBoxId recepient, MessageBase* message) {
    Send(std::make_shared<MessageWrapper>(sender, recepient, MessageBase::UPtr(message)));
}

void InterconnectProxy::AnyCast(MessageBoxId sender, MessageBase* message) {
    Send(std::make_shared<MessageWrapper>(sender, MessageBoxId{0, (uint32_t)ExtMath::RandomInt(0, _queues.size())}, MessageBase::UPtr(message)));
}

void InterconnectProxy::Send(MessageWrapper::SPtr msg) {
    assert(msg);
    uint32_t localId = msg->GetRecepient().coreId;
    if (localId < _queues.size()) {
        _queues[localId]->Push(std::move(msg));
    }
}

std::vector<MessageWrapper::SPtr> InterconnectProxy::ReceiveAll(MessageBoxId id) {
    if (id.coreId < _queues.size()) {
        return _queues[id.coreId]->PullAll();
    }
    return {};
}

InterconnectProxy* InterconnectProxy::GetLocal() {
    return localInterconnectProxy;
}
