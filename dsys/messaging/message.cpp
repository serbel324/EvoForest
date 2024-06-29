#include "message.h"

MessageWrapper::MessageWrapper(MessageBoxId sender, MessageBoxId recepient, MessageBase::UPtr message)
    : _sender(sender)
    , _recepient(recepient)
    , _message(std::move(message))
{}

MessageBoxId MessageWrapper::GetSender() const {
    return _sender;
}

MessageBoxId MessageWrapper::GetRecepient() const {
    return _recepient;
}

uint64_t MessageWrapper::GetType() const {
    return _message->GetType();
}

const MessageBoxId MessageBoxId::NULL_ADDRESS = {(uint32_t)-1, (uint32_t)-1};
