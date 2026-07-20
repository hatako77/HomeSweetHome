#ifndef NOTIFIER_H
#define NOTIFIER_H

#include "Web/Message.h"
#include "Web/WebSocketService.h"
#include "IO/IOConfig.h"
#include "OTA/OTAService.h"
#include "Core/Scene.h"

class Notifier
{
public:
    static void reload();
    static void roomsChanged();

    static void channelChanged(const IOChannel& channel);
    static void channelCreated(const IOChannel& channel);
    static void channelUpdated(const IOChannel& channel);
    static void channelDeleted(uint16_t id);

    static void sceneAdded(const Scene& scene);
    static void sceneUpdated(const Scene& scene);    
    static void sceneRemoved(uint16_t id);    
    static void sceneExecuted(uint16_t id);

    static void otaStatus(const OTAStatus& status, const String& current, const String& remote);
};

#endif
