#ifndef CHANNEL_JS_H
#define CHANNEL_JS_H

#include <pgmspace.h>

const char CHANNEL_JS[] PROGMEM = R"rawliteral(async function getChannels(id = null)
{
    let url = "/api/channels";
    if(id != null)url += "?id=" + id;
    return await apiGet(url);
}
function showChannelDialog(channel = null)
{
    const editing = channel != null;

    Dialog.form({
        title: editing ? "Edit Channel" : "New Channel",

        content: buildChannelForm(channel),

        onSave: async () =>
        {

        }
    });
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
