#ifndef ROOM_JS_H
#define ROOM_JS_H

#include <pgmspace.h>

const char ROOM_JS[] PROGMEM = R"rawliteral(

async function initRooms()
{
    console.log("initRooms");
    const data = await apiGet("/api/rooms");

    if(!data)
        return;

    data.sort((a,b)=>
        a.name.localeCompare(b.name,"fa")
    );

    data.forEach(room=>
    {
        if(room.channels)
        {
            room.channels.sort((a,b)=>
                a.name.localeCompare(b.name,"fa")
            );
        }
    });

    App.state.rooms = data;
    console.log(data.length);
    console.log(App.state.rooms);
}


function getRooms()
{
    return App.state.rooms ?? [];
}

async function showRooms()
{
    App.currentPage = "rooms";

    $("content").innerHTML = `
        <h2>Rooms</h2>
        <button class="btn btn-primary" onclick="addRoom()">
            <i class="fa-solid fa-plus"></i>
            Add Room
        </button>
        <div id="roomsContainer"></div>
    `;

    renderRooms();
}

async function addRoom()
{
    Dialog.prompt({
        title: "Add Room",
        placeholder: "Room name",
        onSave: async(name)=>
        {
            const result = await apiPost(
                "/api/rooms",
                {
                    name
                }
            );
            
            if(!result || !result.success)
            {
                toastError("Cannot create room");
                return;
            }


            toastSuccess("Room created");

            await initRooms();

            Router.navigate("rooms");
        }
    });
}

function renderRooms()
{
    console.log("renderRooms");
    if(App.currentPage !== "rooms")
        return;

    const container = $("roomsContainer");

    if(!container)
        return;

    container.innerHTML = "";

    getRooms().forEach(room =>
    {
        container.appendChild(createRoomCard(room));
    });
}

function createRoomCard(room)
{
    const card = create("div","room-card");

    card.innerHTML = `
        <div class="room-header">
            <div class="room-name">
                <i class="fa-solid fa-house"></i>
                ${room.name}
            </div>

            <div class="room-count">
                ${(room.channels ?? []).length}
            </div>
        </div>

        <div class="channel-list" data-room="${room.id}"></div>
    `;

    const list = card.querySelector(".channel-list");

    (room.channels ?? []).forEach(channel =>
    {
        list.appendChild(createChannelTile(channel));
    });

    enableDrag(list);

    return card;
}

function createChannelTile(channel)
{
    const tile = create("div","tile");

    tile.dataset.id = channel.id;

    tile.classList.toggle("on",channel.state);

    tile.innerHTML = `
        <div class="tile-icon">
            ${icon(channel.icon)}
        </div>

        <div class="tile-name">
            ${channel.name}
        </div>

        <div class="tile-bottom">
            <div class="channel-state"></div>

            <div class="tile-label">
                ${channel.state ? "ON" : "OFF"}
            </div>
        </div>
    `;

    tile.onclick = () => toggleChannel(channel.id);

    return tile;
}

function updateTile(tile,channel)
{
    tile.classList.toggle("on",channel.state);

    const label = tile.querySelector(".tile-label");

    if(label)
        label.innerText = channel.state ? "ON" : "OFF";
}

function updateChannel(channel)
{
    let sourceRoom = null;
    let sourceIndex = -1;
    let local = null;

    for(const room of getRooms())
    {
        if(!room.channels)
            continue;

        const index = room.channels.findIndex(c=>c.id===channel.id);

        if(index>=0)
        {
            sourceRoom = room;
            sourceIndex = index;
            local = room.channels[index];
            break;
        }
    }

    if(!local)
        return;

    const oldRoomId = local.roomId;

    Object.assign(local,channel);

    if(oldRoomId !== channel.roomId)
    {
        sourceRoom.channels.splice(sourceIndex,1);

        const targetRoom = findRoom(channel.roomId);

        if(targetRoom)
        {
            if(!targetRoom.channels)
                targetRoom.channels=[];

            targetRoom.channels.push(local);
        }

        renderRooms();
        return;
    }

    const tile=document.querySelector(`[data-id="${channel.id}"]`);

    if(tile)
        updateTile(tile,local);
}

function toggleChannel(id)
{
        console.log("toggleChannel", id);
        wsSend("channel","toggle",{id});
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
            moveChannel(
                Number(evt.item.dataset.id),
                Number(evt.to.dataset.room)
            );
        }
    });
}

async function moveChannel(channelId,roomId)
{
    await apiPost("/api/channels/move",
    {
        channelId,
        roomId
    });

    // منتظر WebSocket هستیم.
}

function findRoom(id)
{
    return getRooms().find(r=>r.id===id);
}

function findChannel(id)
{
    for(const room of getRooms())
    {
        if(!room.channels)
            continue;

        const channel = room.channels.find(c=>c.id===id);

        if(channel)
            return channel;
    }

    return null;
}

function refreshRooms()
{
    renderRooms();
}

function updateRoomCounter(roomId)
{
    const room=findRoom(roomId);

    if(!room)
        return;

    const list=document.querySelector(`.channel-list[data-room="${roomId}"]`);

    if(!list)
        return;

    const counter=list.parentElement.querySelector(".room-count");

    if(counter)
        counter.innerText=(room.channels??[]).length;
}


function removeRoom(id)
{
    App.state.rooms=getRooms().filter(r=>r.id!==id);

    renderRooms();
}

function updateRoom(room)
{
    const index=getRooms().findIndex(r=>r.id===room.id);

    if(index<0)
        return;

    getRooms()[index]=room;

    renderRooms();
}

function addChannel(roomId,channel)
{
    const room=findRoom(roomId);

    if(!room)
        return;

    if(!room.channels)
        room.channels=[];

    room.channels.push(channel);

    renderRooms();
}

function removeChannel(channelId)
{
    for(const room of getRooms())
    {
        if(!room.channels)
            continue;

        const index=room.channels.findIndex(c=>c.id===channelId);

        if(index>=0)
        {
            room.channels.splice(index,1);

            renderRooms();

            return;
        }
    }
}

function updateChannelInfo(channel)
{
    updateChannel(channel);
}

)rawliteral";

#endif
