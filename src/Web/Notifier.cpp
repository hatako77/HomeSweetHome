#include "Web/Notifier.h"
#include "Core/Scene.h"


extern WebSocketService websocket;

void Notifier::reload()
{
    Message msg("system","reload");
    websocket.send(msg);
}
void Notifier::sceneAdded(const Scene& scene)
{
    Message msg("scene","added");

    msg.json["id"] = scene.id;
    msg.json["name"] = scene.name;
    msg.json["icon"] = scene.icon;
    msg.json["favorite"] = scene.favorite;
    msg.json["enabled"] = scene.enabled;

    websocket.send(msg);
}

void Notifier::sceneUpdated(const Scene& scene)
{
    Message msg("scene","updated");

    msg.json["id"] = scene.id;
    msg.json["name"] = scene.name;
    msg.json["icon"] = scene.icon;
    msg.json["favorite"] = scene.favorite;
    msg.json["enabled"] = scene.enabled;

    websocket.send(msg);
}

void Notifier::sceneRemoved(uint16_t id)
{
    Message msg("scene","removed");

    msg.json["id"] = id;

    websocket.send(msg);
}

void Notifier::sceneExecuted(uint16_t id)
{
    Message msg("scene","executed");

    msg.json["id"] = id;

    websocket.send(msg);
}

void Notifier::channelChanged(const IOChannel& channel)
{
    Message msg("channel","changed");

    msg.json["id"] = channel.id;
    msg.json["state"] = channel.state;

    websocket.send(msg);
}

void Notifier::otaStatus(
    const OTAStatus& status,
    const String& current,
    const String& remote)
{
    Message msg("ota");

    msg.json["running"] = status.running;
    msg.json["finished"] = status.finished;
    msg.json["success"] = status.success;

    msg.json["downloaded"] = status.downloaded;
    msg.json["total"] = status.total;

    msg.json["percent"] = status.percent;
    msg.json["speed"] = status.speedKB;
    msg.json["eta"] = status.eta;

    msg.json["state"] = status.state;
    msg.json["error"] = status.error;

    msg.json["current"] = current;
    msg.json["remote"] = remote;

    websocket.send(msg);
}
