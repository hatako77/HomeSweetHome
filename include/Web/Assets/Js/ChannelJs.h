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
let selectedChannelIcon = 0;
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
                <select id="chRoom"
                        class="textbox">
                    ${roomOptions}
                </select>
            </div>

            <div class="form-group">
                <label>Type</label>
                <select id="chType"
                        class="textbox">
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

    selectedChannelIcon = channel?.icon ?? 0;
    buildIconPicker();
    fillDrivers(channel);
    fillDevices(channel);
    fillPins(channel);
}
function fillDrivers(channel)
{
    $("chDriver").innerHTML = `
        <option value="0">PCF8574</option>
    `;

}

function fillDevices(channel)
{
    const select = $("chDevice");

    let html = "";

    for(let i=0;i<8;i++)
    {
        html += `
            <option
                value="${i}"
                ${channel?.device==i?"selected":""}>
                PCF ${i+1}
            </option>
        `;
    }

    select.innerHTML = html;
}

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

function fillChannelIcons(selected = 0)
{
    const icons =
    [
        "Light",
        "Fan",
        "Door",
        "Lock",
        "Motion",
        "Temperature",
        "Water",
        "Outlet",
        "Bell",
        "Curtain",
        "TV",
        "Generic"
    ];

    const select = $("chIcon");

    select.innerHTML = icons.map((name,index)=>`
        <option value="${index}"
            ${selected==index?"selected":""}>
            ${name}
        </option>
    `).join("");
}

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

        item.className =
            "icon-item" +
            (index===selectedChannelIcon ? " selected" : "");

        item.innerHTML = icon(name,26);

        item.onclick = () =>
        {
            selectedChannelIcon = index;

            buildIconPicker();
        };

        picker.appendChild(item);
    });
}
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

    await initRooms();

    return true;
}

async function toggleChannel(id)
{
    const channel = findChannel(id);

    if(!channel)
        return;

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
    }

    // هیچ کاری نکن.
    // WebSocket خودش UI را آپدیت می‌کند.
}
async function deleteChannel(id)
{
    const result = await apiDelete(
        `/api/channels?id=${id}`
    );

    if(!result || result.success === false)
    {
        showToast(result?.message ?? "Delete failed","error");
        return false;
    }

    await initRooms();

    renderRooms();

    showToast("Channel deleted","success");

    return true;
}

async function moveChannel(channelId, roomId)
{
    return await apiPost("/api/channels/move",
    {
        channelId,
        roomId
    });
}


)rawliteral";

#endif
