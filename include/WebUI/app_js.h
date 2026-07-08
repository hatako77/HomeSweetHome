#ifndef APP_JS_H
#define APP_JS_H

#include <pgmspace.h>

const char APP_JS[] PROGMEM = R"rawliteral(

async function loadRooms()
{
    const roomResult = await (await fetch("/api/rooms")).json();
    const channelResult = await (await fetch("/api/channels")).json();
    
    const rooms = roomResult.data;
    const channels = channelResult;
    const container = document.getElementById("roomsContainer");

    container.innerHTML = "";

    rooms.forEach(room =>
    {
        const card = document.createElement("div");
        card.className = "room-card";

        const list = channels.filter(c => c.roomId == room.id);

        card.innerHTML = `
            <div class="room-header">
                <div class="room-name">
                    <i class="fa-solid fa-house"></i>
                    ${room.name}
                </div>

                <div class="room-count">
                    ${list.length} Channels
                </div>
            </div>

            <div class="channel-list" id="room-${room.id}"></div>
        `;

        container.appendChild(card);

        const channelList = card.querySelector(".channel-list");
        
        list.forEach(ch =>
        {
            const row = document.createElement("div");

            row.className = "channel";

            if(ch.state)
                row.classList.add("on");

            row.dataset.id = ch.id;

            row.innerHTML = `
                <div class="channel-left">
                    <div class="channel-icon">
                        <i class="fa-solid fa-lightbulb"></i>
                    </div>

                    <div class="channel-name">
                        ${ch.name}
                    </div>
                </div>

                <div class="channel-state"></div>
            `;

            row.onclick = async() =>
            {
                await fetch("/api/channels/toggle?id="+ch.id,
                {
                    method:"POST"
                });

                loadRooms();
            };

            channelList.appendChild(row);
        });

        new Sortable(channelList,
        {
            group: "rooms",
        
            animation: 200,
        
            ghostClass: "dragging",
        
            onEnd: async function(evt)
            {
                const channelId = evt.item.dataset.id;
        
                const roomId = evt.to.id.replace("room-","");
        
                await fetch(
                    `/api/channels/assign?id=${channelId}&roomId=${roomId}`,
                    {
                        method:"POST"
                    });
        
                loadRooms();
            }
        });
    });
}


window.onload = function()
{
    loadRooms();};

)rawliteral";

#endif
