#ifndef CHANNEL_JS_H
#define CHANNEL_JS_H

#include <pgmspace.h>

const char CHANNEL_JS[] PROGMEM = R"rawliteral(
async function getChannels(id = null)
{
    let url = "/api/channels";
    if(id != null)url += "?id=" + id;
    return await apiGet(url);
}
//==============================================================
let channels = [];
let selectedChannelIcon = 0;
//==============================================================
async function editChannel(id)
{
    const channel = await apiGet(`/api/channels?id=${id}`);
    if(!channel)
    {
        showToast("Channel not found","error");
        return;
    }
    showChannelDialog(channel);
}
//==============================================================
async function showChannelDialog(channel = null)
{
    const rooms = await apiGet("/api/rooms");
    let roomOptions = `<option value="0">No Room</option>`;
    if(Array.isArray(rooms))
    {
        roomOptions += rooms.map(r => `
            <option value="${r.id}"
                ${channel?.roomId == r.id ? "selected" : ""}>
                ${r.name}
            </option>
        `).join("");
    }
    Dialog.form(
    {
        title: channel ? "Edit Channel" : "New Channel",
        content: `
            <div class="form-group">
                <label>Name</label>
                <input id="chName" class="textbox" value="${channel?.name ?? ""}">
            </div>
            <div class="form-group">
                <label>Room</label>
                <select id="chRoom" class="textbox">
                    ${roomOptions}
                </select>
            </div>
            <div class="form-group">
                <label>Type</label>
                <select id="chType" class="textbox">
                    <option value="0"
                        ${channel?.type==0?"selected":""}>
                        Digital Input
                    </option>
                    <option value="1"
                        ${channel?.type==1?"selected":""}>
                        Digital Output
                    </option>
                </select>
            </div>
            <div class="form-group">
                <label>Driver</label>
                <select id="chDriver" class="textbox">
                </select>
            </div>
            <div class="form-group">
                <label>Device</label>
                <select id="chDevice" class="textbox">
                </select>
            </div>
            <div class="form-group">
                <label>Pin</label>
                <select id="chPin" class="textbox">
                </select>
            </div>
            <div class="form-group">
                <label>Icon</label>
                <div id="iconPicker" class="icon-picker"></div>
            </div>
            <label class="checkbox">
                <input id="chEnabled"
                       type="checkbox"
                       ${channel?.enabled!==false?"checked":""}>
                Enabled
            </label>
            <label class="checkbox">
                <input id="chFavorite"
                       type="checkbox"
                       ${channel?.favorite?"checked":""}>
                Favorite
            </label>
            <label class="checkbox">
                <input id="chActiveLow"
                       type="checkbox"
                       ${channel?.activeLow?"checked":""}>
                Active Low
            </label>
        `,
        onSave: async() =>
        {
            return saveChannel(channel?.id);
        }
    });

    fillDrivers(channel);
    fillDevices(channel);
    fillPins(channel);    
    selectedChannelIcon = channel?.icon ?? 0;
    buildIconPicker();
}
function fillDrivers(channel)
{
    $("chDriver").innerHTML = `
        <option value="0">PCF8574</option>
    `;
}
//==============================================================
function fillDevices(channel)
{
    const select = $("chDevice");
    let html = "";
    for(let i=0;i<8;i++)
    {
        html += `
            <option value="${i}"
                ${channel?.device==i?"selected":""}>
                PCF ${i+1}
            </option>
        `;
    }
    select.innerHTML = html;
}
//==============================================================
function fillPins(channel)
{
    const select = $("chPin");
    let html = "";
    for(let i=0;i<8;i++)
    {
        html += `
            <option
                value="${i}"
                ${channel?.pin==i?"selected":""}>
                Pin ${i}
            </option>
        `;
    }
    select.innerHTML = html;
}
//==============================================================
function buildIconPicker()
{
    const picker = $("iconPicker");
    picker.innerHTML = "";
    const names =
    [
        "light",
        "fan",
        "door",
        "lock",
        "motion",
        "temperature",
        "water",
        "outlet",
        "bell",
        "curtain",
        "tv",
        "generic"
    ];
    names.forEach((name,index)=>
    {
        const item = document.createElement("div");
        item.className = "icon-item" + (index===selectedChannelIcon ? " selected" : "");
        item.innerHTML = icon(name,26);
        item.onclick = () =>
        {
            selectedChannelIcon = index;
            buildIconPicker();
        };
        picker.appendChild(item);
    });
}
//==============================================================
async function saveChannel(id)
{
    const body =
    {
        driverId: Number($("chDriver").value),
        device: Number($("chDevice").value),
        pin: Number($("chPin").value),
        name: $("chName").value.trim(),
        roomId: Number($("chRoom").value),
        type: Number($("chType").value),
        icon: selectedChannelIcon,
        enabled: $("chEnabled").checked,
        favorite: $("chFavorite").checked,
        activeLow: $("chActiveLow").checked
    };
    if(body.name === "")
    {
        showToast("Name is required","error");
        $("chName").focus();
        return false;
    }
    let result;
    if(id)
    {
        result = await apiPut(`/api/channels?id=${id}`, body);
    }
    else
    {
        result = await apiPost("/api/channels", body);
    }
    if(!result || result.success === false)
    {
        showToast(result?.message ?? "Save failed","error");
        return false;
    }
    showToast("Saved","success");
    return true;
}
//==============================================================
async function toggleChannel(id)
{
    const channel = findChannel(id);
    if(!channel) return;
    const result = await apiPost("/api/channels/state",
    {
        id,state: !channel.state
    }
    );
    if(!result || result.success === false)
    {
        showToast(
            result?.message ?? "Toggle failed",
            "error"
        );
        return;
    }
    // هیچ کاری نکن.
    // WebSocket خودش UI را آپدیت می‌کند.
}
//==============================================================
async function deleteChannel(id)
{
    const result = await apiDelete(`/api/channels?id=${id}`);
    if(!result || result.success === false)
    {
        showToast(result?.message ?? "Delete failed","error");
        return false;
    }
    showToast("Channel deleted","success");
    return true;
}
//==============================================================
async function initChannels()
{
    channels = await getChannels();
    if(!Array.isArray(channels)) channels = [];
}
//==============================================================
function findChannel(id)
{
    return channels.find(c => c.id == id);
}
//==============================================================
function removeChannelLocal(id)
{
    channels = channels.filter(c => c.id != id);
}
//==============================================================
function addChannelToList(channel)
{
    channels.push(channel);
    channels.sort((a,b)=> a.roomId - b.roomId || (a.name || "").localeCompare(b.name || ""));
    renderChannelsTable();
}
//==============================================================
function updateChannelLocal(channel)
{
    const index = channels.findIndex(c=>c.id==channel.id);
    if(index < 0) return;
    Object.assign(channels[index],channel);
    channels.sort((a,b)=>
        a.roomId - b.roomId ||
        a.name.localeCompare(b.name)
    );
}
//==============================================================
function renderChannels()
{
    const body = $("page");
    let html = `
        <div class="page-header">
            <div class="page-title">
                Channels
            </div>
            <button
                class="btn primary"
                onclick="showChannelDialog()">
                ${icon("plus")}
                Add Channel
            </button>
        </div>
        <table class="table">
            <thead>
                <tr>
                    <th>Name</th>
                    <th>Room</th>
                    <th>Type</th>
                    <th>State</th>
                    <th></th>
                </tr>
            </thead>
            <tbody>
    `;
    channels.forEach(channel =>
    {
        const room = findRoom(channel.roomId);
        html += `
            <tr>
                <td>
                    ${iconByIndex(channel.icon)}
                    ${channel.name}
                </td>
                <td>
                    ${room ? room.name : "-"}
                </td>
                <td>
                    ${
                        channel.type == 0 ?
                        "Input" :
                        "Output"
                    }
                </td>
                <td>
                    ${
                        channel.state ?
                        "ON" :
                        "OFF"
                    }
                </td>
                <td class="actions">
                    <button
                        class="icon-btn"
                        onclick="editChannel(${channel.id})">
                        ${icon("edit")}
                    </button>
                    <button
                        class="icon-btn danger"
                        onclick="deleteChannel(${channel.id})">
                        ${icon("trash")}
                    </button>
                </td>
            </tr>
        `;
    });
    html += `
            </tbody>
        </table>
    `;
    body.innerHTML = html;
}
//==============================================================
async function showChannels()
{
    await initChannels();
    const app = $("content");
    app.innerHTML = `
        <div class="page">
            <div class="page-header">
                <h2>Channels</h2>
                <button class="btn primary"
                        onclick="showChannelDialog()">
                    <i class="fa-solid fa-plus"></i>
                    Add Channel
                </button>
            </div>
            <div id="channelsTable"></div>
        </div>
    `;
    channels = await getChannels();
    renderChannelsTable();
}
//==============================================================
function updateChannelInList(channel)
{
    const index = channels.findIndex(c => c.id == channel.id);

    if(index < 0)
        return;

    channels[index] = channel;

    renderChannelsTable();
}
//==============================================================
function removeChannelFromList(id)
{
    channels = channels.filter(c => c.id != id);

    renderChannelsTable();
}
//==============================================================

function renderChannelsTable()
{
    const container = $("channelsTable");
    let html = `
        <table class="table">
            <thead>
                <tr>
                    <th>ID</th>
                    <th>Name</th>
                    <th>Room</th>
                    <th>Type</th>
                    <th>State</th>
                    <th></th>
                </tr>
            </thead>
            <tbody>
    `;
        channels.forEach(channel =>
        {
            const room = findRoom(channel.roomId);
        
            html += buildChannelRow(
                channel,
                room ? room.name : "-"
            );
        });
        
        html += `
            </tbody>
        </table>
    `;
    container.innerHTML = html;
}
//==============================================================
function buildChannelRow(channel, roomName)
{
    return `
        <tr>
            <td>${channel.id}</td>
            <td>${channel.name}</td>
            <td>${roomName}</td>
            <td>
                ${channel.type == 0
                    ? "Input"
                    : "Output"}
            </td>
            <td>
                ${channel.state
                    ? "ON"
                    : "OFF"}
            </td>
            <td class="actions">
                <button class="icon-button"
                        onclick="editChannel(${channel.id})">
                    <i class="fa-solid fa-pen"></i>
                </button>
                <button class="icon-button danger"
                        onclick="deleteChannel(${channel.id})">
                    <i class="fa-solid fa-trash"></i>
                </button>
            </td>
        </tr>
    `;
}
//==============================================================







)rawliteral";

#endif
