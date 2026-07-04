#ifndef PCF8574_DRIVER_H
#define PCF8574_DRIVER_H

#include <Arduino.h>
#include <Wire.h>
#include <PCF8574.h>

class PCF8574Driver
{
public:
    static const uint8_t DEVICE_COUNT = 8;

    void begin();

    bool isConnected(uint8_t index);

    bool read(uint8_t index, uint8_t pin);

    void write(uint8_t index, uint8_t pin, bool state);

private:
    PCF8574* pcf[DEVICE_COUNT];

    bool connected[DEVICE_COUNT];
};

#endif
