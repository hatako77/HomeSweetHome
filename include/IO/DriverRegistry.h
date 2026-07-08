#pragma once

#include "IO/IIODriver.h"

class DriverRegistry
{
public:
    static bool registerDriver(IIODriver* driver);
    static void registerDrivers();
};
