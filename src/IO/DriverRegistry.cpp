#include "IO/DriverRegistry.h"
#include "IO/IOManager.h"
#include "IO/PCF8574Driver.h"

static PCF8574Driver pcfDriver;

bool DriverRegistry::registerDriver(IIODriver* driver)
{
    return ioManager.registerDriver(driver);
}

void DriverRegistry::registerDrivers()
{
    registerDriver(&pcfDriver);
}
