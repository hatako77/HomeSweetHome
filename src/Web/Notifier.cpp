#include "Web/Notifier.h"

extern WebSocketService websocket;

void Notifier::reload()
{
    Message msg("reload");

    websocket.send(msg);
}

void Notifier::channelChanged(const IOChannel& channel)
{
    Message msg("channel");

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
