#ifndef IO_CONFIG_H
#define IO_CONFIG_H

#include <Arduino.h>
#include "IO/IIODriver.h"

enum class IOIcon : uint8_t
{
    Light,
    Fan,
    Door,
    Lock,
    Motion,
    Temperature,
    Water,
    Outlet,
    Bell,
    Curtain,
    TV,
    Generic
};


enum class IOType : uint8_t
{
    DigitalInput = 0,
    DigitalOutput,
    AnalogInput,
    PWMOutput,
    Virtual,
    Disabled
};

struct IOChannel
{
    uint16_t id;
    String name;
    IOIcon icon;
    IOType type;
    bool state;
    bool enabled;
    bool activeLow;
    uint8_t pin;
    uint8_t roomId;
    uint8_t groupId;
    bool favorite;
    IIODriver* driver;
    uint8_t device;
};

#endif
