#ifndef PCF8574_DRIVER_H
#define PCF8574_DRIVER_H
#include "IIODriver.h"
#include <Arduino.h>
#include <Wire.h>
#include <PCF8574.h>

class PCF8574Driver : public IIODriver
{
public:
    static const uint8_t DEVICE_COUNT = 8;

bool begin() override;
void update() override;

bool isConnected(uint8_t device) override;
bool read(uint8_t device, uint8_t pin) override;
bool write(uint8_t device, uint8_t pin, bool state) override;

uint8_t deviceCount() override;

private:
    PCF8574* pcf[DEVICE_COUNT];
    bool connected[DEVICE_COUNT];
    uint8_t deviceCount = 0;
};

#endif
