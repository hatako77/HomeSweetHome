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
    Generic,
    Count
};


enum class IOType : uint8_t
{
    DigitalInput = 0,
    DigitalOutput,
    AnalogInput,
    PWMOutput,
    Virtual,
    Disabled,
    Count
};
struct IOAddress
{
    uint8_t driverId;
    uint8_t device;
    uint8_t pin;
};
struct IOChannel
{
    uint16_t id = 0;

    bool connected = true;

    String name = "";

    IOIcon icon = IOIcon::Generic;
    IOType type = IOType::DigitalOutput;

    bool state = false;
    bool enabled = true;
    bool activeLow = false;

    uint8_t roomId = 0;
    uint8_t groupId = 0;

    bool favorite = false;

    IOAddress address = {0,0,0};
};
#endif
