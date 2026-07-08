#include "IO/DriverRegistry.h"
#include "IO/IOManager.h"
#include "IO/PCF8574Driver.h"

static PCF8574Driver pcfDriver;

void DriverRegistry::registerDrivers()
{
    ioManager.registerDriver(&pcfDriver);
}
