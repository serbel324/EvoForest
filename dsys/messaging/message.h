#pragma once

#include <cstdint>
#include <memory>
#include <dsys/paging.h>


enum MessageType : uint64_t {
    // from CPU
    MSG_FORK = 0,
    MSG_BLOCK_CONSUMED,
    MSG_ANYCAST,

    // from CLI
    MSG_DUMP = 1 << 20,
    MSG_STOP,
};

struct MessageBoxId {
    uint32_t nodeId;
    uint32_t coreId;

    static const MessageBoxId NULL_ADDRESS;
};

class MessageBase {
public:
    using UPtr = std::unique_ptr<MessageBase>;

public:
    virtual ~MessageBase() = default;
    virtual uint64_t GetType() = 0;
};


template<uint64_t MessageType>
class Message : public MessageBase {
public:
    uint64_t GetType() override {
        return MessageType;
    }
};


class MessageWrapper {
public:
    using SPtr = std::shared_ptr<MessageWrapper>;

public:
    MessageWrapper(MessageBoxId sender, MessageBoxId GetRecepient, MessageBase::UPtr message);

public:
    MessageBoxId GetRecepient() const;
    MessageBoxId GetSender() const;
    uint64_t GetType() const;

    template<class MessageType>
    MessageType* As() {
        return dynamic_cast<MessageType*>(_message.get());
    }

private:
    MessageBoxId _sender; 
    MessageBoxId _recepient;
    std::unique_ptr<MessageBase> _message;
};
