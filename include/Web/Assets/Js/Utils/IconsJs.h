#ifndef ICONS_JS_H
#define ICONS_JS_H

#include <pgmspace.h>

const char ICONS_JS[] PROGMEM = R"rawliteral(

const Icons =
{

light:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M9 21h6v-1H9v1zm3-20a7 7 0 0 0-4 12.7V17h8v-3.3A7 7 0 0 0 12 1z"/>
</svg>`,

fan:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M12 11a1 1 0 100 2 1 1 0 000-2zm0-9c2 0 3 2 2 5l-1 3a4 4 0 015-1c3 1 4 5 1 6l-3 1a4 4 0 01-2 5c-3 1-5-2-4-5l1-3a4 4 0 01-5 1c-3-1-4-5-1-6l3-1a4 4 0 012-5z"/>
</svg>`,

door:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M6 2l10-1v22H6V2zm7 10a1 1 0 100 2"/>
</svg>`,

lock:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M17 9V7a5 5 0 00-10 0v2H5v13h14V9h-2zm-8-2a3 3 0 116 0v2H9V7z"/>
</svg>`,

outlet:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M7 3h10a3 3 0 013 3v12a3 3 0 01-3 3H7a3 3 0 01-3-3V6a3 3 0 013-3zm2 6v3m6-3v3"/>
</svg>`,

tv:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M4 5h16v12H4V5zm5 14h6"/>
</svg>`,

curtain:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M4 2h16v20H4V2zm4 0v20m4-20v20m4-20v20"/>
</svg>`,

bell:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M12 22a2 2 0 002-2h-4a2 2 0 002 2zm6-6V11a6 6 0 10-12 0v5L4 18v1h16v-1l-2-2z"/>
</svg>`,

water:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M12 2C8 7 6 10 6 13a6 6 0 0012 0c0-3-2-6-6-11z"/>
</svg>`,

temperature:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M14 14.8V5a2 2 0 10-4 0v9.8a4 4 0 104 0z"/>
</svg>`,

motion:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M12 4a8 8 0 018 8h-2a6 6 0 00-6-6V4zm0 4a4 4 0 014 4h-2a2 2 0 00-2-2V8zm0 4a1 1 0 110 2 1 1 0 010-2z"/>
</svg>`,

generic:`
<svg viewBox="0 0 24 24" fill="currentColor">
<circle cx="12" cy="12" r="8"/>
</svg>`

};

)rawliteral";

#endif
