#ifndef IO_CONFIG_H
#define IO_CONFIG_H

#include <Arduino.h>

enum class IOType : uint8_t
{
    DigitalInput,
    DigitalOutput
};

struct IOChannel
{
    String name;

    String icon = "lightbulb";

    IOType type = IOType::DigitalOutput;

    bool state = false;

    bool enabled = true;

    bool activeLow = false;

    uint8_t pcf = 0;

    uint8_t pin = 0;
};

#endif
