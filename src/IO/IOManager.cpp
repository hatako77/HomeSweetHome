#include "IO/IOManager.h"
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "IO/TypeHelper.h"
#include "IO/IconHelper.h"
IOManager ioManager;

void IOManager::begin()
{
    driverCount = 0;    
    drivers[driverCount++] = &pcfDriver;    
    for (uint16_t i = 0; i < driverCount; i++)
    {
        drivers[i]->begin();
    }
    channelCount = 0;
    nextId = 1;

    for (uint16_t d = 0; d < driverCount; d++)
    {
        IIODriver* drv = drivers[d];
        for (uint16_t device = 0; device < drv->deviceCount(); device++)
        {
        if (!drv->isConnected(device))
            continue;
            for (uint16_t pin = 0; pin < drv->pinCount(device); pin++)
            {
                IOChannel ch;    
                ch.id = channelCount;
                ch.name = "IO " + String(channelCount  + 1);
                ch.icon = IOIcon::Light;
                ch.type = IOType::DigitalOutput;
                ch.state = false;
                ch.enabled = true;
                ch.activeLow = false;    
                ch.address.driverId = d;
                ch.address.device = device;
                ch.address.pin = pin;    
                ch.roomId = 0;
                ch.groupId = 0;
                ch.favorite = false;    
                add(ch);
            }
        }
    }    
    Serial.print("channelCount: ");
    Serial.println(channelCount);
    load();
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

bool IOManager::add(const IOChannel& channel)
{
    if (channelCount >= MAX_IO)
        return false;

    IOChannel ch = channel;

    if (ch.id == 0)
        ch.id = nextId++;

    channels[channelCount++] = ch;

    return true;
}

bool IOManager::toggle(uint16_t id)
{
    return write(id, !read(id));
}

bool IOManager::on(uint16_t id)
{
    return write(id, true);
}

bool IOManager::off(uint16_t id)
{
    return write(id, false);
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
            channels[i] = channel;
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

    for (uint8_t i = 0; i < count(); i++)
    {
        auto* ch = getChannel(i);
        JsonObject o = arr.add<JsonObject>();
        o["id"] = ch->id;        
        o["name"] = ch->name;        
        o["enabled"] = ch->enabled;
        o["favorite"] = ch->favorite;        
        o["activeLow"] = ch->activeLow;        
        o["type"] = static_cast<uint8_t>(ch->type);
        o["icon"] = static_cast<uint8_t>(ch->icon);        
        o["roomId"] = ch->roomId;
        o["groupId"] = ch->groupId;        
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
    if (!LittleFS.exists("/io.json"))
        return false;

    File f = LittleFS.open("/io.json", "r");

    if (!f)
        return false;

    JsonDocument doc;

    deserializeJson(doc, f);

    JsonArray arr = doc.as<JsonArray>();

    uint8_t i = 0;

    for (JsonObject o : arr)
    {
        if (i >= count())
            break;

        auto* ch = getChannel(i);

        ch->name = o["name"] | ch->name;
        ch->enabled = o["enabled"] | true;
        ch->activeLow = o["activeLow"] | false;
        IOType type;
        if (typeFromValue(o["type"] | 1, type))
            ch->type = type;
        IOIcon icon;        
        if (iconFromValue(o["icon"] | 0, icon))
            ch->icon = icon;
        ch->favorite = o["favorite"] | false;
        ch->roomId = o["roomId"] | 0;
        ch->groupId = o["groupId"] | 0;
        ch->favorite = o["favorite"] | false;
        ch->address.driverId = o["driverId"] | 0;
        ch->address.device = o["device"] | 0;
        ch->address.pin = o["pin"] | 0;
        i++;
    }

    f.close();

    return true;
}
