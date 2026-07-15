#ifndef ROUTER_JS_H
#define ROUTER_JS_H

#include <pgmspace.h>

const char ROUTER_JS[] PROGMEM = R"rawliteral(

const Router =
{
    current:"dashboard",

    async navigate(page)
    {
        this.current=page;

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
                await showOTA();
                break;

            case "settings":
                await showSettings();
                break;
        }
    }
};

)rawliteral";

#endif
