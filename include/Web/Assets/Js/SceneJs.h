#ifndef SCENE_JS_H
#define SCENE_JS_H

#include <pgmspace.h>

const char SCENE_JS[] PROGMEM=R"rawliteral(

let scenes=[];

async function loadScenes()
{
	const data=await apiGet("/api/scenes");
	if(!data)
		return;
	scenes=data;
	renderScenes();
}

function showScenes()
{
	$("content").innerHTML=`
		<h2>Scenes</h2>
		<div class="toolbar">
			<button class="btn" onclick="newScene()">
				<i class="fa-solid fa-plus"></i>
				New Scene
			</button>
		</div>
		<div id="scenesContainer"></div>
	`;
	loadScenes();
}

function renderScenes()
{
	const container=$("scenesContainer");
	if(!container)
		return;
	container.innerHTML="";
	scenes.forEach(scene=>
	{
		container.appendChild(createSceneCard(scene));
	});
}

function createSceneCard(scene)
{
	const card=create("div","card");
	card.innerHTML=`
		<h3>${scene.name}</h3>
		<div class="info">
			<div>Actions : ${scene.actions}</div>
			<div>${scene.enabled?"Enabled":"Disabled"}</div>
		</div>
		<div style="display:flex;gap:10px;">
			<button class="btn" onclick="runScene(${scene.id})">
				Run
			</button>
			<button class="btn" onclick="editScene(${scene.id})">
				Edit
			</button>
			<button class="btn" onclick="deleteScene(${scene.id})">
				Delete
			</button>
		</div>
	`;
	return card;
}

function runScene(id)
{
	apiPost("/api/scenes/run?id="+id);
}

function editScene(id)
{
	console.log("Edit Scene",id);
}

function newScene()
{
	console.log("New Scene");
}

async function deleteScene(id)
{
	await apiPost("/api/scenes/remove?id="+id);
	loadScenes();
}

)rawliteral";

#endif
