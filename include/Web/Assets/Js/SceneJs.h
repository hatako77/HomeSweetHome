#ifndef SCENE_JS_H
#define SCENE_JS_H

#include <pgmspace.h>

const char SCENE_JS[] PROGMEM=R"rawliteral(

let scenes=[];
let channels = [];

async function initScenes()
{
    const data = await apiGet("/api/scenes");
    if(!Array.isArray(data)) return;
    scenes = data;
    renderScenes();
}
//==============================================================
function showScenes()
{
    $("content").innerHTML = `
        <h2>Scenes</h2>
        <div class="toolbar">
            <button class="btn primary"
                    onclick="newScene()">
                <i class="fa-solid fa-plus"></i>
                New Scene
            </button>
        </div>
        <div id="sceneList" class="card-list"></div>
    `;
    initScenes();
}
//==============================================================
function renderScenes()
{
    const list = $("sceneList");
    list.innerHTML = "";
    scenes.forEach(scene =>
    {
        list.appendChild(createSceneCard(scene));
    });
}
//==============================================================
function findScene(id)
{
    return scenes.find(x => Number(x.id) === Number(id));
}
//==============================================================
function addSceneToList(scene)
{
    scenes.push(scene);
    renderScenes();
}
//==============================================================
function updateSceneInList(scene)
{
    const index = scenes.findIndex(x => Number(x.id) === Number(scene.id));
    if(index < 0)return;
    scenes[index] = scene;
    renderScenes();
}
//==============================================================
function removeSceneFromList(id)
{
    scenes = scenes.filter(x => Number(x.id) !== Number(id));
    renderScenes();
}
//==============================================================
function createSceneCard(scene)
{
    const card = create("div","card");
    card.innerHTML = `
        <div class="card-header">
            <div class="card-title">
                <i class="fa-solid fa-${scene.icon ?? "bolt"}"></i>
                <span>${scene.name}</span>
            </div>
            <div class="card-actions">
                <button class="icon-btn"
                        onclick="executeScene(${scene.id})">
                    <i class="fa-solid fa-play"></i>
                </button>
                <button class="icon-btn"
                        onclick="editScene(${scene.id})">
                    <i class="fa-solid fa-pen"></i>
                </button>
                <button class="icon-btn danger"
                        onclick="deleteScene(${scene.id})">
                    <i class="fa-solid fa-trash"></i>
                </button>
            </div>
        </div>
        <div class="card-body">
            <div>
                Actions :
                ${scene.actions.length}
            </div>
            <div>
                ${scene.enabled ? "Enabled" : "Disabled"}
            </div>
        </div>
    `;
    return card;
}
//==============================================================
async function newScene()
{
    await showSceneDialog();
}
//==============================================================
async function editScene(id)
{
    const scene = await apiGet(`/api/scenes?id=${id}`);
    if(!scene)return;
    await showSceneDialog(scene);
}
//==============================================================
async function executeScene(id)
{
    const result =await apiPost(`/api/scenes?action=execute&id=${id}`);

    if(!result || result.success === false)
    {
        showToast(result?.message ?? "Execute failed","error");
        return;
    }
    showToast("Scene executed","success");
}
//==============================================================
async function deleteScene(id)
{
    if(!await Dialog.confirm("Delete scene?"))return;
    const result =await apiDelete(`/api/scenes?id=${id}`);
    if(!result || result.success === false)
    {
        showToast(result?.message ?? "Delete failed","error");
        return;
    }
    removeSceneFromList(id);
    showToast("Deleted","success");
}
//==============================================================
async function showSceneDialog(scene = null)
{
    Dialog.form(
    {
        title: scene ? "Edit Scene" : "New Scene",
        content: `
            <div class="form-group">
                <label>Name</label>
                <input
                    id="sceneName"
                    class="textbox"
                    value="${scene?.name ?? ""}">
            </div>
            <div class="form-group">
                <label>Icon</label>
                <div id="sceneIconPicker"
                     class="icon-picker">
                </div>
            </div>
            <label class="checkbox">
                <input
                    id="sceneEnabled"
                    type="checkbox"
                    ${scene?.enabled !== false ? "checked" : ""}>
                Enabled
            </label>

            <label class="checkbox">
                <input
                    id="sceneFavorite"
                    type="checkbox"
                    ${scene?.favorite ? "checked" : ""}>
                Favorite
            </label>

            <hr>

            <label class="checkbox">
                <input
                    id="sceneNotification"
                    type="checkbox"
                    ${scene?.notificationSend ? "checked" : ""}>
                Send Notification
            </label>

            <div class="form-group">
                <label>Notification Text</label>

                <input
                    id="sceneNotificationText"
                    class="textbox"
                    maxlength="63"
                    value="${scene?.notificationText ?? ""}">
            </div>
            <hr>
            <h3>Actions</h3>
            <div id="sceneActions"></div>
            <button class="btn" id="btnAddAction"> <i class="fa-solid fa-plus"></i> Add Action </button>
        `,

        onSave: async () => { return await saveScene(scene?.id); }
    });

    selectedSceneIcon = scene?.icon ?? "bolt";
    buildSceneIconPicker();
    sceneActions = structuredClone(scene?.actions ?? []);
    if(channels.length === 0) channels = await apiGet("/api/channels") ?? [];
    renderSceneActions();
    $("btnAddAction").onclick = () =>
    {
        sceneActions.push({
            channelId:0,
            state:false,
            delayMs:0,
            durationMs:0
        });    
        renderSceneActions();
    };

}
//==============================================================
let sceneActions = [];
//==============================================================
function renderSceneActions()
{
    const container = $("sceneActions");
    container.innerHTML = "";
    sceneActions.forEach((action,index)=>
    {
        const row = document.createElement("div");
        row.className = "scene-action-row";
        const options = channels.map(ch => `
            <option value="${ch.id}" ${Number(action.channelId)===Number(ch.id)?"selected":""}>
                ${ch.name}
            </option>
        `).join("");
        row.innerHTML = `
            <select class="textbox actionChannel"> ${options} </select>
            <select class="textbox actionState">
                <option value="1" ${action.state?"selected":""}>
                    ON
                </option>
                <option value="0" ${!action.state?"selected":""}>
                    OFF
                </option>
            </select>

            <div class="time-column">
                <small>Delay</small>
                <input class="textbox actionDelay" type="number" min="0" value="${action.delayMs}">            
            </div>

            <div class="time-column">
                <small>Duration</small>
                <input class="textbox actionDuration" type="number" min="0" value="${action.durationMs}">
            </div>

            <button class="btn danger actionDelete"> <i class="fa-solid fa-trash"></i> </button>
        `;

        row.querySelector(".actionChannel").onchange=e=>
        {
            action.channelId = Number(e.target.value);
        };

        row.querySelector(".actionState").onchange=e=>
        {
            action.state = Number(e.target.value)===1;
        };

        row.querySelector(".actionDuration").onchange=e=>
        {
            action.durationMs = Number(e.target.value);
        };
        row.querySelector(".actionDelay").onchange = e =>
        {
            action.delayMs = Number(e.target.value);
        };
        row.querySelector(".actionDelete").onclick=()=>
        {
            sceneActions.splice(index,1);
            renderSceneActions();
        };

        container.appendChild(row);
    });
}
//==============================================================
async function saveScene(id = null)
{
    const body =
    {
        name: $("sceneName").value.trim(),
        icon: selectedSceneIcon,
        enabled: $("sceneEnabled").checked,
        favorite: $("sceneFavorite").checked,

        notificationSend: $("sceneNotification").checked,
        notificationText: $("sceneNotificationText").value.trim(),

        actions: sceneActions
    };

    if(body.name === "")
    {
        showToast("Name is required","error");
        $("sceneName").focus();
        return false;
    }

    let result;

    if(id)
    {
        result = await apiPut(
            `/api/scenes?id=${id}`,
            body
        );
    }
    else
    {
        result = await apiPost(
            "/api/scenes",
            body
        );
    }

    if(!result || result.success === false)
    {
        showToast(
            result?.message ?? "Save failed",
            "error"
        );
        return false;
    }

    showToast("Saved","success");

    if(id)
    {
        updateSceneInList(
        {
            id,
            ...body,
            actions: sceneActions.length
        });
    }
    else
    {
        addSceneToList(
        {
            id: result.id,
            ...body,
            actions: sceneActions.length
        });
    }

    return true;
}
//==============================================================

//==============================================================

//==============================================================

//==============================================================

)rawliteral";

#endif
