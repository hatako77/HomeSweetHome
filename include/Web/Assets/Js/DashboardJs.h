#ifndef DASHBOARD_JS_H
#define DASHBOARD_JS_H

#include <pgmspace.h>

const char DASHBOARD_JS[] PROGMEM = R"rawliteral(

async function initDashboard()
{
    try
    {
        await initRooms();

        $("content").innerHTML=`
        <h2>Dashboard</h2>

        <div class="dashboard-grid">

            <div class="dashboard-card">
                <h3>Rooms</h3>
                <div class="value" id="dashboardRooms">0</div>
            </div>

            <div class="dashboard-card">
                <h3>Channels</h3>
                <div class="value" id="dashboardChannels">0</div>
            </div>

            <div class="dashboard-card">
                <h3>Active</h3>
                <div class="value" id="dashboardOn">0</div>
            </div>

            <div class="dashboard-card">
                <h3>Offline</h3>
                <div class="value" id="dashboardOffline">0</div>
            </div>

        </div>
        `;

        updateDashboard();
    }
    finally
    {
        hideLoader();
    }
}

function updateDashboard()
{
    if(!$("dashboardRooms")) return;
    const roomCount=rooms.length;

    let channelCount=0;
    let onCount=0;
    console.log(rooms);
    rooms.forEach(room=>
    {
        channelCount+=room.channels.length;

        room.channels.forEach(channel=>
        {
            if(channel.state)
                onCount++;
        });
    });

    $("dashboardRooms").innerText=roomCount;
    $("dashboardChannels").innerText=channelCount;
    $("dashboardOn").innerText=onCount;
    $("dashboardOffline").innerText=channelCount-onCount;
}
)rawliteral";

#endif
