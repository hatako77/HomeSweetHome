#ifndef STATE_JS_H
#define STATE_JS_H

#include <pgmspace.h>

const char STATE_JS[] PROGMEM = R"rawliteral(

window.State =
{
    rooms: [],
    scenes: [],
    schedules: [],
    sensors: [],
    settings: {},
    ota: {},

    async loadRooms()
    {
        const data = await apiGet("/api/rooms");
        this.rooms = data || [];
        return this.rooms;
    },

    async loadScenes()
    {
        const data = await apiGet("/api/scenes");
        this.scenes = data || [];
        return this.scenes;
    },

    async loadSchedules()
    {
        const data = await apiGet("/api/schedules");
        this.schedules = data || [];
        return this.schedules;
    },

    async loadSensors()
    {
        const data = await apiGet("/api/sensors");
        this.sensors = data || [];
        return this.sensors;
    },

    async loadSettings()
    {
        const data = await apiGet("/api/settings");
        this.settings = data || {};
        return this.settings;
    }
};

)rawliteral";

#endif
