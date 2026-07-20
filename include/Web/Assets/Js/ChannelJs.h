#ifndef CHANNEL_JS_H
#define CHANNEL_JS_H

#include <pgmspace.h>

const char CHANNEL_JS[] PROGMEM = R"rawliteral(async function getChannels(id = null)
{
    let url = "/api/channels";
    if(id != null)url += "?id=" + id;
    return await apiGet(url);
}

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
                <label>Icon</label>
                <select id="chIcon"
                        class="textbox">
                </select>
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

    fillChannelIcons(channel?.icon ?? 0);
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

async function saveChannel(id)
{
    const body =
    {
        name: $("chName").value.trim(),
        roomId: Number($("chRoom").value),
        type: Number($("chType").value),
        icon: Number($("chIcon").value),
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

    await loadChannels();

    return true;
}

function buildChannelForm(channel)
{
    const c =
    {
        id: channel?.id ?? 0,
        name: channel?.name ?? "",
        roomId: channel?.roomId ?? 0,
        type: channel?.type ?? 0,
        icon: channel?.icon ?? 0,
        enabled: channel?.enabled ?? true,
        favorite: channel?.favorite ?? false,
        activeLow: channel?.activeLow ?? false
    };

    return `
        <div class="form-group">
            <label>Name</label>
            <input
                id="channelName"
                class="textbox"
                value="${c.name}">
        </div>

        <div class="form-group">
            <label>Room</label>
            <select id="channelRoom" class="textbox">
                ${buildRoomOptions(c.roomId)}
            </select>
        </div>

        <div class="form-group">
            <label>Type</label>
            <select id="channelType" class="textbox">

            </select>
        </div>

        <div class="form-group">
            <label>Icon</label>
            <select id="channelIcon" class="textbox">

            </select>
        </div>

        <label>
            <input
                type="checkbox"
                id="channelEnabled"
                ${c.enabled ? "checked" : ""}>
            Enabled
        </label>

        <label>
            <input
                type="checkbox"
                id="channelFavorite"
                ${c.favorite ? "checked" : ""}>
            Favorite
        </label>

        <label>
            <input
                type="checkbox"
                id="channelActiveLow"
                ${c.activeLow ? "checked" : ""}>
            Active Low
        </label>
    `;
}
function buildRoomOptions(selectedId)
{
    let html = `<option value="0">No Room</option>`;

    getRooms().forEach(room =>
    {
        html += `
            <option
                value="${room.id}"
                ${room.id == selectedId ? "selected" : ""}>
                ${room.name}
            </option>
        `;
    });

    return html;
}
async function createChannel(channel)
{
    return await apiPost("/api/channels", channel);
}

async function updateChannel(id, channel)
{
    return await apiPut("/api/channels?id=" + id, channel);
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

    await loadChannels();

    renderRooms();

    showToast("Channel deleted","success");

    return true;
}
async function updateChannel(channel)
{
    const result = await apiPut(
        `/api/channels?id=${channel.id}`,
        channel
    );

    if(!result || result.success === false)
    {
        showToast(result?.message ?? "Update failed","error");
        return false;
    }

    await loadChannels();

    renderRooms();

    showToast("Channel updated","success");

    return true;
}
async function deleteChannel(id)
{
    return await apiDelete("/api/channels?id=" + id);
}

async function moveChannel(channelId, roomId)
{
    return await apiPost("/api/channels/move",
    {
        channelId,
        roomId
    });
}

async function toggleChannel(id)
{
    const channel = await getChannels(id);
    if(!channel)return;

    const result = await updateChannel(id,
    {
        state: !channel.state
    });

    if(result?.success === false)
    {
        showToast(result.message ?? "Operation failed","error");
        return;
    }

    await loadRooms();
    renderRooms();
}

)rawliteral";

#endif
