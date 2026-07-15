#ifndef ROOM_JS_H
#define ROOM_JS_H

#include <pgmspace.h>

const char ROOM_JS[] PROGMEM = R"rawliteral(
let rooms = [];

async function initRooms()
{
    const data = await apiGet("/api/rooms");

    if(!data)
        return;

    rooms = data;
}

function showRooms()
{
    $("content").innerHTML=`
        <h2>Rooms</h2>
        <div id="roomsContainer"></div>
    `;
    renderRooms();
}

function renderRooms()
{
    const container=$("roomsContainer");
    if(!container)
        return;
    container.innerHTML="";
    rooms.forEach(room=>
    {
        container.appendChild(createRoomCard(room));
    });
}

function createRoomCard(room)
{
    const card=create("div","room-card");
    card.innerHTML=`
        <div class="room-header">
            <div class="room-name">
                <i class="fa-solid fa-house"></i>
                ${room.name}
            </div>
            <div class="room-count">${room.channels.length}</div>
        </div>
        <div class="channel-list" data-room="${room.id}"></div>
    `;
    const list=card.querySelector(".channel-list");
    room.channels.forEach(channel=>
    {
        list.appendChild(createChannelTile(channel));
    });
    enableDrag(list);
    return card;
}

function createChannelTile(channel)
{
    const tile=create("div","tile");
    tile.dataset.id=channel.id;
    if(channel.state)
        tile.classList.add("on");
    tile.innerHTML=`
        <div class="tile-icon">
            ${icon(channel.icon)}
        </div>
        <div class="tile-name">
            ${channel.name}
        </div>
        <div class="tile-bottom">
            <div class="channel-state"></div>
            <div class="tile-label">
                ${channel.state?"ON":"OFF"}
            </div>
        </div>
    `;
    tile.onclick=()=>
    {
        toggleChannel(channel.id);
    };
    return tile;
}

function updateTile(tile,channel)
{
    if(channel.state)
        tile.classList.add("on");
    else
        tile.classList.remove("on");
    const label=tile.querySelector(".tile-label");
    if(label)
        label.innerText=channel.state?"ON":"OFF";
}

function updateChannel(channel)
{
    const room=findRoom(channel.roomId);
    if(!room)
        return;
    const local=room.channels.find(c=>c.id==channel.id);
    if(!local)
        return;
    local.state=channel.state;
    const tile=document.querySelector(`[data-id="${channel.id}"]`);
    if(tile)
        updateTile(tile,local);
    updateDashboard();
}

function toggleChannel(id)
{
    wsSend("channel","toggle",{id:id});
}

function enableDrag(list)
{
    new Sortable(list,
    {
        animation:150,
        group:"channels",
        draggable:".tile",
        onEnd(evt)
        {
            const channelId=parseInt(evt.item.dataset.id);
            const roomId=parseInt(evt.to.dataset.room);
            moveChannel(channelId,roomId);
        }
    });
}

async function moveChannel(channelId,roomId)
{
    const result=await apiPost("/api/channel/move",
    {
        channelId:channelId,
        roomId:roomId
    });
    if(result&&result.success)
        initRooms();
        renderRooms();
        updateDashboard();

}

function findRoom(id)
{
    return rooms.find(r=>r.id==id);
}

function findChannel(id)
{
    for(const room of rooms)
    {
        const channel=room.channels.find(c=>c.id==id);
        if(channel)
            return channel;
    }
    return null;
}

function refreshRooms()
{
        initRooms();
        renderRooms();
        updateDashboard();
}

function updateRoomCounter(roomId)
{
    const room=findRoom(roomId);
    if(!room)
        return;
    const card=document.querySelector(`.room-card .channel-list[data-room="${roomId}"]`);
    if(!card)
        return;
    const header=card.parentElement.querySelector(".room-count");
    if(header)
        header.innerText=room.channels.length;
}

function refreshRoomCounters()
{
    rooms.forEach(room=>
    {
        updateRoomCounter(room.id);
    });
}

function refreshData()
{
        initRooms();
        renderRooms();
        updateDashboard();
}

function addRoom(room)
{
    rooms.push(room);
    renderRooms();
    updateDashboard();
}

function removeRoom(id)
{
    const index=rooms.findIndex(r=>r.id==id);
    if(index<0)
        return;
    rooms.splice(index,1);
    renderRooms();
    updateDashboard();
}

function updateRoom(room)
{
    const index=rooms.findIndex(r=>r.id==room.id);
    if(index<0)
        return;
    rooms[index]=room;
    renderRooms();
    updateDashboard();
}

function addChannel(roomId,channel)
{
    const room=findRoom(roomId);
    if(!room)
        return;
    room.channels.push(channel);
    renderRooms();
    updateDashboard();
}

function removeChannel(channelId)
{
    for(const room of rooms)
    {
        const index=room.channels.findIndex(c=>c.id==channelId);
        if(index>=0)
        {
            room.channels.splice(index,1);
            renderRooms();
            updateDashboard();
            return;
        }
    }
}

function updateChannelInfo(channel)
{
    const local=findChannel(channel.id);
    if(!local)
        return;
    Object.assign(local,channel);
    renderRooms();
    updateDashboard();
}

)rawliteral";

#endif
