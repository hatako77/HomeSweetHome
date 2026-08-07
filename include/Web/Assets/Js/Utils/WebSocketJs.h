#ifndef WEBSOCKET_JS_H
#define WEBSOCKET_JS_H

#include <pgmspace.h>

const char WEBSOCKET_JS[] PROGMEM = R"rawliteral(

let socket = null;
let reconnectTimer = null;

function initWebSocket()
{
    connectWebSocket();
}
function updateRunningScenesBar()
{
    const bar = $("runningScenesBar");

    if(!bar)
        return;

    bar.innerHTML = "";

    scenes
        .filter(x => x.running)
        .forEach(scene =>
        {
            bar.appendChild(createRunningScene(scene));
        });
}
function createRunningScene(scene)
{
    const div = document.createElement("div");

    div.className = "running-scene";

    div.dataset.id = scene.id;

    div.innerHTML = `
        <div class="running-scene-icon">
            ${icon(scene.icon,20)}
        </div>

        <div class="running-scene-progress">
            <div
                class="running-scene-fill"
                style="width:${scene.progress}%">
            </div>
        </div>
    `;

    return div;
}
function connectWebSocket()
{
    if(socket)
        socket.close();

    const protocol =
        location.protocol === "https:" ? "wss://" : "ws://";

    socket = new WebSocket(protocol + location.host + "/ws");

    socket.onopen = () =>
    {
        if(reconnectTimer)
        {
            clearTimeout(reconnectTimer);
            reconnectTimer = null;
        }
    };

    socket.onclose = () =>
    {
        reconnectTimer = setTimeout(connectWebSocket,3000);
    };

    socket.onerror = error =>
    {
        console.error(error);
    };

    socket.onmessage = event =>
    {
        let msg;

        try
        {
            msg = JSON.parse(event.data);
        }
        catch
        {
            return;
        }

        switch(msg.type)
        {
            //--------------------------------------------------
            // CHANNEL
            //--------------------------------------------------            
            case "channel":
                handleChannelMessage(msg);            
                break;
            //--------------------------------------------------
            // ROOM
            //--------------------------------------------------
            case "room":
            handleRoomMessage(msg);        
            break;
            //--------------------------------------------------
            // OTA
            //--------------------------------------------------
            case "ota":
                if(msg.data && typeof updateOTA === "function")  updateOTA(msg.data);
                break;
            //--------------------------------------------------
            // SYSTEM
            //--------------------------------------------------
            case "system":
                if(msg.action === "reload") location.reload();
                break;
            //--------------------------------------------------
            // sceneProgress
            //--------------------------------------------------
            case "sceneProgress":
            {
                if(!Array.isArray(msg.items)) break;            
                for(const item of msg.items)
                {
                    const scene = findScene(item.id);            
                    if(!scene) continue;            
                    scene.running = item.running;
                    scene.progress = item.progress;            
                    updateSceneCard(scene);
                    updateRunningScenesBar();
                }            
                break;
            }
            //--------------------------------------------------
            // NOTIFICATION
            //--------------------------------------------------
            case "notification":
                if(msg.data?.text) toastInfo(msg.data.text);
                break;
        }
    };
}

function handleChannelMessage(msg)
{
    if(!msg.data)
        return;

    switch(msg.action)
    {
        case "created":
            addChannelToList(msg.data);
            renderRooms();
            break;

        case "updated":
            updateChannelInList(msg.data);
            break;

        case "changed":
            updateChannelInList(msg.data);
            break;

        case "deleted":
            removeChannelFromList(msg.data.id);
            renderRooms();
            break;
    }
}
async function handleRoomMessage(msg)
{
    switch(msg.action)
    {
        case "changed":
        case "added":
        case "updated":
        case "removed":

            await initRooms();
            break;
    }
}

function wsSend(type, action, data = {})
{
    if(!socket)
    {
        return;
    }

    const msg = {
        type,
        action,
        data
    };

    socket.send(JSON.stringify(msg));
}
)rawliteral";

#endif
