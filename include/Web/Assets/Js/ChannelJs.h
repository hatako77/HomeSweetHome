#ifndef CHANNEL_JS_H
#define CHANNEL_JS_H

#include <pgmspace.h>

const char CHANNEL_JS[] PROGMEM = R"rawliteral(
async function loadChannels(id = null)
{
    let url = "/api/channels";
    if(id != null)url += "?id=" + id;
    return await apiGet(url);
}
//==============================================================
const ChannelTypeNames =
{
    0: "Digital Input",
    1: "Digital Output",
    2: "Analog Input",
    3: "PWM Output",
    4: "Virtual",
    5: "Disabled"
};
const DriverNames =
{
    0: "PCF8574",
    1: "ESP32",
    2: "Virtual"
};
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
                <input id="chName"
                       class="textbox"
                       value="${channel?.name ?? ""}">
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
                        ${channel?.type == 0 ? "selected" : ""}>
                        Digital Input
                    </option>
                    <option value="1"
                        ${channel?.type == 1 ? "selected" : ""}>
                        Digital Output
                    </option>
                </select>
            </div>
            <div class="form-group">
                <label>Driver</label>
                <select id="chDriver" class="textbox"></select>
            </div>
            <div class="form-group">
                <label>Device</label>
                <select id="chDevice" class="textbox"></select>
            </div>

            <div class="form-group">
                <label>Pin</label>
                <select id="chPin" class="textbox"></select>
            </div>
            <div class="form-group">
                <label>Icon</label>
                <div id="iconPicker" class="icon-picker"></div>
            </div>
            <label class="checkbox">
                <input id="chEnabled"
                       type="checkbox"
                       ${channel?.enabled !== false ? "checked" : ""}>
                Enabled
            </label>
            <label class="checkbox">
                <input id="chFavorite"
                       type="checkbox"
                       ${channel?.favorite ? "checked" : ""}>
                Favorite
            </label>
            <label class="checkbox">
                <input id="chActiveLow"
                       type="checkbox"
                       ${channel?.activeLow ? "checked" : ""}>
                Active Low
            </label>
        `,
        onSave: async () =>
        {
            return saveChannel(channel?.id);
        }
    });
    fillDrivers(channel);
    fillDevices(channel);
    fillPins(channel);
    const driver = $("chDriver");
    const device = $("chDevice");
    await refreshPinList(Number($("chDriver").value),Number($("chDevice").value),channel?.id ?? 0,channel?.pin ?? -1);
    driver.onchange = async () =>
    {
        fillDevices(channel);
        await refreshPinList(Number($("chDriver").value),Number($("chDevice").value),channel?.id ?? 0,-1);
    };
    
    device.onchange = async () =>
    {
        fillPins(channel);
        await refreshPinList(Number($("chDriver").value),Number($("chDevice").value),channel?.id ?? 0,-1);
    };
    selectedChannelIcon = channel?.icon ?? 0;
    buildIconPicker();
}
//==============================================================
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
                PCF ${i}
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
        "ac",
        "door",
        "garage",
        "lock",
        "motion",
        "temperature",
        "water",
        "outlet",
        "bell",
        "curtain",
        "tv",
        "generic",
        "schedules"
    ];
    names.forEach((_, index) =>
    {
        const item = document.createElement("div");    
        item.className = "icon-item" + (index === selectedChannelIcon ? " selected" : "");    
        item.innerHTML = icon(index, 26);    
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
    if(id)
    {
        updateChannelInList(
        {
            id,
            ...body
        });
    }
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
    const channel = findChannel(id);

    if(!channel)
        return;

    Dialog.confirm(
        "Delete Channel",
        `Delete "${channel.name}" ?`,
        async () =>
        {
            const result = await api(
                `/api/channels?id=${id}`,
                {
                    method: "DELETE"
                }
            );

            if(!result || !result.success)
            {
                toastError(result?.message ?? "Cannot delete channel");
                return;
            }

            toastSuccess("Channel deleted");
            // هیچ render یا init اینجا لازم نیست
        }
    );
}
//==============================================================
async function initChannels()
{
    channels = await loadChannels();    
    if(!Array.isArray(channels)) channels = [];
}
//==============================================================
function getChannels()
{
    return channels;
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
    const container = $("channelsContainer");

    if(!container)
        return;

    container.innerHTML = "";

    getChannels().forEach(channel =>
    {
        container.appendChild(
            createChannelCard(channel)
        );
    });
}
//==============================================================
async function showChannels()
{
    await initChannels();
    const app = $("content");
    app.innerHTML = `
        <div class="page">
            <div class="page-header">
                <button class="btn primary" style="z-index: 999;border-radius: 5rem;position: fixed;bottom: 2rem;left: 2rem;height: 4rem;width: 4rem;"
                        onclick="showChannelDialog()">
                    <span class="svg-icon" style="width:24px;height:24px">                                
                        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round">
                            <path d="M12 5v14"></path>
                            <path d="M5 12h14"></path>
                        </svg>
                    </span>
                </button>
            </div>
            <div id="channelsContainer"></div>
        </div>
    `;
    await initChannels();
    renderChannelsTable();
}
//==============================================================
function updateChannelInList(channel)
{
    const local = channels.find(c => c.id === channel.id);
    if(!local) return;
    const roomChanged = local.roomId !== channel.roomId;
    const connectedChanged = local.connected !== channel.connected;

    Object.assign(local, channel);

    //----------------------------------------
    // Channels Page
    //----------------------------------------

    if(App.currentPage === "channels")
    {
        const row = document.querySelector(`tr[data-id="${local.id}"]`);

        if(row)
        {
            const room = findRoom(local.roomId);

            row.outerHTML = buildChannelRow(
                local,
                room ? room.name : "-"
            );
        }
        else
        {
            renderChannelsTable();
        }
    }

    //----------------------------------------
    // Rooms Page
    //----------------------------------------

    if(App.currentPage === "rooms")
    {
        if(roomChanged || connectedChanged)
        {
            renderRooms();
        }
        else
        {
            const tile = document.querySelector(
                `[data-id="${local.id}"]`
            );
            if(tile)
            {
                updateTile(tile, local);
            }
        }
    }
}
//==============================================================
function removeChannelFromList(id)
{
    const index = channels.findIndex(c => c.id == id);

    if(index < 0)
        return;

    channels.splice(index, 1);

    renderChannelsTable();

    if(App.currentPage === "rooms")
        renderRooms();
}
//==============================================================

function renderChannelsTable()
{
    renderChannels();
    console.log("test");
}
//==============================================================
async function editChannel(id)
{
    const channel = findChannel(id);

    if(!channel)
        return;

    showChannelDialog(
    {
        id: channel.id,
        name: channel.name,
        roomId: channel.roomId,
        enabled: channel.enabled,
        favorite: channel.favorite,
        activeLow: channel.activeLow,
        type: channel.type,
        icon: channel.icon,
        driverId: channel.driverId,
        device: channel.device,
        pin: channel.pin
    });
}
//==============================================================
async function loadUsedPins(driver, device, ignore = 0)
{
    return await apiGet(`/api/channels/usedpins?driver=${driver}&device=${device}&ignore=${ignore}`);
}
//==============================================================
async function refreshPinList(driver, device, ignoreId = 0, currentPin = -1)
{
    const channels = await loadUsedPins(driver, device, ignoreId);

    const select = $("chPin");
    select.innerHTML = "";

    for(let i = 0; i < 8; i++)
    {
        const option = document.createElement("option");
        option.value = i;

        const inUse = channels.find(ch =>
            Number(ch.driverId) === Number(driver) &&
            Number(ch.device)   === Number(device) &&
            Number(ch.pin)      === i &&
            Number(ch.id)       !== Number(ignoreId)
        );

        option.textContent = inUse
            ? `P${i} (In Use)`
            : `P${i}`;

        option.disabled = !!inUse && i !== currentPin;
        option.selected = i === currentPin;

        select.appendChild(option);
    }
}
//==============================================================
async function refreshPins(currentPin = -1)
{
    await refreshPinList(
    Number($("chDriver").value),
    Number($("chDevice").value),
    channel?.id ?? 0,
    currentPin
);
}
//==============================================================
function createChannelCard(channel)
{
    const card = create("div", "channel-card");

    if(channel.state)
        card.classList.add("active");

    card.dataset.id = channel.id;

    card.innerHTML = `
        <div class="channel-header">

            <div class="channel-icon">
                ${icon(channel.icon, 26)}
            </div>

            <div class="channel-info">

                <div class="channel-name">
                    ${channel.name}
                </div>

                <div class="channel-address">

                    <span>${ChannelTypeNames[channel.type] ?? "-"}</span>&nbsp; - &nbsp;
                    <span>${DriverNames[channel.driverId] ?? "-"}</span></br>
                    <span class="status-dot ${channel.connected ? "online" : "offline"}"></span>
                    <span class="">PCF${channel.device}</span>&nbsp; - &nbsp;
                    <span>Pin ${channel.pin}</span>

                </div>

            </div>

            <div class="channel-actions">

                <button style="align-items: center;justify-content: center;display: flex;background-color: #00000000;color: bisque;border-radius: 7rem;width: 2.5rem;height: 2.5rem;" class="icon-button edit">
                    ${icon("edit",20)}
                </button>

                <button style="align-items: center;justify-content: center;display: flex;background-color: #00000000;color: bisque;border-radius: 7rem;width: 2.5rem;height: 2.5rem;" class="icon-button delete">
                    ${icon("delete",20)}
                </button>

            </div>

        </div>
    `;

    card.querySelector(".edit").onclick = e =>
    {
        e.stopPropagation();
        editChannel(channel.id);
    };

    card.querySelector(".delete").onclick = e =>
    {
        e.stopPropagation();
        deleteChannel(channel.id);
    };

    return card;
}


//==============================================================
)rawliteral";

#endif
