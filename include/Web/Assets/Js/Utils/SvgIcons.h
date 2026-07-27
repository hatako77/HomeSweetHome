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


home:  `
<svg xmlns="http://www.w3.org/2000/svg" width="800px" height="800px" viewBox="0 0 24 24" fill="none" style="&#10;">
<path d="M19 9L19 17C19 18.8856 19 19.8284 18.4142 20.4142C17.8284 21 16.8856 21 15 21L14 21L10 21L9 21C7.11438 21 6.17157 21 5.58579 20.4142C5 19.8284 5 18.8856 5 17L5 9" stroke="#323232" stroke-width="2" stroke-linejoin="round" style="&#10;    stroke: white;&#10;"/>
<path d="M3 11L7.5 7L10.6713 4.18109C11.429 3.50752 12.571 3.50752 13.3287 4.18109L16.5 7L21 11" stroke="#323232" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="stroke: white;"/>
<path d="M10 21V17C10 15.8954 10.8954 15 12 15V15C13.1046 15 14 15.8954 14 17V21" stroke="#323232" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="&#10;    stroke: none;&#10;    fill: color(srgb 0 255 0);&#10;"/>
</svg>`,

edit: `
<svg xmlns="http://www.w3.org/2000/svg" width="800px" height="800px" viewBox="0 0 24 24" fill="none">
<path d="M21.2799 6.40005L11.7399 15.94C10.7899 16.89 7.96987 17.33 7.33987 16.7C6.70987 16.07 7.13987 13.25 8.08987 12.3L17.6399 2.75002C17.8754 2.49308 18.1605 2.28654 18.4781 2.14284C18.7956 1.99914 19.139 1.92124 19.4875 1.9139C19.8359 1.90657 20.1823 1.96991 20.5056 2.10012C20.8289 2.23033 21.1225 2.42473 21.3686 2.67153C21.6147 2.91833 21.8083 3.21243 21.9376 3.53609C22.0669 3.85976 22.1294 4.20626 22.1211 4.55471C22.1128 4.90316 22.0339 5.24635 21.8894 5.5635C21.7448 5.88065 21.5375 6.16524 21.2799 6.40005V6.40005Z" stroke="#000000" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round" style="
    stroke: rgb(0 255 0);
"></path>
<path d="M11 4H6C4.93913 4 3.92178 4.42142 3.17163 5.17157C2.42149 5.92172 2 6.93913 2 8V18C2 19.0609 2.42149 20.0783 3.17163 20.8284C3.92178 21.5786 4.93913 22 6 22H17C19.21 22 20 20.2 20 18V13" stroke="#000000" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round" style="
    stroke: rgb(255 255 255);
"></path>
</svg>`,

delete: `
<svg xmlns="http://www.w3.org/2000/svg" width="800px" height="800px" viewBox="0 0 24 24" fill="none">
<path d="M12 2.75C11.0215 2.75 10.1871 3.37503 9.87787 4.24993C9.73983 4.64047 9.31134 4.84517 8.9208 4.70713C8.53026 4.56909 8.32557 4.1406 8.46361 3.75007C8.97804 2.29459 10.3661 1.25 12 1.25C13.634 1.25 15.022 2.29459 15.5365 3.75007C15.6745 4.1406 15.4698 4.56909 15.0793 4.70713C14.6887 4.84517 14.2602 4.64047 14.1222 4.24993C13.813 3.37503 12.9785 2.75 12 2.75Z" fill="#1C274C" style="
    fill: white;
"></path>
<path d="M2.75 6C2.75 5.58579 3.08579 5.25 3.5 5.25H20.5001C20.9143 5.25 21.2501 5.58579 21.2501 6C21.2501 6.41421 20.9143 6.75 20.5001 6.75H3.5C3.08579 6.75 2.75 6.41421 2.75 6Z" fill="#1C274C" style="
    fill: white;
"></path>
<path d="M5.91508 8.45011C5.88753 8.03681 5.53015 7.72411 5.11686 7.75166C4.70356 7.77921 4.39085 8.13659 4.41841 8.54989L4.88186 15.5016C4.96735 16.7844 5.03641 17.8205 5.19838 18.6336C5.36678 19.4789 5.6532 20.185 6.2448 20.7384C6.83639 21.2919 7.55994 21.5307 8.41459 21.6425C9.23663 21.75 10.2751 21.75 11.5607 21.75H12.4395C13.7251 21.75 14.7635 21.75 15.5856 21.6425C16.4402 21.5307 17.1638 21.2919 17.7554 20.7384C18.347 20.185 18.6334 19.4789 18.8018 18.6336C18.9637 17.8205 19.0328 16.7844 19.1183 15.5016L19.5818 8.54989C19.6093 8.13659 19.2966 7.77921 18.8833 7.75166C18.47 7.72411 18.1126 8.03681 18.0851 8.45011L17.6251 15.3492C17.5353 16.6971 17.4712 17.6349 17.3307 18.3405C17.1943 19.025 17.004 19.3873 16.7306 19.6431C16.4572 19.8988 16.083 20.0647 15.391 20.1552C14.6776 20.2485 13.7376 20.25 12.3868 20.25H11.6134C10.2626 20.25 9.32255 20.2485 8.60915 20.1552C7.91715 20.0647 7.54299 19.8988 7.26957 19.6431C6.99616 19.3873 6.80583 19.025 6.66948 18.3405C6.52891 17.6349 6.46488 16.6971 6.37503 15.3492L5.91508 8.45011Z" fill="#1C274C" style="
    fill: white;
"></path>
<path d="M9.42546 10.2537C9.83762 10.2125 10.2051 10.5132 10.2464 10.9254L10.7464 15.9254C10.7876 16.3375 10.4869 16.7051 10.0747 16.7463C9.66256 16.7875 9.29502 16.4868 9.25381 16.0746L8.75381 11.0746C8.71259 10.6625 9.0133 10.2949 9.42546 10.2537Z" fill="#1C274C" style="
    fill: red;
"></path>
<path d="M15.2464 11.0746C15.2876 10.6625 14.9869 10.2949 14.5747 10.2537C14.1626 10.2125 13.795 10.5132 13.7538 10.9254L13.2538 15.9254C13.2126 16.3375 13.5133 16.7051 13.9255 16.7463C14.3376 16.7875 14.7051 16.4868 14.7464 16.0746L15.2464 11.0746Z" fill="#1C274C" style="
    fill: red;
"></path>
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
<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" height="800px" width="800px" version="1.1" id="Layer_1" viewBox="0 0 512 512" xml:space="preserve">

<circle cx="256.004" cy="66.616" r="66.616" style="&#10;    fill: white;&#10;"/>

<circle cx="71.516" cy="256" r="66.616" style="&#10;    fill: white;&#10;"/>
<circle style="fill: rgb(0 255 0);" cx="256.004" cy="256" r="66.616"/>
<circle cx="440.484" cy="256" r="66.616" style="&#10;    fill: white;&#10;"/>

<circle cx="256.004" cy="445.384" r="66.616" style="&#10;    fill: white;&#10;"/>

</svg>`,

scenes: `
<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" id="Icons" viewBox="0 0 32 32" xml:space="preserve">
<style type="text/css">
	.st0{fill:none;stroke:#ffffff;stroke-width:1;stroke-linecap:round;stroke-linejoin:round;stroke-miterlimit:10;}
</style>
<path class="st0" d="M27,31H5c-1.1,0-2-0.9-2-2V16h26v13C29,30.1,28.1,31,27,31z" style="
    fill: white;
    stroke: none;
"></path>
<path class="st0" d="M23.5,1.1L2.8,8.7c-1,0.4-1.6,1.5-1.2,2.6L3,15l24.4-8.9l-1.4-3.8C25.7,1.3,24.5,0.8,23.5,1.1z" style="fill: rgb(0 255 0);stroke: none;"></path>
<line class="st0" x1="3" y1="21" x2="29" y2="21"></line>
</svg>`,

schedules: `
<svg xmlns="http://www.w3.org/2000/svg" width="800px" height="800px" viewBox="0 0 24 24" fill="none">
<path d="M2 8H21" stroke="rgb(0, 255, 0)" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path><path d="M9 20H6C3.79086 20 2 18.2091 2 16V7C2 4.79086 3.79086 3 6 3H17C19.2091 3 21 4.79086 21 7V10" stroke="rgb(255, 255, 255)" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path>
<path d="M8 2V4" stroke="rgb(255, 255, 255)" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path>
<path d="M15 2V4" stroke="rgb(255, 255, 255)" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path>

<path d="M18.5 15.6429L17 17.1429" stroke="rgb(0, 255, 0)" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path>
<circle cx="17" cy="17" r="5" stroke="rgb(255, 255, 255)" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></circle>
</svg>`,

sensors: `
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 64 64" style="enable-background:new 0 0 64 64">
  <g style="fill: none;stroke-miterlimit:10;stroke: #ffffff;stroke-width: 3;">
    <path d="m49.2 41.2c1.5-2.8 2.3-5.9 2.3-9.2 0-3.3-.8-6.5-2.3-9.2" style="
"></path>
    <path d="m14.8 22.8c-1.5 2.8-2.3 5.9-2.3 9.2 0 3.3.8 6.5 2.3 9.2"></path>
    <path d="m55.2 44.8c2.1-3.8 3.3-8.2 3.3-12.8 0-4.7-1.2-9-3.3-12.8"></path>
    <path d="M8.8,19.2C6.7,23,5.5,27.3,5.5,32  c0,4.7,1.2,9,3.3,12.8"></path>
  </g>
  <g style="fill: white;">
    <path d="m33.2 17.9c1.9.2 3.6-1.3 3.7-3.2.1-1.9-1.3-3.6-3.2-3.7-1.9-.1-3.6 1.3-3.7 3.2-.1 1.9 1.3 3.6 3.2 3.7"></path>
    <path d="m30.3 19.4c.7-.5 1.5-.8 2.4-.7 1.2.1 2.2.8 2.8 1.7l3.5 7 4.8 3.3c.4.3.7.8.6 1.4-.1.9-.8 1.5-1.7 1.4-.3 0-.5-.1-.7-.2l-5.2-3.6c-.2-.1-.3-.3-.4-.5l-1.3-2.6-1.6 6.9 6.2 7.3c.1.2.2.5.3.8l1.7 8.8c0 .2 0 .3 0 .5-.1 1.3-1.2 2.3-2.5 2.2-1.1-.1-1.9-.9-2.1-1.9l-1.6-8.2-5-5.5-1.2 5.3c0 .3-.4.8-.5 1l-4.8 8.1c-.5.7-1.3 1.2-2.2 1.1-1.3-.1-2.3-1.2-2.2-2.5 0-.4.2-.8.3-1l4.5-7.5 3.7-16.5-2.4 2-1.3 5.9c-.2.8-.9 1.4-1.7 1.3-.9-.1-1.5-.8-1.4-1.7 0-.1 0-.1 0-.2l1.5-6.9c.1-.3.3-.6.5-.8l7-5.7" style="
    fill: rgb(0 255 0);
    /* stroke: rgb(0 255 0); */
"></path>
  </g>
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
