#ifndef ROOM_JS_H
#define ROOM_JS_H

#include <pgmspace.h>

const char ROOM_JS[] PROGMEM = R"rawliteral(
async function deleteRoom(id)
{
    const room = getRooms().find(r => r.id == id);

    if(!room)
        return;

    Dialog.confirm(
        "Delete Room",
        `Delete "${room.name}" ?`,
        async() =>
        {
            const result = await api(
                "/api/rooms?id=" + id,
                {
                    method:"DELETE"
                });

            if(!result || !result.success)
            {
                toastError(
                    result?.message ??
                    "Cannot delete room"
                );
                return;
            }

            toastSuccess("Room deleted");

            await initRooms();

            Router.navigate("rooms");
        }
    );
}

async function editRoom(id)
{
    const room = getRooms().find(r => r.id == id);

    if(!room)
        return;

    Dialog.prompt(
    {
        title: "Edit Room",
        value: room.name,
        placeholder: "Room name",

        onSave: async(name)=>
        {
            const result = await api(
                "/api/rooms?id=" + id,
                {
                    method: "PUT",
                    headers:
                    {
                        "Content-Type":"application/json"
                    },
                    body: JSON.stringify(
                    {
                        name: name
                    })
                });

            if(!result || !result.success)
            {
                toastError(result.message || "Cannot update room");
                return;
            }

            toastSuccess("Room updated");

            await initRooms();

            Router.navigate("rooms");
        }
    });
}

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
                toastError(result.message || "Cannot create room");
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
                ${icon("generic")}
                ${room.name}
            </div>

            <div class="room-actions">

                <div class="room-count">
                    ${(room.channels ?? []).length}
                </div>
                
                <button class="icon-btn add-channel-btn" title="Add Channel">
                    ${icon("plus")}
                </button>

                <button
                    class="icon-btn"
                    onclick="editRoom(${room.id})"
                    title="Edit">
                    ${icon("edit")}
                </button>

                <button
                    class="icon-btn danger"
                    onclick="deleteRoom(${room.id})"
                    title="Delete">
                    ${icon("trash")}
                </button>

            </div>

        </div>

        <div
            class="channel-list"
            data-room="${room.id}">
        </div>
    `;

    const list = card.querySelector(".channel-list");

    (room.channels ?? []).forEach(channel =>
    {
        list.appendChild(createChannelTile(channel));
    });

    enableDrag(list);

    card.querySelector(".add-channel-btn").onclick=(e)=>
    {
        e.stopPropagation();
        addChannel(room.id);
    };
    return card;
}


async function addChannel(roomId)
{
    showChannelDialog({
        roomId: roomId,
        enabled: true,
        favorite: false,
        activeLow: false,
        type: 1,
        icon: 0,
        driverId: 0,
        device: 0,
        pin: 0
    });
}

function createChannelTile(channel)
{
    const tile = create("div","channel-tile");
    tile.classList.toggle("on",channel.state);
    tile.setAttribute("data-id", channel.id);
    tile.innerHTML = `
        <div class="channel-info">

            <div class="channel-icon">
                ${icon(channel.icon)}
            </div>

            <div class="channel-text">

                <div class="channel-name">
                    ${channel.name}
                </div>

                <div class="channel-state ${channel.state ? "on" : "off"}">
                    ${channel.state ? "ON" : "OFF"}
                </div>

            </div>

        </div>

        <div class="channel-actions">

            <button class="icon-btn edit-btn" title="Edit">
                ${icon("edit")}
            </button>

            <button class="icon-btn delete-btn" title="Delete">
                ${icon("trash")}
            </button>

        </div>
    `;

    //--------------------------------------------------
    // Toggle
    //--------------------------------------------------

    tile.onclick = async () =>
    {
        await toggleChannel(channel.id);
    };

    //--------------------------------------------------
    // Edit
    //--------------------------------------------------

    tile.querySelector(".edit-btn").onclick = (e) =>
    {
        e.stopPropagation();

        editChannel(channel.id);
    };

    //--------------------------------------------------
    // Delete
    //--------------------------------------------------

    tile.querySelector(".delete-btn").onclick = (e) =>
    {
        e.stopPropagation();

        removeChannel(channel.id);
    };

    return tile;
}

function updateTile(tile, channel)
{
    tile.classList.toggle("on", channel.state);

    const state = tile.querySelector(".channel-state");

    if(state)
    {
        state.classList.toggle("on", channel.state);
        state.classList.toggle("off", !channel.state);
        state.innerText = channel.state ? "ON" : "OFF";
    }

    const name = tile.querySelector(".channel-name");

    if(name)
        name.innerText = channel.name;

        const iconElement = tile.querySelector(".channel-icon");        
        if(iconElement)
            iconElement.innerHTML = icon(channel.icon);
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

        return;
    }

    const tile=document.querySelector(`[data-id="${channel.id}"]`);

    if(tile)
        updateTile(tile,local);
}

async function toggleChannel(id)
{
    const ch = findChannel(id);

    if(!ch)
        return;

    await apiPut("/api/channels?id=" + id,{
        state: !ch.state
    });
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

function addChannelLocal(roomId, channel)
{
    const room = findRoom(roomId);

    if(!room)
        return;

    if(!room.channels)
        room.channels = [];

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
