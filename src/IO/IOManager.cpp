#include "IOManager.h"

void IOManager::begin()
{
    for (uint8_t i = 0; i < TOTAL_IO; i++)
    {
        io[i].pcf = i / 8;
        io[i].pin = i % 8;
    }
}

IOConfig& IOManager::get(uint8_t index)
{
    return io[index];
}

bool IOManager::getState(uint8_t index)
{
    return io[index].state;
}

void IOManager::setState(uint8_t index, bool state)
{
    io[index].state = state;
}

void IOManager::toggle(uint8_t index)
{
    io[index].state = !io[index].state;
}
