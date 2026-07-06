#ifndef IIO_DRIVER_H
#define IIO_DRIVER_H

#include <Arduino.h>

class IIODriver
{
public:
    virtual ~IIODriver() {}
    virtual bool begin() = 0;
    virtual void update() = 0;
    virtual bool read(uint8_t device, uint8_t pin) = 0;
    virtual bool write(uint8_t device, uint8_t pin, bool state) = 0;
    virtual bool isConnected(uint8_t device) = 0;
    virtual uint8_t deviceCount() = 0;
    virtual uint8_t pinCount(uint8_t device) = 0;
};

#endif
