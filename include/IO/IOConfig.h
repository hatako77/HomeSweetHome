#ifndef IO_CONFIG_H
#define IO_CONFIG_H

#include <Arduino.h>

enum class IOType : uint8_t
{
    INPUT,
    OUTPUT
};

struct IOChannel
{
    String name;

    IOType type;

    bool state;

    bool activeLow;

    bool enabled;

    uint8_t pcf;

    uint8_t pin;
};

#endif
