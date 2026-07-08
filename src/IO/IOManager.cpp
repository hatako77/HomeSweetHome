#include "IO/IOManager.h"

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
            // TODO: Load icon from storage.
            ch.icon = IOIcon::Light;
            // TODO: Load channel configuration from storage.
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
    if (id >= channelCount)
        return false;
    if (channels[id].type != IOType::DigitalOutput)
        return false;
    channels[id].state = state;
    bool hwState = state;
    if (channels[id].activeLow)
        hwState = !hwState;
    IIODriver* drv = getDriver(channels[id].address.driverId);
    if (!drv)
        return false;    
    drv->write(
        channels[id].address.device,
        channels[id].address.pin,
        hwState
    );
    return true;
}
bool IOManager::read(uint16_t id) const
{
    if (id >= channelCount)
        return false;
    return channels[id].state;
}

IOChannel* IOManager::getChannel(uint16_t id) 
{
    if (id >= channelCount)
        return nullptr;
    return &channels[id];
}
const IOChannel* IOManager::getChannel(uint16_t id) const
{
    if (id >= channelCount)
        return nullptr;
    return &channels[id];
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
bool IOManager::update(const IOChannel&)
{
    return false;
}

bool IOManager::remove(uint16_t)
{
    return false;
}
