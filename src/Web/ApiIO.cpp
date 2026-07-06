#include "Web/ApiIO.h"

#include <ArduinoJson.h>

#include <WebServer.h>
#include "IO/IOManager.h"

namespace
{

const char* iconToString(IOIcon icon)
{
    switch(icon)
    {
        case IOIcon::Light:       return "light";
        case IOIcon::Fan:         return "fan";
        case IOIcon::Door:        return "door";
        case IOIcon::Lock:        return "lock";
        case IOIcon::Motion:      return "motion";
        case IOIcon::Temperature: return "temperature";
        case IOIcon::Water:       return "water";
        case IOIcon::Outlet:      return "outlet";
        case IOIcon::Bell:        return "bell";
        case IOIcon::Curtain:     return "curtain";
        case IOIcon::TV:          return "tv";
        case IOIcon::Generic:     return "generic";
        default:                  return "generic";
    }
}

const char* typeToString(IOType type)
{
    switch(type)
    {
        case IOType::DigitalInput:
            return "input";

        case IOType::DigitalOutput:
            return "output";

        default:
            return "unknown";
    }
}

}

namespace ApiIO
{

void registerRoutes(WebServerService& web)
{
    auto& server = web.server();

    server.on("^\\/api\\/io\\/([0-9]+)$", HTTP_POST, [&server]()
    {
        String uri = server.uri();

        int pos = uri.lastIndexOf('/');

        uint8_t id = uri.substring(pos + 1).toInt();

        IOChannel* ch = ioManager.get(id);

        if(ch == nullptr)
        {
            server.send(404,"text/plain","Not Found");
            return;
        }

        bool state = !ioManager.getState(id);

        ioManager.setState(id,state);

        server.send(200,"text/plain",state ? "ON" : "OFF");
    });

    server.on("/api/io/toggle", HTTP_POST, [&server]()
    {
        if(!server.hasArg("id"))
        {
            server.send(400,"text/plain","Missing id");
            return;
        }

        uint8_t id = server.arg("id").toInt();

        if(id >= ioManager.count())
        {
            server.send(404,"text/plain","Invalid ID");
            return;
        }

        ioManager.toggle(id);

        server.send(200,"application/json","{\"success\":true}");
    });

    server.on("/api/io", HTTP_GET, [&server]()
    {
        JsonDocument doc;

        JsonArray arr = doc.to<JsonArray>();

        for(uint8_t i = 0; i < ioManager.count(); i++)
        {
            IOChannel* ch = ioManager.get(i);

            if(ch == nullptr)
                continue;

            JsonObject obj = arr.add<JsonObject>();

            obj["id"] = i;
            obj["name"] = ch->name;

            obj["state"] = ioManager.getState(i);

            obj["enabled"] = ch->enabled;
            obj["activeLow"] = ch->activeLow;

            obj["favorite"] = ch->favorite;

            obj["type"] = typeToString(ch->type);
            obj["icon"] = iconToString(ch->icon);

            obj["roomId"] = ch->roomId;
            obj["groupId"] = ch->groupId;

            obj["board"] = ch->board;
            obj["address"] = ch->address;
            obj["pin"] = ch->pin;
        }

        String out;

        serializeJson(doc,out);

        server.send(200,"application/json",out);
    });
}

}
