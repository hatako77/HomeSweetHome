#ifndef TYPE_HELPER_H
#define TYPE_HELPER_H

#include "IOConfig.h"

inline const char* typeName(IOType type)
{
    switch(type)
    {
        case IOType::DigitalInput:  return "Digital Input";
        case IOType::DigitalOutput: return "Digital Output";
        case IOType::AnalogInput:   return "Analog Input";
        case IOType::PWMOutput:     return "PWM Output";
        case IOType::Virtual:       return "Virtual";
        case IOType::Disabled:      return "Disabled";
        default:                    return "Unknown";
    }
}

#endif
