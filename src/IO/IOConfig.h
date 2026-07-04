#ifndef IO_CONFIG_H
#define IO_CONFIG_H

#include <Arduino.h>

enum class IOMode : uint8_t
{
    Input = 0,
    Output = 1
};

enum class IOIcon : uint8_t
{
    Generic = 0,
    Light,
    Fan,
    Door,
    Window,
    Motion,
    Smoke,
    Water,
    Button
};

struct IOConfig
{
    bool enabled = false;

    String name = "";

    IOMode mode = IOMode::Output;

    IOIcon icon = IOIcon::Generic;

    bool invert = false;

    bool state = false;

    uint8_t pcf = 0;

    uint8_t pin = 0;
};

#endif
