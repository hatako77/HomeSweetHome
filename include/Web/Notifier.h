#ifndef NOTIFIER_H
#define NOTIFIER_H

#include "Web/Message.h"
#include "Web/WebSocketService.h"
#include "Core/IOChannel.h"
#include "OTA/OTAService.h"

class Notifier
{
public:

    static void reload();

    static void channelChanged(const IOChannel& channel);

    static void otaStatus(
        const OTAStatus& status,
        const String& current,
        const String& remote
    );
};

#endif
