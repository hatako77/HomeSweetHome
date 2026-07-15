#ifndef APP_JS_H
#define APP_JS_H

#include <pgmspace.h>

const char APP_JS[] PROGMEM = R"rawliteral(
'use strict';
window.App =
{
    version: '2.0.0',
    ws: null,
    api: {},
    dom: {},
    ui: {},
    state:
    {
        rooms: [],
        channels: [],
        scenes: [],
        schedules: [],
        sensors: []
    },
    currentPage: "dashboard"
};
window.addEventListener("DOMContentLoaded", async () =>
{
    try
    {
        if(typeof initDom === "function")initDom();
        if(typeof initIcons === "function")initIcons();
        if(typeof initSidebar === "function")initSidebar();
        if(typeof initDialog === "function")initDialog();
        if(typeof initModal === "function")initModal();
        if(typeof initToast === "function")initToast();
        if(typeof initLoader === "function")initLoader();
        if(typeof initApi === "function")initApi();
        if(typeof initWebSocket === "function")initWebSocket();

        // ---------- Load Initial State ----------

        if(typeof initRooms === "function")await initRooms();
        if(typeof initChannels === "function")await initChannels();
        if(typeof initScenes === "function")await initScenes();
        if(typeof initSchedules === "function")await initSchedules();
        if(typeof initSensors === "function")await initSensors();

        // ---------- Show First Page ----------

        if(typeof initDashboard === "function")await initDashboard();
    }
    catch(ex)
    {
        console.error(ex);
    }
});

)rawliteral";

#endif
