#ifndef ROUTER_JS_H
#define ROUTER_JS_H

#include <pgmspace.h>

const char ROUTER_JS[] PROGMEM = R"rawliteral(

const Router =
{
    async navigate(page)
    {
        App.currentPage = page;

        switch(page)
        {
            case "dashboard":
                await initDashboard();
                break;

            case "rooms":
                await showRooms();
                break;

            case "scenes":
                await showScenes();
                break;

            case "schedules":
                await showSchedules();
                break;

            case "sensors":
                await showSensors();
                break;

            case "ota":
                await initOTA();
                break;

            case "settings":
                await showSettings();
                break;

            default:
                await initDashboard();
                break;
        }

        if(typeof setActiveMenu === "function")
            setActiveMenu(page);
    }
};

)rawliteral";

#endif
