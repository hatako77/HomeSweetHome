#include "IO/PCF8574Driver.h"

void PCF8574Driver::begin()
{
    Wire.begin(16, 17);

    Serial.println("Scanning...");
    
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
    
        if (Wire.endTransmission() == 0) {
            Serial.printf("Found: 0x%02X\n", addr);
        }
    }
    for (uint8_t i = 0; i < DEVICE_COUNT; i++)
    {
        uint8_t addr = 0x20 + i;

        pcf[i] = new PCF8574(addr);

        connected[i] = pcf[i]->begin();

        Serial.print("PCF ");
        Serial.print(addr, HEX);

        if (connected[i])
            Serial.println(" OK");
        else
            Serial.println(" NOT FOUND");
    }
}

bool PCF8574Driver::isConnected(uint8_t index)
{
    return connected[index];
}

bool PCF8574Driver::read(uint8_t index, uint8_t pin)
{
    if (!connected[index])
        return false;

    return pcf[index]->read(pin);
}

void PCF8574Driver::write(uint8_t index, uint8_t pin, bool state)
{
    if (!connected[index])
        return;

    pcf[index]->write(pin, state);
}
