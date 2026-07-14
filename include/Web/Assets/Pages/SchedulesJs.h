#ifndef SCHEDULES_JS_H
#define SCHEDULES_JS_H

#include <pgmspace.h>

const char SCHEDULES_JS[] PROGMEM=R"rawliteral(

let schedules=[];

async function loadSchedules()
{
	const data=await apiGet("/api/schedules");
	if(!data)
		return;
	schedules=data;
	renderSchedules();
}

function showSchedules()
{
	$("content").innerHTML=`
		<h2>Schedules</h2>
		<div style="margin-bottom:20px;">
			<button class="btn" onclick="newSchedule()">
				<i class="fa-solid fa-plus"></i>
				New Schedule
			</button>
		</div>
		<div id="schedulesContainer"></div>
	`;
	loadSchedules();
}

function renderSchedules()
{
	const container=$("schedulesContainer");
	if(!container)
		return;
	container.innerHTML="";
	schedules.forEach(schedule=>
	{
		container.appendChild(createScheduleCard(schedule));
	});
}

function createScheduleCard(schedule)
{
	const card=create("div","card");
	card.innerHTML=`
		<h3>${schedule.name}</h3>
		<div class="info">
			<div>Time : ${schedule.time}</div>
			<div>Scene : ${schedule.sceneName}</div>
			<div>Status : ${schedule.enabled?"Enabled":"Disabled"}</div>
		</div>
		<div style="display:flex;gap:10px;">
			<button class="btn" onclick="editSchedule(${schedule.id})">
				Edit
			</button>
			<button class="btn" onclick="deleteSchedule(${schedule.id})">
				Delete
			</button>
		</div>
	`;
	return card;
}

function newSchedule()
{
	console.log("New Schedule");
}

function editSchedule(id)
{
	console.log("Edit Schedule",id);
}

async function deleteSchedule(id)
{
	await apiPost("/api/schedules/remove?id="+id);
	loadSchedules();
}

)rawliteral";

#endif
