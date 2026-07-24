#ifndef SVG_ICONS_H
#define SVG_ICONS_H

#include <pgmspace.h>

const char SVG_ICONS[] PROGMEM = R"rawliteral(

window.Icons = {

plus: `
<svg viewBox="0 0 24 24" fill="none"
stroke="currentColor" stroke-width="2.2"
stroke-linecap="round" stroke-linejoin="round">
    <path d="M12 5v14"/>
    <path d="M5 12h14"/>
</svg>`,

edit: `
<svg viewBox="0 0 24 24" fill="none"
stroke="currentColor" stroke-width="2"
stroke-linecap="round" stroke-linejoin="round">
    <path d="M4 20l4.5-1L19 8.5 15.5 5 5 15.5 4 20z"/>
    <path d="M14.5 5.5L18 9"/>
</svg>`,

delete: `
<svg viewBox="0 0 24 24" fill="none"
stroke="currentColor" stroke-width="2"
stroke-linecap="round" stroke-linejoin="round">
    <path d="M4 7h16"/>
    <path d="M9 4h6"/>
    <path d="M7 7l1 13h8l1-13"/>
    <path d="M10 11v6"/>
    <path d="M14 11v6"/>
</svg>`,

create: `
<svg viewBox="0 0 24 24" fill="none"
stroke="currentColor" stroke-width="2"
stroke-linecap="round" stroke-linejoin="round">
    <circle cx="12" cy="12" r="9"/>
    <path d="M12 8v8"/>
    <path d="M8 12h8"/>
</svg>`,

add: `
<svg viewBox="0 0 24 24" fill="none"
stroke="currentColor" stroke-width="2"
stroke-linecap="round" stroke-linejoin="round">
    <rect x="3" y="5" width="14" height="16" rx="2"/>
    <path d="M20 8v8"/>
    <path d="M16 12h8"/>
</svg>`,

channels: `
<svg viewBox="0 0 24 24" fill="none"
stroke="currentColor" stroke-width="2"
stroke-linecap="round" stroke-linejoin="round">
<circle cx="6" cy="6" r="2"/>
<circle cx="18" cy="6" r="2"/>
<circle cx="6" cy="18" r="2"/>
<circle cx="18" cy="18" r="2"/>
<path d="M8 6h8"/>
<path d="M6 8v8"/>
<path d="M18 8v8"/>
<path d="M8 18h8"/>
</svg>`,

scenes: `
<svg viewBox="0 0 24 24" fill="none"
stroke="currentColor" stroke-width="2"
stroke-linecap="round" stroke-linejoin="round">
<path d="M4 7h16"/>
<path d="M4 12h16"/>
<path d="M4 17h16"/>
<circle cx="7" cy="7" r="1"/>
<circle cx="12" cy="12" r="1"/>
<circle cx="17" cy="17" r="1"/>
</svg>`,

schedules: `
<svg viewBox="0 0 24 24" fill="none"
stroke="currentColor" stroke-width="2"
stroke-linecap="round" stroke-linejoin="round">
<rect x="4" y="5" width="16" height="15" rx="2"/>
<path d="M8 3v4"/>
<path d="M16 3v4"/>
<path d="M4 10h16"/>
<path d="M12 13v3"/>
<path d="M12 13l2 1"/>
</svg>`,

sensors: `
<svg viewBox="0 0 24 24" fill="none"
stroke="currentColor" stroke-width="2"
stroke-linecap="round" stroke-linejoin="round">
<circle cx="12" cy="12" r="3"/>
<path d="M12 2v3"/>
<path d="M12 19v3"/>
<path d="M2 12h3"/>
<path d="M19 12h3"/>
<path d="M5 5l2 2"/>
<path d="M17 17l2 2"/>
<path d="M17 7l2-2"/>
<path d="M5 19l2-2"/>
</svg>`,

ota: `
<svg viewBox="0 0 24 24" fill="none"
stroke="currentColor" stroke-width="2"
stroke-linecap="round" stroke-linejoin="round">
<path d="M12 3v11"/>
<path d="M8 10l4 4l4-4"/>
<path d="M5 20h14"/>
</svg>`,

settings: `
<svg viewBox="0 0 24 24" fill="none"
stroke="currentColor" stroke-width="2"
stroke-linecap="round" stroke-linejoin="round">
<circle cx="12" cy="12" r="3"/>
<path d="M19 12a7 7 0 0 0-.1-1l2-1.5l-2-3.5l-2.4 1a7 7 0 0 0-1.8-1L14 2h-4l-.7 3a7 7 0 0 0-1.8 1l-2.4-1l-2 3.5L5.1 11a7 7 0 0 0 0 2l-2 1.5l2 3.5l2.4-1a7 7 0 0 0 1.8 1l.7 3h4l.7-3a7 7 0 0 0 1.8-1l2.4 1l2-3.5l-2-1.5c.1-.3.1-.7.1-1z"/>
</svg>`,


rooms: `
<svg viewBox="0 0 24 24" fill="none"
stroke="currentColor" stroke-width="2"
stroke-linecap="round" stroke-linejoin="round">
<path d="M3 10l9-7l9 7"/>
<path d="M5 9v11h14V9"/>
<path d="M9 20v-6h6v6"/>
</svg>`,


edit: `
<svg viewBox="0 0 24 24">
<path d="M4 20l4-1l10-10l-3-3L5 16z"/>
<path d="M14 5l3 3"/>
</svg>`,

trash: `
<svg viewBox="0 0 24 24">
<path d="M5 7h14"/>
<path d="M9 7V4h6v3"/>
<path d="M8 7v12"/>
<path d="M16 7v12"/>
<path d="M6 7l1 13h10l1-13"/>
</svg>`,

light: `
<svg viewBox="0 0 24 24">
<path d="M9 18h6M10 21h4M12 2a7 7 0 0 0-4 12c.7.8 1.4 1.8 1.8 3h4.4c.4-1.2 1.1-2.2 1.8-3A7 7 0 0 0 12 2z"/>
</svg>`,

fan: `
<svg viewBox="0 0 24 24">
<path d="M12 12m-2 0a2 2 0 1 0 4 0a2 2 0 1 0-4 0M12 4c2 0 3 2 2 4l-2 4M20 12c0 2-2 3-4 2l-4-2M12 20c-2 0-3-2-2-4l2-4M4 12c0-2 2-3 4-2l4 2"/>
</svg>`,

door: `
<svg viewBox="0 0 24 24">
<path d="M7 3l10-1v20l-10-1V3zm4 9h1"/>
</svg>`,

lock: `
<svg viewBox="0 0 24 24">
<path d="M7 11V8a5 5 0 0 1 10 0v3M6 11h12v10H6z"/>
</svg>`,

motion: `
<svg viewBox="0 0 24 24" fill="none"
     stroke="currentColor"
     stroke-width="2"
     stroke-linecap="round"
     stroke-linejoin="round">
    <!-- Head -->
    <circle cx="8" cy="5" r="2"/>
    <!-- Body -->
    <path d="M8 7.5v5"/>
    <path d="M8 9l-3 2"/>
    <path d="M8 9l3 2"/>
    <path d="M8 12l-2 5"/>
    <path d="M8 12l3 5"/>
    <!-- Detection waves -->
    <path d="M15 7a3 3 0 0 1 0 10"/>
    <path d="M18 4a6.5 6.5 0 0 1 0 16"/>
</svg>
`,

temperature: `
<svg viewBox="0 0 24 24" fill="none"
     stroke="currentColor"
     stroke-width="2"
     stroke-linecap="round"
     stroke-linejoin="round">
    <!-- Head -->
    <circle cx="8" cy="5" r="2"/>
    <!-- Body -->
    <path d="M8 7.5v5"/>
    <path d="M8 9l-3 2"/>
    <path d="M8 9l3 2"/>
    <path d="M8 12l-2 5"/>
    <path d="M8 12l3 5"/>
    <!-- Detection waves -->
    <path d="M15 7a3 3 0 0 1 0 10"/>
    <path d="M18 4a6.5 6.5 0 0 1 0 16"/>
</svg>
`,

water: `
<svg viewBox="0 0 24 24">
<path d="M12 3s5 6 5 10a5 5 0 1 1-10 0c0-4 5-10 5-10z"/>
</svg>`,

outlet: `
<svg viewBox="0 0 24 24">
<path d="M7 3v4M17 3v4M5 7h14v14H5zM9 12h1M14 12h1"/>
</svg>`,

bell: `
<svg viewBox="0 0 24 24">
<path d="M12 3a4 4 0 0 1 4 4v4l2 3H6l2-3V7a4 4 0 0 1 4-4zm-2 15a2 2 0 0 0 4 0"/>
</svg>`,

curtain: `
<svg viewBox="0 0 24 24">
<path d="M4 4h16M6 4v16M10 4v16M14 4v16M18 4v16"/>
</svg>`,

tv: `
<svg viewBox="0 0 24 24">
<path d="M4 6h16v12H4zM9 20h6M10 4l2 2l2-2"/>
</svg>`,

generic: `
<svg viewBox="0 0 24 24">
<path d="M12 2l2.4 5l5.6.8l-4 4l1 5.7L12 15l-5 2.5l1-5.7l-4-4l5.6-.8z"/>
</svg>`

};

)rawliteral";

#endif
