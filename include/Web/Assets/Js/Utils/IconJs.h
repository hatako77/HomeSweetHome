#ifndef ICON_JS_H
#define ICON_JS_H

#include <pgmspace.h>

const char ICON_JS[] PROGMEM = R"rawliteral(

function getIcon(id)
{
    switch(Number(id))
    {
    case 0:return Icons.light;
    case 1:return Icons.fan;
    case 2:return Icons.door;
    case 3:return Icons.lock;
    case 4:return Icons.motion;
    case 5:return Icons.temperature;
    case 6:return Icons.water;
    case 7:return Icons.outlet;
    case 8:return Icons.bell;
    case 9:return Icons.curtain;
    case 10:return Icons.tv;
    default:return Icons.generic;
    }
}

function icon(id,size=22)
{
return `
<span class="svg-icon" style="width:${size}px;height:${size}px">
${getIcon(id)}
</span>
`;
}

)rawliteral";

#endif
