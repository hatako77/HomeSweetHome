#include "Web/ApiIO.h"

#include <ArduinoJson.h>

#include "Web/WebServerService.h"
#include "IO/IOManager.h"

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
        if (ch == nullptr)
        {
            server.send(404, "text/plain", "Not Found");
            return;
        }    
        bool state = !ioManager.getState(id);    
        ioManager.setState(id, state);    
        server.send(200, "text/plain", state ? "ON" : "OFF");
    });
    
    
    server.on("/api/io/toggle", HTTP_POST, [&server]()
    {
        if (!server.hasArg("id"))
        {
            server.send(400, "text/plain", "Missing id");
            return;
        }    
        uint8_t id = server.arg("id").toInt();    
        if (id >= ioManager.count())
        {
            server.send(404, "text/plain", "Invalid ID");
            return;
        }    
        ioManager.toggle(id);
        server.send(200, "application/json", "{\"success\":true}");
    });




    
    server.on("/api/io", HTTP_GET, [&server]()
    {
        JsonDocument doc;
        JsonArray arr = doc.to<JsonArray>();

        for (uint8_t i = 0; i < ioManager.count(); i++)
        {
            IOChannel* ch = ioManager.get(i);
            JsonObject obj = arr.add<JsonObject>();
            obj["id"] = i;
            obj["name"] = ch->name;
            obj["state"] = ch->state;
            obj["enabled"] = ch->enabled;
            obj["activeLow"] = ch->activeLow;
            obj["icon"] = (uint8_t)ch->icon;
            obj["pcf"] = ch->pcf;
            obj["pin"] = ch->pin;
            obj["type"] =
                ch->type == IOType::DigitalOutput ?
                "output" :
                "input";
        }

        String out;
        serializeJson(doc, out);
        server.send(200, "application/json", out);
    });
}

}
