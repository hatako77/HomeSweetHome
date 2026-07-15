#ifndef DASHBOARD_JS_H
#define DASHBOARD_JS_H

#include <pgmspace.h>

const char DASHBOARD_JS[] PROGMEM = R"rawliteral(

async function initDashboard()
{
    App.currentPage = "dashboard";

    try
    {
        $("content").innerHTML = `
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
    if(App.currentPage !== "dashboard")
        return;

    const rooms = App.state.rooms ?? [];

    let roomCount = rooms.length;
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

    const roomsEl = $("dashboardRooms");
    const channelsEl = $("dashboardChannels");
    const onEl = $("dashboardOn");
    const offEl = $("dashboardOffline");

    if(roomsEl)
        roomsEl.innerText = roomCount;

    if(channelsEl)
        channelsEl.innerText = channelCount;

    if(onEl)
        onEl.innerText = onCount;

    if(offEl)
        offEl.innerText = channelCount - onCount;
}

)rawliteral";

#endif
