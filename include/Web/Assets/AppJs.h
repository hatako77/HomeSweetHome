#ifndef APP_JS_H
#define APP_JS_H

#include <pgmspace.h>

const char APP_JS[] PROGMEM=R"rawliteral(

let rooms=[];
let scenes=[];
let schedules=[];
let sensors=[];

window.addEventListener("load",()=>
{
	loadSidebar();
	connectWebSocket();
	showDashboard();
});

window.addEventListener("resize",()=>
{
});

function refreshAll()
{
	loadRooms();
	loadScenes();
	loadSchedules();
	loadSensors();
}

)rawliteral";

#endif
