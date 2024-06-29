#pragma once

#include <dsys/messaging/message.h>

class MessageDump : public Message<MSG_DUMP> {
public:
    enum class EDumpedInfo {
        MEMORY = 0,
        PAGES,
        STATS,
    };

    MessageDump(EDumpedInfo dumpedInfo, bool toFile);

    EDumpedInfo dumpedInfo;
    bool toFile;
};
