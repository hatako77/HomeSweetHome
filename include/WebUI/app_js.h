#ifndef APP_JS_H
#define APP_JS_H

#include <pgmspace.h>


const Icons =
{
light:`
<svg viewBox="0 0 24 24">
<path d="M9 21h6v-1H9v1zm3-20a7 7 0 0 0-4 12.7V17h8v-3.3A7 7 0 0 0 12 1z"/>
</svg>`,

fan:`
<svg viewBox="0 0 24 24">
<path d="M12 11a1 1 0 1 0 0 2 1 1 0 0 0 0-2zm0-9c2 0 3 2 2 5l-1 3a4 4 0 0 1 5-1c3 1 4 5 1 6l-3 1a4 4 0 0 1-2 5c-3 1-5-2-4-5l1-3a4 4 0 0 1-5 1c-3-1-4-5-1-6l3-1a4 4 0 0 1 2-5c.6-.2 1.3-.3 2-.3z"/>
</svg>`,

door:`
<svg viewBox="0 0 24 24">
<path d="M6 2l10-1v22H6V2zm7 10a1 1 0 1 0 0 2"/>
</svg>`,

lock:`
<svg viewBox="0 0 24 24">
<path d="M17 9V7a5 5 0 0 0-10 0v2H5v13h14V9h-2zm-8-2a3 3 0 1 1 6 0v2H9V7z"/>
</svg>`,

outlet:`
<svg viewBox="0 0 24 24">
<path d="M7 3h10a3 3 0 0 1 3 3v12a3 3 0 0 1-3 3H7a3 3 0 0 1-3-3V6a3 3 0 0 1 3-3zm2 6v3m6-3v3"/>
</svg>`,

tv:`
<svg viewBox="0 0 24 24">
<path d="M4 5h16v12H4V5zm5 14h6"/>
</svg>`,

curtain:`
<svg viewBox="0 0 24 24">
<path d="M4 2h16v20H4V2zm4 0v20m4-20v20m4-20v20"/>
</svg>`,

bell:`
<svg viewBox="0 0 24 24">
<path d="M12 22a2 2 0 0 0 2-2h-4a2 2 0 0 0 2 2zm6-6V11a6 6 0 1 0-12 0v5L4 18v1h16v-1l-2-2z"/>
</svg>`,

water:`
<svg viewBox="0 0 24 24">
<path d="M12 2C8 7 6 10 6 13a6 6 0 0 0 12 0c0-3-2-6-6-11z"/>
</svg>`,

temperature:`
<svg viewBox="0 0 24 24">
<path d="M14 14.8V5a2 2 0 1 0-4 0v9.8a4 4 0 1 0 4 0z"/>
</svg>`,

motion:`
<svg viewBox="0 0 24 24">
<path d="M12 4a8 8 0 0 1 8 8h-2a6 6 0 0 0-6-6V4zm0 4a4 4 0 0 1 4 4h-2a2 2 0 0 0-2-2V8zm0 4a1 1 0 1 1 0 2 1 1 0 0 1 0-2z"/>
</svg>`,

generic:`
<svg viewBox="0 0 24 24">
<circle cx="12" cy="12" r="8"/>
</svg>`
};


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
            row.className = "channel tile";
            if(ch.state)
                row.classList.add("on");

            row.dataset.id = ch.id;

            row.innerHTML = `
            <div class="tile-icon">
                ${Icons[ch.icon] || Icons.generic}
            </div>
            
            <div class="tile-name">
                ${ch.name}
            </div>
            
            <div class="tile-bottom">
            
                <div class="channel-state"></div>
            
                <div class="tile-label">
                    ${ch.state ? "ON" : "OFF"}
                </div>
            
            </div>
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
