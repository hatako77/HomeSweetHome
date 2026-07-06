#include "IO/IOManager.h"

IOManager ioManager;

void IOManager::begin()
{
    driverCount = 0;    
    drivers[driverCount++] = &pcfDriver;    
    for (uint8_t i = 0; i < driverCount; i++)
    {
        drivers[i]->begin();
    }
    ioCount = 0;

    for (uint8_t d = 0; d < driverCount; d++)
{
    IIODriver* drv = drivers[d];

    for (uint8_t device = 0; device < drv->deviceCount(); device++)
    {
        if (!drv->isConnected(device))
            continue;

        for (uint8_t pin = 0; pin < drv->pinCount(device); pin++)
        {
            IOChannel ch;

            ch.id = ioCount;
            ch.name = "IO " + String(ioCount + 1);
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

            registerChannel(ch);
        }
    }
}

    Serial.print("IO Count: ");
    Serial.println(ioCount);
}

void IOManager::update()
{
    for (uint8_t i = 0; i < driverCount; i++)
    {
        drivers[i]->update();
    }

    for (uint8_t i = 0; i < ioCount; i++)
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
bool IOManager::write(uint8_t id, bool state)
{
    if (id >= ioCount)
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
bool IOManager::read(uint8_t id) const
{
    if (id >= ioCount)
        return false;
    return channels[id].state;
}

IOChannel* IOManager::getChannel(uint8_t id) 
{
    if (id >= ioCount)
        return nullptr;
    return &channels[id];
}
IOChannel* IOManager::getChannel(uint8_t id) const
{
    if (id >= ioCount)
        return nullptr;
    return &channels[id];
}

uint8_t IOManager::count() const
{
    return ioCount;
}

bool IOManager::registerChannel(const IOChannel& channel)
{
    if (ioCount >= MAX_IO)
        return false;

    channels[ioCount++] = channel;

    return true;
}

bool IOManager::toggle(uint8_t id)
{
    return write(id, !read(id));
}

bool IOManager::on(uint8_t id)
{
    return write(id, true);
}

bool IOManager::off(uint8_t id)
{
    return write(id, false);
}
IIODriver* IOManager::getDriver(uint8_t driverId)
{
    if (driverId >= driverCount)
        return nullptr;

    return drivers[driverId];
}
