#include "Web/Notifier.h"

#include "Core/Scene.h"
#include "Web/Message.h"

extern WebSocketService websocket;

void Notifier::reload()
{
    Message msg("system", "reload");

    websocket.send(msg);
}

void Notifier::sceneAdded(const Scene& scene)
{
    Message msg("scene", "added");

    msg.data["id"]       = scene.id;
    msg.data["name"]     = scene.name;
    msg.data["icon"]     = scene.icon;
    msg.data["favorite"] = scene.favorite;
    msg.data["enabled"]  = scene.enabled;

    websocket.send(msg);
}

void Notifier::sceneUpdated(const Scene& scene)
{
    Message msg("scene", "updated");

    msg.data["id"]       = scene.id;
    msg.data["name"]     = scene.name;
    msg.data["icon"]     = scene.icon;
    msg.data["favorite"] = scene.favorite;
    msg.data["enabled"]  = scene.enabled;

    websocket.send(msg);
}

void Notifier::sceneRemoved(uint16_t id)
{
    Message msg("scene", "removed");

    msg.data["id"] = id;

    websocket.send(msg);
}

void Notifier::roomsChanged()
{
    Message msg("room","reload");
    websocket.send(msg);
}

void Notifier::sceneExecuted(uint16_t id)
{
    Message msg("scene", "executed");

    msg.data["id"] = id;

    websocket.send(msg);
}

void Notifier::channelChanged(const IOChannel& channel)
{
    Message msg("channel", "changed");

    msg.data["id"]        = channel.id;
    msg.data["roomId"]    = channel.roomId;
    msg.data["name"]      = channel.name;
    msg.data["state"]     = channel.state;
    msg.data["enabled"]   = channel.enabled;
    msg.data["favorite"]  = channel.favorite;
    msg.data["activeLow"] = channel.activeLow;
    msg.data["type"]      = (uint8_t)channel.type;
    msg.data["icon"]      = (uint8_t)channel.icon;
    msg.data["roomId"]    = channel.roomId;
    msg.data["name"]      = channel.name;
    msg.data["icon"]      = (uint8_t)channel.icon;
    websocket.send(msg);
}

void Notifier::otaStatus(
    const OTAStatus& status,
    const String& current,
    const String& remote)
{
    Message msg("ota", "status");

    msg.data["running"]    = status.running;
    msg.data["finished"]   = status.finished;
    msg.data["success"]    = status.success;

    msg.data["downloaded"] = status.downloaded;
    msg.data["total"]      = status.total;

    msg.data["percent"]    = status.percent;
    msg.data["speed"]      = status.speedKB;
    msg.data["eta"]        = status.eta;

    msg.data["state"]      = status.state;
    msg.data["error"]      = status.error;

    msg.data["current"]    = current;
    msg.data["remote"]     = remote;

    websocket.send(msg);
}
