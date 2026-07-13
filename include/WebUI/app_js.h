#ifndef APP_JS_H
#define APP_JS_H

#include <pgmspace.h>

const char APP_JS[] PROGMEM = R"rawliteral(

"use strict";

const Icons =
{

light:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M9 21h6v-1H9v1zm3-20a7 7 0 0 0-4 12.7V17h8v-3.3A7 7 0 0 0 12 1z"/>
</svg>`,

fan:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M12 11a1 1 0 100 2 1 1 0 000-2zm0-9c2 0 3 2 2 5l-1 3a4 4 0 015-1c3 1 4 5 1 6l-3 1a4 4 0 01-2 5c-3 1-5-2-4-5l1-3a4 4 0 01-5 1c-3-1-4-5-1-6l3-1a4 4 0 012-5z"/>
</svg>`,

door:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M6 2l10-1v22H6V2zm7 10a1 1 0 100 2"/>
</svg>`,

lock:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M17 9V7a5 5 0 00-10 0v2H5v13h14V9h-2zm-8-2a3 3 0 116 0v2H9V7z"/>
</svg>`,

outlet:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M7 3h10a3 3 0 013 3v12a3 3 0 01-3 3H7a3 3 0 01-3-3V6a3 3 0 013-3zm2 6v3m6-3v3"/>
</svg>`,

tv:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M4 5h16v12H4V5zm5 14h6"/>
</svg>`,

curtain:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M4 2h16v20H4V2zm4 0v20m4-20v20m4-20v20"/>
</svg>`,

bell:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M12 22a2 2 0 002-2h-4a2 2 0 002 2zm6-6V11a6 6 0 10-12 0v5L4 18v1h16v-1l-2-2z"/>
</svg>`,

water:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M12 2C8 7 6 10 6 13a6 6 0 0012 0c0-3-2-6-6-11z"/>
</svg>`,

temperature:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M14 14.8V5a2 2 0 10-4 0v9.8a4 4 0 104 0z"/>
</svg>`,

motion:`
<svg viewBox="0 0 24 24" fill="currentColor">
<path d="M12 4a8 8 0 018 8h-2a6 6 0 00-6-6V4zm0 4a4 4 0 014 4h-2a2 2 0 00-2-2V8zm0 4a1 1 0 110 2 1 1 0 010-2z"/>
</svg>`,

generic:`
<svg viewBox="0 0 24 24" fill="currentColor">
<circle cx="12" cy="12" r="8"/>
</svg>`

};

let rooms=[];
let channels=[];
let socket=null;

function initSidebar()
{
    document.getElementById("sidebar").innerHTML = `
        <div class="sidebar-menu">

            <button class="menu-item active">
                <i class="fa-solid fa-house"></i>
                <span>Dashboard</span>
            </button>

            <button class="menu-item">
                <i class="fa-solid fa-door-open"></i>
                <span>Rooms</span>
            </button>

            <button class="menu-item">
                <i class="fa-solid fa-wand-magic-sparkles"></i>
                <span>Scenes</span>
            </button>

            <button class="menu-item">
                <i class="fa-solid fa-clock"></i>
                <span>Schedules</span>
            </button>

            <button class="menu-item">
                <i class="fa-solid fa-person-rays"></i>
                <span>Sensors</span>
            </button>

            <button class="menu-item">
                <i class="fa-solid fa-upload"></i>
                <span>OTA</span>
            </button>

            <button class="menu-item">
                <i class="fa-solid fa-gear"></i>
                <span>Settings</span>
            </button>

        </div>
    `;
    document.querySelectorAll(".menu-item").forEach(button =>
    {
        button.onclick = function ()
        {
            document.querySelectorAll(".menu-item")
                .forEach(x => x.classList.remove("active"));
    
            this.classList.add("active");
    
            switch(this.dataset.page)
            {
                case "dashboard":
                    showDashboard();
                    break;
    
                case "rooms":
                    showRooms();
                    break;
    
                case "scenes":
                    showScenes();
                    break;
    
                case "schedules":
                    showSchedules();
                    break;
    
                case "sensors":
                    showSensors();
                    break;
    
                case "ota":
                    showOTA();
                    break;
    
                case "settings":
                    showSettings();
                    break;
            }
        };
    });
}


function $(id)
{
    return document.getElementById(id);
}
function updateOTA(status)
{
    const state =
        document.getElementById("otaState");

    const progress =
        document.getElementById("otaProgress");

    const percent =
        document.getElementById("otaPercent");

    const speed =
        document.getElementById("otaSpeed");

    const eta =
        document.getElementById("otaEta");

    if(state)
        state.innerText = status.state;

    if(progress)
        progress.value = status.percent;

    if(percent)
        percent.innerText =
            status.percent + "%";

    if(speed)
        speed.innerText =
            status.speed.toFixed(1) + " KB/s";

    if(eta)
        eta.innerText =
            status.eta + " s";
}
function updateTile(tile, ch)
{
    if(ch.state)
        tile.classList.add("on");
    else
        tile.classList.remove("on");

    tile.querySelector(".tile-label").textContent =
        ch.state ? "ON" : "OFF";
}

function icon(name)
{
    return Icons[name] || Icons.generic;
}

async function api(url,method="GET",body=null)
{
    const options={method};

    if(body)
    {
        options.headers={
            "Content-Type":"application/json"
        };

        options.body=JSON.stringify(body);
    }

    const res=await fetch(url,options);

    return await res.json();
}

function create(tag,cls="")
{
    const e=document.createElement(tag);

    if(cls)
        e.className=cls;

    return e;
}

function clear(el)
{
    while(el.firstChild)
        el.removeChild(el.firstChild);
}
function createChannelTile(ch)
{
    const tile=create("div","channel tile");

    tile.dataset.id=ch.id;

    if(ch.state)
        tile.classList.add("on");

    tile.innerHTML=`
        <div class="tile-icon">
        ${icon(ch.icon)}
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

    tile.onclick=async()=>
    {
        await fetch(
            "/api/channels/toggle?id="+ch.id,
            {
                method:"POST"
            });
    };

    return tile;
}

function updateTile(tile,ch)
{
    if(ch.state)
        tile.classList.add("on");
    else
        tile.classList.remove("on");

    const lbl=tile.querySelector(".tile-label");

    if(lbl)
        lbl.innerText=ch.state ? "ON":"OFF";
}

function createRoomCard(room)
{
    const card=create("div","room-card");

    const header=create("div","room-header");

    const roomChannels=
        channels.filter(c=>c.roomId==room.id);

    header.innerHTML=`

<div class="room-name">

🏠 ${room.name}

</div>

<div class="room-count">

${roomChannels.length}

</div>

`;

    card.appendChild(header);

    const list=create("div","channel-list");

    list.id="room-"+room.id;

    roomChannels.forEach(ch=>
    {
        list.appendChild(
            createChannelTile(ch)
        );
    });

    card.appendChild(list);

    enableDrag(list);

    return card;
}
function enableDrag(channelList)
{
    new Sortable(channelList,
    {
        group:"rooms",

        animation:180,

        ghostClass:"dragging",

        onEnd:async(evt)=>
        {
            const channelId=evt.item.dataset.id;

            const roomId=
                evt.to.id.replace("room-","");

            await fetch(
                "/api/channels/assign?id="+
                channelId+
                "&roomId="+
                roomId,
                {
                    method:"POST"
                });

            const ch=channels.find(
                c=>c.id==channelId
            );

            if(ch)
                ch.roomId=parseInt(roomId);

            renderRooms();
        }
    });
}

function renderRooms()
{
    const container=$("roomsContainer");

    if(!container)
        return;

    clear(container);

    rooms.forEach(room=>
    {
        container.appendChild(
            createRoomCard(room)
        );
    });
}

async function loadRooms()
{
    const roomResult =
        await api("/api/rooms");

    const channelResult =
        await api("/api/channels");

    rooms = roomResult || [];

    channels = channelResult || [];

    renderRooms();
}
function connectWebSocket()
{
    const protocol =
        location.protocol === "https:"
        ? "wss://"
        : "ws://";

    socket = new WebSocket(
        protocol + location.host + "/ws"
    );

    socket.onopen = () =>
    {
        console.log("WebSocket Connected");
    };

    socket.onclose = () =>
    {
        console.log("WebSocket Closed");

        setTimeout(
            connectWebSocket,
            2000
        );
    };

    socket.onerror = (e) =>
    {
        console.log(e);
    };

    socket.onmessage = (event) =>
    {
        console.log("WS RAW:", event.data);

        let msg;

        try
        {
            msg = JSON.parse(event.data);
        }
        catch
        {
            return;
        }

        if(msg.type==="channel")
        {
            const ch = channels.find(
                c => c.id == msg.id
            );

            if(!ch)
                return;

            ch.state = msg.state;

            const tile =
                document.querySelector(
                    '[data-id="'+msg.id+'"]'
                );

            if(tile)
                updateTile(tile,ch);
        }

        if(msg.type==="reload")
        {
            loadRooms();
        }
        if(msg.type==="ota")
        {
            updateOTA(msg);
        }
    };
}

async function loadVersion()
{
    try
    {
        const version =
            await api("/api/version");

        const el =
            $("version");

        if(el)
            el.innerText =
                version.version;
    }
    catch(e)
    {
        console.log(e);
    }
}
function updateRoomCounter(roomId)
{
    const room = rooms.find(r => r.id == roomId);

    if(!room)
        return;

    const roomCard = [...document.querySelectorAll(".room-card")]
        .find(card =>
        {
            const list = card.querySelector(".channel-list");
            return list && list.id === "room-" + room.id;
        });

    if(!roomCard)
        return;

    const count = channels.filter(c => c.roomId == room.id).length;

    const lbl = roomCard.querySelector(".room-count");

    if(lbl)
        lbl.innerText = count + " Channels";
}

function refreshRoomCounters()
{
    rooms.forEach(r => updateRoomCounter(r.id));
}

function updateChannel(channel)
{
    const old = channels.find(c => c.id == channel.id);

    if(!old)
        return;

    Object.assign(old, channel);

    const tile = document.querySelector(
        '[data-id="' + channel.id + '"]'
    );

    if(tile)
    {
        tile.querySelector(".tile-name").innerText = old.name;

        tile.querySelector(".tile-icon").innerHTML =
            icon(old.icon);

        updateTile(tile, old);
    }
}

function findChannel(id)
{
    return channels.find(c => c.id == id);
}

function findRoom(id)
{
    return rooms.find(r => r.id == id);
}

function showToast(text)
{
    let toast = document.getElementById("toast");

    if(!toast)
    {
        toast = document.createElement("div");

        toast.id = "toast";

        document.body.appendChild(toast);
    }

    toast.innerText = text;

    toast.classList.add("show");

    clearTimeout(toast.timer);

    toast.timer = setTimeout(() =>
    {
        toast.classList.remove("show");
    },2000);
}
async function toggleChannel(id)
{
    try
    {
        await fetch(
            "/api/channels/toggle?id="+id,
            {
                method:"POST"
            });

        const ch=findChannel(id);

        if(!ch)
            return;

        ch.state=!ch.state;

        const tile=document.querySelector(
            '[data-id="'+id+'"]'
        );

        if(tile)
            updateTile(tile,ch);

        if(socket && socket.readyState===1)
        {
            socket.send(JSON.stringify(
            {
                type:"toggle",
                id:id
            }));
        }
    }
    catch(e)
    {
        console.log(e);
    }
}

async function moveChannel(channelId,roomId)
{
    await fetch(
        "/api/channels/assign?id="+
        channelId+
        "&roomId="+
        roomId,
        {
            method:"POST"
        });

    const ch=findChannel(channelId);

    if(ch)
        ch.roomId=roomId;

    renderRooms();

    refreshRoomCounters();
}

async function refreshData()
{
    await loadRooms();

    refreshRoomCounters();
}

function reconnectSocket()
{
    if(socket)
        socket.close();

    connectWebSocket();
}

function log()
{
    console.log.apply(console,arguments);
}
function initSortable()
{
    document.querySelectorAll(".channel-list").forEach(list =>
    {
        enableDrag(list);
    });
}

function showDashboard()
{
    document.getElementById("content").innerHTML="<h2>Dashboard</h2>";
}

function showRooms()
{
    document.getElementById("content").innerHTML="<h2>Rooms</h2>";
}

function showScenes()
{
    document.getElementById("content").innerHTML="<h2>Scenes</h2>";
}

function showSchedules()
{
    document.getElementById("content").innerHTML="<h2>Schedules</h2>";
}

function showSensors()
{
    document.getElementById("content").innerHTML="<h2>Sensors</h2>";
}

function showOTA()
{
    document.getElementById("content").innerHTML="<h2>OTA</h2>";
}

function showSettings()
{
    document.getElementById("content").innerHTML="<h2>Settings</h2>";
}

function init()
{
    loadVersion();

    loadRooms()
    .then(() =>
    {
        initSortable();

        connectWebSocket();
    });
}

document.addEventListener(
    "visibilitychange",
    () =>
    {
        if(document.visibilityState==="visible")
        {
            if(!socket || socket.readyState!==1)
                reconnectSocket();
        }
    });

window.addEventListener(
    "online",
    () =>
    {
        reconnectSocket();
    });

window.addEventListener(
    "offline",
    () =>
    {
        console.log("Offline");
    });

window.onload=()=>
{
    init();
    initSidebar();
    document.getElementById("content").innerHTML = `<h2>Dashboard</h2>`;
};
function heartbeat()
{
    if(socket && socket.readyState===1)
    {
        socket.send(
            JSON.stringify(
            {
                type:"ping"
            }
        ));
    }
}

setInterval(
    heartbeat,
    30000
);

window.addEventListener(
    "beforeunload",
    () =>
    {
        if(socket)
            socket.close();
    }
);

)rawliteral";

#endif
