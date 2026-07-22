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

    await initRooms();
    await initChannels();
    
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
    if(App.currentPage !== "rooms")
        return;

    const container = $("roomsContainer");

    if(!container)
        return;

    container.innerHTML = "";

    const channelsByRoom = new Map();

    getChannels().forEach(channel =>
    {
        if(!channelsByRoom.has(channel.roomId))
            channelsByRoom.set(channel.roomId, []);

        channelsByRoom.get(channel.roomId).push(channel);
    });

    channelsByRoom.forEach(list =>
    {
        list.sort((a,b)=>
            (a.name || "").localeCompare(b.name || "", "fa")
        );
    });

    getRooms().forEach(room =>
    {
        container.appendChild(
            createRoomCard(room, channelsByRoom)
        );
    });
}

function createRoomCard(room, channelsByRoom)
{
    const roomChannels = channelsByRoom.get(room.id) || [];

    const card = create("div","room-card");

    card.innerHTML = `
        <div class="room-header">

            <div class="room-name">
                ${icon("generic")}
                ${room.name}
            </div>

            <div class="room-actions">

                <div class="room-count">
                    ${roomChannels.length}
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

        <div class="channel-list" data-room="${room.id}">
        </div>
    `;

    const list = card.querySelector(".channel-list");

    roomChannels.forEach(channel =>
    {
        list.appendChild(createChannelTile(channel));
    });

    card.querySelector(".add-channel-btn").onclick = (e) =>
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
    console.log("CREATE", channel.id, channel);
    const disabled = channel.connected === false;
    const tile = create("div","channel-tile");
    tile.setAttribute("data-id", channel.id);
    tile.innerHTML = `
        <div class="tile ${channel.state ? "on" : "off"} ${disabled ? "disabled" : ""}">

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
    `;

    //--------------------------------------------------
    // Toggle
    //--------------------------------------------------
    if(!disabled)
    {
           tile.onclick = async () =>
        {
            await toggleChannel(channel.id);
        };
    }

    return tile;
}

function updateTile(tile, channel)
{
    console.log(channel.id, channel.state);
    console.log(findChannel(channel.id).state);
    const inner = tile.querySelector(".tile");
    
    if(inner)
    {
        inner.classList.toggle("on", channel.state);
        inner.classList.toggle("off", !channel.state);
        console.log(inner.className);
    }
    console.log("updateTile", channel.id);
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
    console.log("UPDATE", local);
    console.log(channel);
    console.log(local === channel);
    console.log("ROOM updateChannel");
    const local = findChannel(channel.id);

    if(!local)
        return;

    const roomChanged = local.roomId !== channel.roomId;
    const connectedChanged = local.connected !== channel.connected;

    Object.assign(local, channel);

    if(roomChanged || connectedChanged)
    {
        renderRooms();
        return;
    }

    const tile = document.querySelector(`[data-id="${channel.id}"]`);

    if(tile)
        updateTile(tile, local);
}

function findRoom(id)
{
    return getRooms().find(r=>r.id===id);
}

function findChannel(id)
{
    return getChannels().find(c => c.id === id);
}

function refreshRooms()
{
    renderRooms();
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



)rawliteral";

#endif
