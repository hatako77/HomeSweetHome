#ifndef SVG_ICONS_H
#define SVG_ICONS_H

#include <pgmspace.h>

const char SVG_ICONS[] PROGMEM = R"rawliteral(

window.Icons={
light:`
<svg viewBox="0 0 24 24">
<path d="M9 18h6M10 21h4M12 2a7 7 0 0 0-4 12c.7.8 1.4 1.8 1.8 3h4.4c.4-1.2 1.1-2.2 1.8-3A7 7 0 0 0 12 2z"/>
</svg>`,

fan:`
<svg viewBox="0 0 24 24">
<path d="M12 12m-2 0a2 2 0 1 0 4 0a2 2 0 1 0-4 0M12 4c2 0 3 2 2 4l-2 4M20 12c0 2-2 3-4 2l-4-2M12 20c-2 0-3-2-2-4l2-4M4 12c0-2 2-3 4-2l4 2"/>
</svg>`,

door:`
<svg viewBox="0 0 24 24">
<path d="M7 3l10-1v20l-10-1V3zm4 9h1"/>
</svg>`,

lock:`
<svg viewBox="0 0 24 24">
<path d="M7 11V8a5 5 0 0 1 10 0v3M6 11h12v10H6z"/>
</svg>`,

motion:`
<svg viewBox="0 0 24 24">
<path d="M4 12h4M16 12h4M12 4v4M12 16v4M7 7l3 3M14 14l3 3M17 7l-3 3M10 14l-3 3"/>
</svg>`,

temperature:`
<svg viewBox="0 0 24 24">
<path d="M12 3v11a3 3 0 1 1-2 2.8V3a2 2 0 1 1 4 0v11"/>
</svg>`,

water:`
<svg viewBox="0 0 24 24">
<path d="M12 3s5 6 5 10a5 5 0 1 1-10 0c0-4 5-10 5-10z"/>
</svg>`,

outlet:`
<svg viewBox="0 0 24 24">
<path d="M7 3v4M17 3v4M5 7h14v14H5zM9 12h1M14 12h1"/>
</svg>`,

bell:`
<svg viewBox="0 0 24 24">
<path d="M12 3a4 4 0 0 1 4 4v4l2 3H6l2-3V7a4 4 0 0 1 4-4zm-2 15a2 2 0 0 0 4 0"/>
</svg>`,

curtain:`
<svg viewBox="0 0 24 24">
<path d="M4 4h16M6 4v16M10 4v16M14 4v16M18 4v16"/>
</svg>`,

tv:`
<svg viewBox="0 0 24 24">
<path d="M4 6h16v12H4zM9 20h6M10 4l2 2l2-2"/>
</svg>`,

generic:`
<svg viewBox="0 0 24 24">
<path d="M12 2l2.4 5l5.6.8l-4 4l1 5.7L12 15l-5 2.5l1-5.7l-4-4l5.6-.8z"/>
</svg>`
};



)rawliteral";

#endif
