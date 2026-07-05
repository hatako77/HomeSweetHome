#include "IO/IOManager.h"

IOManager ioManager;

void IOManager::begin()
{
    driver.begin();

    ioCount = 0;

    for(uint8_t p=0;p<8;p++)
    {
        if(!driver.isConnected(p))
            continue;

        for(uint8_t pin=0;pin<8;pin++)
        {
            channels[ioCount].name="IO "+String(ioCount+1);

            channels[ioCount].type=IOType::OUTPUT;

            channels[ioCount].state=false;

            channels[ioCount].activeLow=false;

            channels[ioCount].enabled=true;

            channels[ioCount].pcf=p;

            channels[ioCount].pin=pin;

            ioCount++;
        }
    }

    Serial.print("IO Count : ");
    Serial.println(ioCount);
}

void IOManager::update()
{

}

bool IOManager::setState(uint8_t id,bool state)
{
    if(id>=ioCount)
        return false;

    channels[id].state=state;

    driver.write(
        channels[id].pcf,
        channels[id].pin,
        state
    );

    return true;
}

bool IOManager::getState(uint8_t id)
{
    if(id>=ioCount)
        return false;

    channels[id].state=
        driver.read(
            channels[id].pcf,
            channels[id].pin
        );

    return channels[id].state;
}

IOChannel* IOManager::get(uint8_t id)
{
    if(id>=ioCount)
        return nullptr;

    return &channels[id];
}

uint8_t IOManager::count()
{
    return ioCount;
}
