#ifndef IIO_DRIVER_H
#define IIO_DRIVER_H

#include <Arduino.h>

class IIODriver
{
public:
    virtual ~IIODriver() {}

    virtual bool begin() = 0;

    virtual bool read(uint8_t pin) = 0;

    virtual bool write(uint8_t pin, bool state) = 0;

    virtual bool isConnected() = 0;

    virtual uint8_t pinCount() = 0;
};

#endif
