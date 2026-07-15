#ifndef SENSOR_JS_H
#define SENSOR_JS_H

#include <pgmspace.h>

const char SENSOR_JS[] PROGMEM=R"rawliteral(

let sensors=[];

async function loadSensors()
{
	const data=await apiGet("/api/sensors");
	if(!data)
		return;
	sensors=data;
	renderSensors();
}

function showSensors()
{
	$("content").innerHTML=`
		<h2>Sensors</h2>
		<div id="sensorsContainer"></div>
	`;
	loadSensors();
}

function renderSensors()
{
	const container=$("sensorsContainer");
	if(!container)
		return;
	container.innerHTML="";
	sensors.forEach(sensor=>
	{
		container.appendChild(createSensorCard(sensor));
	});
}

function createSensorCard(sensor)
{
	const card=create("div","card");
	card.innerHTML=`
		<h3>${sensor.name}</h3>
		<div class="info">
			<div>Type : ${sensor.type}</div>
			<div>Value : ${sensor.value}</div>
			<div>Status : ${sensor.online?"Online":"Offline"}</div>
		</div>
	`;
	return card;
}

function refreshSensors()
{
	loadSensors();
}

)rawliteral";

#endif
