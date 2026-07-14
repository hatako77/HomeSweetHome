#ifndef WEBSOCKET_JS_H
#define WEBSOCKET_JS_H

#include <pgmspace.h>

const char WEBSOCKET_JS[] PROGMEM = R"rawliteral(
let socket=null;
let reconnectTimer=null;
function connectWebSocket()
{
  if(socket)
  socket.close();
  const protocol=location.protocol==="https:"?"wss://":"ws://";
  socket=new WebSocket(protocol+location.host+"/ws");
    socket.onopen=()=>
    {
    console.log("WebSocket Connected");
    if(reconnectTimer)
      {
        clearTimeout(reconnectTimer);
        reconnectTimer=null;
      }
    };
  socket.onclose=()=>
    {
    console.log("WebSocket Disconnected");
    reconnectTimer=setTimeout(connectWebSocket,3000);
    };
socket.onerror=(e)=>
{
console.log(e);
};
socket.onmessage=(event)=>
{
let msg;
try
{
msg=JSON.parse(event.data);
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
loadRooms();
break;
case "ota":
if(msg.data)
updateOTA(msg.data);
break;
case "notification":
if(msg.data)
showToast(msg.data.text);
break;
}
};
}
function wsSend(type,action,data={})
{
if(!socket)
return;
if(socket.readyState!==1)
return;
socket.send(JSON.stringify({
type:type,
action:action,
data:data
}));
}
)rawliteral";

#endif
