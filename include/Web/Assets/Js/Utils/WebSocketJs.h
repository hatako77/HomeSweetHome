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
