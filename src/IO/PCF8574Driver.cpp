#include "IO/PCF8574Driver.h"

bool PCF8574Driver::begin()
{
    Wire.begin(16, 17);

    deviceCountValue = 0;

    Serial.println("Scanning I2C...");

    for (uint8_t addr = 1; addr < 127; addr++)
    {
        Wire.beginTransmission(addr);

        if (Wire.endTransmission() == 0)
            Serial.printf("Found: 0x%02X\n", addr);
    }

    for (uint8_t i = 0; i < DEVICE_COUNT; i++)
    {
        uint8_t addr = 0x20 + i;

        pcf[i] = new PCF8574(addr);

        connected[i] = pcf[i]->begin();

        Serial.printf("PCF %02X ", addr);

        if (connected[i])
        {
            Serial.println("OK");
            deviceCountValue++;
            pcf[i]->write8(0xFF);
        }
        else
        {
            Serial.println("NOT FOUND");
        }
    }

    Serial.print("PCF Found: ");
    Serial.println(deviceCountValue);
    return true;
}

void PCF8574Driver::update()
{
    for (uint8_t i = 0; i < DEVICE_COUNT; i++)
    {
        if (!connected[i])
            continue;

        pcf[i]->read8();
    }
}

bool PCF8574Driver::isConnected(uint8_t index)
{
    if (index >= DEVICE_COUNT)
        return false;

    return connected[index];
}

uint8_t PCF8574Driver::deviceCount()
{
    return deviceCountValue;
}

bool PCF8574Driver::read(uint8_t index, uint8_t pin)
{
    if (!connected[index])
        return false;

    return pcf[index]->read(pin);
}

bool  PCF8574Driver::write(uint8_t index, uint8_t pin, bool state)
{
    if (!connected[index])
        return false;

    pcf[index]->write(pin, state);
    return true;
}
uint8_t PCF8574Driver::pinCount(uint8_t device)
{
    if (!isConnected(device))
        return 0;

    return 8;
}
