#pragma once

#include <dsys/messaging/message.h>

class MessageBlockConsumed : public Message<MSG_BLOCK_CONSUMED> {
public:
    MessageBlockConsumed(uint32_t creatureId, uint32_t cellsConsumed);

    uint32_t creatureId;
    uint32_t cellsConsumed;
};
