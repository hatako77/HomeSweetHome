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

function connectWebSocket()
{
    if(socket)
        socket.close();

    const protocol =
        location.protocol === "https:" ? "wss://" : "ws://";

    socket = new WebSocket(protocol + location.host + "/ws");

    socket.onopen = () =>
    {
        console.log("WebSocket Connected");

        if(reconnectTimer)
        {
            clearTimeout(reconnectTimer);
            reconnectTimer = null;
        }
    };

    socket.onclose = () =>
    {
        console.log("WebSocket Disconnected");

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

        console.log("WS:", msg);

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

                if(msg.data && typeof updateOTA === "function")
                    updateOTA(msg.data);

                break;

            //--------------------------------------------------
            // SYSTEM
            //--------------------------------------------------

            case "system":

                if(msg.action === "reload")
                    location.reload();

                break;

            //--------------------------------------------------
            // NOTIFICATION
            //--------------------------------------------------

            case "notification":

                if(msg.data?.text)
                    toastInfo(msg.data.text);

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
        case "updated":
            updateChannel(msg.data);
            updateChannelInList(msg.data);
            break;

        case "created":
            addChannelToList(msg.data);
            addChannelLocal(msg.data.roomId, msg.data);
            break;

        case "deleted":
            removeChannelFromList(msg.data.id);
            removeChannel(msg.data.id);
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
    console.log("wsSend");

    if(!socket)
    {
        console.log("socket null");
        return;
    }

    console.log("ready =", socket.readyState);

    const msg = {
        type,
        action,
        data
    };

    console.log(msg);

    socket.send(JSON.stringify(msg));
}
)rawliteral";

#endif
