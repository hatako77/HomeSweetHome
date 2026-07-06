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
    for (uint8_t p = 0; p < PCF8574Driver::DEVICE_COUNT; p++)
    {
        if (!pcfDriver.isConnected(p))
            continue;
        for (uint8_t pin = 0; pin < 8; pin++)
        {
            IOChannel ch;            
            ch.id = ioCount;
            ch.name = "IO " + String(ioCount + 1);
            ch.icon = IOIcon::Light;
            ch.type = IOType::DigitalOutput;
            ch.state = false;
            ch.enabled = true;
            ch.activeLow = false;
            
            ch.driver = drivers[0];
            ch.device = p;
            ch.pin = pin;
            
            ch.roomId = 0;
            ch.groupId = 0;
            ch.favorite = false;
            
            registerChannel(ch);
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
        bool state = channels[i].driver->read(
            channels[i].device,
            channels[i].pin
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
    channels[id].driver->write(
        channels[id].device,
        channels[id].pin,
        hwState
    );
    return true;
}
bool IOManager::read(uint8_t id)
{
    if (id >= ioCount)
        return false;
    return channels[id].state;
}

IOChannel* IOManager::get(uint8_t id)
{
    if (id >= ioCount)
        return nullptr;
    return &channels[id];
}

uint8_t IOManager::count()
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

PCF8574Driver& IOManager::getDriver()
{
    return pcfDriver;
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
