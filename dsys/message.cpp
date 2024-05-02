#include <dsys/message.h>

Message::Message(uint64_t type)
    : _type(type)
{}

uint64_t Message::GetType() const {
    return _type;
}

uint64_t Message::GetSender() const {
    return _sender;
}

uint64_t Message::GetRecepient() const {
    return _recepient;
}
