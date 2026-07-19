#ifndef ICON_REGISTRY_H
#define ICON_REGISTRY_H

#include <Arduino.h>
#include <pgmspace.h>

struct SvgIcon
{
    const char* name;
    const char* svg;
};

extern const SvgIcon Icons[];
extern const size_t IconCount;

const char* findIcon(const String& name);

#endif
