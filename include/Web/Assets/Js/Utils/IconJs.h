#ifndef ICON_JS_H
#define ICON_JS_H

#include <pgmspace.h>

const char ICONS_JS[] PROGMEM = R"rawliteral(

const iconMap =
{
    0: "light",
    1: "fan",
    2: "door",
    3: "lock",
    4: "motion",
    5: "temperature",
    6: "water",
    7: "outlet",
    8: "bell",
    9: "curtain",
    10: "tv",
    settings:"settings",
    ota:"ota",
    scenes:"scenes",
    schedules:"schedules",
    rooms:"rooms",
    channels:"channels",
    sensors: "sensors",  
    plus: "plus",
    edit: "edit",
    trash: "trash",
    add:"add",
    plus:"plus",
    create:"create",
    delete:"delete",
    edit:"edit",
    generic: "generic"
};

function getIcon(id)
{
    const key = iconMap[id] ?? "generic";
    return Icons[key] ?? Icons.generic;
}



)rawliteral";

#endif
