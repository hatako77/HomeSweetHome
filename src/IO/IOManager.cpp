#include "IO/IOManager.h"

IOManager ioManager;

void IOManager::begin()
{
    driver.begin();

    ioCount = 0;

    for (uint8_t p = 0; p < PCF8574Driver::DEVICE_COUNT; p++)
    {
        if (!driver.isConnected(p))
            continue;

        for (uint8_t pin = 0; pin < 8; pin++)
        {
            channels[ioCount].name = "IO " + String(ioCount + 1);
            channels[ioCount].type = IOType::DigitalOutput;
            channels[ioCount].state = false;
            channels[ioCount].activeLow = false;
            channels[ioCount].enabled = true;
            channels[ioCount].id = ioCount;
            channels[ioCount].icon = IOIcon::Light;
            channels[ioCount].favorite = false;
            channels[ioCount].roomId = 0;
            channels[ioCount].groupId = 0;
            channels[ioCount].board = p;
            channels[ioCount].pin = pin;
            ioCount++;
        }
    }

    Serial.print("IO Count: ");
    Serial.println(ioCount);
}

void IOManager::update()
{
    driver.update();

    for (uint8_t i = 0; i < ioCount; i++)
    {
        bool state = driver.read(
            channels[i].board,
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
    driver.write(
        channels[id].board,
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

PCF8574Driver& IOManager::getDriver()
{
    return driver;
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
