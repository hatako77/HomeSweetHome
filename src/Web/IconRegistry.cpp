#include "Web/IconRegistry.h"

static const char LIGHT_ICON[] PROGMEM = R"svg(
<svg xmlns="http://www.w3.org/2000/svg"
viewBox="0 0 24 24"
fill="none"
stroke="currentColor"
stroke-width="2"
stroke-linecap="round"
stroke-linejoin="round">
<path d="M9 18h6"/>
<path d="M10 22h4"/>
<path d="M12 2a7 7 0 0 0-4 13c.7.6 1 1.3 1 2h6c0-.7.3-1.4 1-2A7 7 0 0 0 12 2z"/>
</svg>
)svg";

const SvgIcon Icons[] =
{
    { "light", LIGHT_ICON }
};

const size_t IconCount =
    sizeof(Icons) / sizeof(Icons[0]);

const char* findIcon(const String& name)
{
    for(size_t i = 0; i < IconCount; i++)
    {
        if(name == Icons[i].name)
            return Icons[i].svg;
    }

    return "";
}
