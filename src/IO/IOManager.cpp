#include "IO/IOManager.h"
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "IO/DriverRegistry.h"
#include "IO/TypeHelper.h"
#include "IO/IconHelper.h"
#include "Web/Notifier.h"


IOManager ioManager;

void IOManager::begin()
{
    driverCount = 0;    
    DriverRegistry::registerDrivers();
    channelCount = 0;
    nextId = 1;

    bool loaded = load();
    
    Serial.print("Load: ");
    Serial.println(loaded ? "OK" : "NEW");
    
    Serial.print("Channel Count: ");
    Serial.println(channelCount);
    
    for (uint16_t i = 0; i < channelCount; i++)
    {
        Serial.printf(
            "%2d | %-12s | Room:%d | Driver:%d Device:%d Pin:%d\n",
            channels[i].id,
            channels[i].name.c_str(),
            channels[i].roomId,
            channels[i].address.driverId,
            channels[i].address.device,
            channels[i].address.pin
        );
    }    
    for (uint16_t i = 0; i < channelCount; i++)
    {
        Serial.print(channels[i].id);
        Serial.print(" : ");
        Serial.println(channels[i].name);
    }
}

void IOManager::update()
{
    for (uint16_t i = 0; i < driverCount; i++)
    {
        drivers[i]->update();
    }

    for (uint16_t i = 0; i < channelCount; i++)
    {
        IIODriver* drv = getDriver(channels[i].address.driverId);
        
        if (!drv)
            continue;
        channels[i].connected = drv->isConnected(channels[i].address.device);   
        bool state = drv->read(
            channels[i].address.device,
            channels[i].address.pin
        );
        if (channels[i].activeLow)
            state = !state;

        channels[i].state = state;
    }

}
bool IOManager::write(uint16_t id, bool state)
{

    IOChannel* ch = getChannel(id);
    Serial.printf(
        "WRITE -> id=%d state=%d\n",
        ch->id,
        ch->state
    );
    if (!ch)
        return false;    
    if (ch->type != IOType::DigitalOutput)
        return false;    
    ch->state = state;    
    bool hwState = state;    
    if (ch->activeLow)
        hwState = !hwState;    
    IIODriver* drv = getDriver(ch->address.driverId);    
    if (!drv)
        return false;    
    drv->write(
        ch->address.device,
        ch->address.pin,
        hwState);  
    Notifier::channelChanged(*ch);
    return true;
}


bool IOManager::read(uint16_t id) const
{
const IOChannel* ch = getChannel(id);
    if (!ch)
        return false;
    return ch->state;}
    IOChannel* IOManager::getChannel(uint16_t id)
    {
        for (uint16_t i = 0; i < channelCount; i++)
        {
            if (channels[i].id == id)
                return &channels[i];
        }
    return nullptr;
}
const IOChannel* IOManager::getChannel(uint16_t id) const
{
    for (uint16_t i = 0; i < channelCount; i++)
    {
        if (channels[i].id == id)
            return &channels[i];
    }
    return nullptr;
}

uint16_t IOManager::count() const
{
    return channelCount;
}

IOChannel* IOManager::add(const IOChannel& channel)
{
    if(channelCount >= MAX_IO)
        return nullptr;

    IOChannel ch = channel;

    if(ch.id == 0)
        ch.id = nextId++;

    channels[channelCount] = ch;

    return &channels[channelCount++];
}


IIODriver* IOManager::getDriver(uint16_t driverId)
{
    if (driverId >= driverCount)
        return nullptr;

    return drivers[driverId];
}

uint16_t IOManager::countByRoom(uint16_t roomId) const
{
        uint16_t count = 0;
        for (uint16_t i = 0; i < channelCount; i++)
    {
        if (channels[i].roomId == roomId)
            count++;
    }
    return count;
}

bool IOManager::remove(uint16_t id)
{
    for (uint16_t i = 0; i < channelCount; i++)
    {
        if (channels[i].id == id)
        {
            for (uint16_t j = i; j < channelCount - 1; j++)
            {
                channels[j] = channels[j + 1];
            }

            channelCount--;
            return true;
        }
    }

    return false;
}
bool IOManager::update(const IOChannel& channel)
{
    for (uint16_t i = 0; i < channelCount; i++)
    {
        if (channels[i].id == channel.id)
        {
            channels[i].name       = channel.name;
            channels[i].icon       = channel.icon;
            channels[i].type       = channel.type;
            channels[i].enabled    = channel.enabled;
            channels[i].favorite   = channel.favorite;
            channels[i].activeLow  = channel.activeLow;
            channels[i].roomId     = channel.roomId;
            if(channels[i].state != channel.state)
            {
                write(channel.id,channel.state);
            }
            return true;
        }
    }
    return false;
}

bool IOManager::save() const
{
    File f = LittleFS.open("/io.json", "w");

    if (!f)
        return false;

    JsonDocument doc;

    JsonArray arr = doc.to<JsonArray>();

    for (uint16_t i = 0; i < channelCount; i++)
    {
        const auto* ch = &channels[i];
        JsonObject o = arr.add<JsonObject>();
        o["id"] = ch->id;        
        o["name"] = ch->name;        
        o["enabled"] = ch->enabled;
        o["favorite"] = ch->favorite;        
        o["activeLow"] = ch->activeLow;        
        o["type"] = static_cast<uint8_t>(ch->type);
        o["icon"] = static_cast<uint8_t>(ch->icon);        
        o["roomId"] = ch->roomId;
        o["driverId"] = ch->address.driverId;
        o["device"] = ch->address.device;
        o["pin"] = ch->address.pin;
    }

    serializeJson(doc, f);

    f.close();

    return true;
}

bool IOManager::load()
{
    if (!LittleFS.exists("/io.json")) return false;
    File f = LittleFS.open("/io.json", "r");
    if (!f) return false;
    JsonDocument doc;
    deserializeJson(doc, f);
    JsonArray arr = doc.as<JsonArray>();
    channelCount = 0;
    for(JsonObject o : arr)
    {
        if(channelCount >= MAX_IO) break;    
        IOChannel& ch = channels[channelCount++];    
        ch.id = o["id"] | nextId;    
        if(ch.id >= nextId) nextId = ch.id + 1;    
        ch.name = o["name"] | "";
        ch.enabled = o["enabled"] | true;
        ch.activeLow = o["activeLow"] | false;
        ch.favorite = o["favorite"] | false;
        ch.roomId = o["roomId"] | 0;    
        typeFromValue(o["type"] | 1, ch.type);
        iconFromValue(o["icon"] | 0, ch.icon);    
        ch.address.driverId = o["driverId"] | 0;
        ch.address.device   = o["device"]   | 0;
        ch.address.pin      = o["pin"]      | 0;
    }
    f.close();
    return true;
}

bool IOManager::registerDriver(IIODriver* driver)
{
    if (driverCount >= MAX_DRIVERS)
        return false;

    drivers[driverCount++] = driver;
    driver->begin();
    return true;
}




IOChannel* IOManager::getAt(uint16_t index)
{
    if (index >= channelCount)
        return nullptr;

    return &channels[index];
}

const IOChannel* IOManager::getAt(uint16_t index) const
{
    if (index >= channelCount)
        return nullptr;

    return &channels[index];
}


