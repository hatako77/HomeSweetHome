#ifndef STATE_JS_H
#define STATE_JS_H

#include <pgmspace.h>

const char STATE_JS[] PROGMEM = R"rawliteral(

window.App = window.App || {};

App.state =
{
    rooms: [],
    channels: [],
    scenes: [],
    schedules: [],
    sensors: [],
    settings: {},
    dashboard:
    {
        roomCount: 0,
        channelCount: 0,
        onCount: 0,
        offCount: 0
    }
};

App.currentPage = "dashboard";

App.setState = function(key, value)
{
    this.state[key] = value;
};

App.getState = function(key)
{
    return this.state[key];
};

App.updateDashboardState = function()
{
    const rooms = this.state.rooms ?? [];

    let channelCount = 0;
    let onCount = 0;

    rooms.forEach(room =>
    {
        const channels = room.channels ?? [];

        channelCount += channels.length;

        channels.forEach(channel =>
        {
            if(channel.state)
                onCount++;
        });
    });

    this.state.dashboard =
    {
        roomCount: rooms.length,
        channelCount,
        onCount,
        offCount: channelCount - onCount
    };
};

#endif
)rawliteral";

#endif
