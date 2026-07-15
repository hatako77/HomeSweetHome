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
        console.log(error);
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
            case "channel":

                if(msg.data)
                    updateChannel(msg.data);

                break;

            case "room":

                if(msg.data)
                    updateRoom(msg.data);

                break;

            case "roomAdded":

                if(msg.data)
                    addRoom(msg.data);

                break;

            case "roomRemoved":

                if(msg.data)
                    removeRoom(msg.data.id);

                break;

            case "ota":

                if(msg.data && typeof updateOTA === "function")
                    updateOTA(msg.data);

                break;

            case "notification":

                if(msg.data && typeof showToast === "function")
                    showToast(msg.data.text);

                break;
        }
    };
}

function wsSend(type, action, data = {})
{
    if(!socket)
        return;

    if(socket.readyState !== WebSocket.OPEN)
        return;

    socket.send(JSON.stringify(
    {
        type,
        action,
        data
    }));
}

)rawliteral";

#endif
