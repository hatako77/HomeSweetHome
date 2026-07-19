#ifndef CHANNEL_JS_H
#define CHANNEL_JS_H

#include <pgmspace.h>

const char CHANNEL_JS[] PROGMEM = R"rawliteral(async function getChannels(id = null)
{
    let url = "/api/channels";
    if(id != null)url += "?id=" + id;
    return await apiGet(url);
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
