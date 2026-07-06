#ifndef ICON_HELPER_H
#define ICON_HELPER_H

#include "IOConfig.h"
inline bool iconFromValue(uint8_t value, IOIcon& icon)
{
    if (value >= static_cast<uint8_t>(IOIcon::Count))
        return false;

    icon = static_cast<IOIcon>(value);
    return true;
}
inline const char* iconName(IOIcon icon)
{
    switch(icon)
    {
        case IOIcon::Light:        return "lightbulb";
        case IOIcon::Fan:          return "fan";
        case IOIcon::Door:         return "door";
        case IOIcon::Lock:         return "lock";
        case IOIcon::Motion:       return "motion";
        case IOIcon::Temperature:  return "temperature";
        case IOIcon::Water:        return "water";
        case IOIcon::Outlet:       return "outlet";
        case IOIcon::Bell:         return "bell";
        case IOIcon::Curtain:      return "curtain";
        case IOIcon::TV:           return "tv";
        default:                   return "generic";
    }
}

#endif
