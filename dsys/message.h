#pragma once

#include <cstdint>
#include <memory>
#include <dsys/paging.h>

enum MessageType : uint64_t {
    FORK = 0,

    SELF_KILL = 1024,
};

using MessageBoxId = uint64_t;

class Message {
public:
    using SPtr = std::shared_ptr<Message>;

public:
    Message(uint64_t type);
    virtual ~Message() = default;

public:
    uint64_t GetType() const;
    MessageBoxId GetSender() const;
    MessageBoxId GetRecepient() const;

    template<class MessageType>
    MessageType* As() {
        return dynamic_cast<MessageType>(this);
    }

private:
    uint64_t _type;
    MessageBoxId _sender; 
    MessageBoxId _recepient;
};
