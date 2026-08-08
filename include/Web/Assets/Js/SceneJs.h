#ifndef SCENE_JS_H
#define SCENE_JS_H

#include <pgmspace.h>

const char SCENE_JS[] PROGMEM=R"rawliteral(

let scenes=[];

function updateSceneProgress(items)
{
    if(!Array.isArray(items))
        return;

    items.forEach(item =>
    {
        updateSceneCard(item);
    });
}
//==============================================================
async function initScenes()
{
    const data = await apiGet("/api/scenes");
    if(!Array.isArray(data)) return;
    scenes = data;
    renderScenes();
    updateRunningScenesBar();
}
//==============================================================
function updateSceneCard(scene)
{
    const card = document.querySelector(
        `[data-scene-id="${scene.id}"]`
    );

    if(!card)
        return;

    //-------------------------------------------------
    // Progress
    //-------------------------------------------------

    const progressBar =
        card.querySelector(".scene-progress-bar");

    if(progressBar)
        progressBar.style.width =
            `${scene.progress}%`;

    const progressText =
        card.querySelector(".scene-progress-text");

    if(progressText)
        progressText.textContent =
            `${scene.progress}%`;

    //-------------------------------------------------
    // Status
    //-------------------------------------------------

    const status =
        card.querySelector(".scene-status");

    if(status)
    {
        if(scene.running)
        {
            status.classList.add("running");

            status.innerHTML =
                `<i class="fa-solid fa-circle"></i> Running`;
        }
        else
        {
            status.classList.remove("running");

            status.innerHTML = "Idle";
        }
    }

    //-------------------------------------------------
    // بروزرسانی آبجکت داخل آرایه
    //-------------------------------------------------

    const s = findScene(scene.id);

    if(s)
    {
        s.progress = scene.progress;
        s.running = scene.running;
    }
}
//==============================================================
function showScenes()
{
    $("content").innerHTML = `
            <button class="btn primary" style="z-index: 999;border-radius: 5rem;position: fixed;bottom: 2rem;left: 2rem;height: 4rem;width: 4rem;"
                    onclick="newScene()">
                    <span class="svg-icon" style="width:24px;height:24px">                                
                        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round">
                            <path d="M12 5v14"></path>
                            <path d="M5 12h14"></path>
                        </svg>
                    </span>
            </button>
        <div id="sceneList" style="margin-bottom: 6rem;" class="card-list"></div>
    `;
    initScenes();
}
//==============================================================
function renderScenes()
{
    if(App.currentPage !== "scenes") return;
    const list = $("sceneList");
    if(!list) return;
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
function removeSceneFromList(id)
{
    scenes = scenes.filter(x => Number(x.id) !== Number(id));
    renderScenes();
}
//==============================================================
function createSceneCard(scene)
{
    const card = create("div","card");
    card.dataset.sceneId = scene.id;
    card.innerHTML = `
        <div class="card-header">
            <div class="card-title">
                <i class="fa-solid fa-${scene.icon ?? "bolt"}"></i>
                <span>${scene.name}</span>
            </div>
            <div class="card-actions">
                <button class="icon-btn"
                        onclick="editScene(${scene.id})">
                    ${icon("edit",20)}
                </button>
                <button class="icon-btn danger"
                        onclick="deleteScene(${scene.id})">
                    ${icon("delete",20)}
                </button>
            </div>
        </div>
        <div class="card-body">
            <div class="scene-info">
            
                <div class="scene-status status ${scene.running ? "running" : ""}">
                    ${
                        scene.running
                        ? `<span class="status running">
                                <i class="fa-solid fa-circle"></i>
                                Running
                           </span>`
                        : `<span class="status idle">
                                Idle
                           </span>`
                    }
                </div>
            
                <div class="scene-progress">
                    <div class="scene-progress-track">
                        <div class="scene-progress-bar"
                             style="width:${scene.progress ?? 0}%">
                        </div>
                    </div>
            
                        <small class="scene-progress-text">
                            ${scene.progress ?? 0}%
                        </small>
                    </div>
            
                <div class="scene-actions-count">
                    Actions : ${scene.actionCount}
                </div>
            
            </div>
            <div>
                ${scene.enabled ? "Enabled" : "Disabled"}
            </div>
        </div>
                <button class="btn btn-secondary" onclick="executeScene(${scene.id})">
                    اجرای سناریو
                    <span style="height: 30px;">                    
                        <svg xmlns="http://www.w3.org/2000/svg" width="800px" height="800px" viewBox="0 0 24 24" fill="none" style="width: 100%;height: 100%;">
                            <path fill-rule="evenodd" clip-rule="evenodd" d="M12 22C17.5228 22 22 17.5228 22 12C22 6.47715 17.5228 2 12 2C6.47715 2 2 6.47715 2 12C2 17.5228 6.47715 22 12 22ZM10.6935 15.8458L15.4137 13.059C16.1954 12.5974 16.1954 11.4026 15.4137 10.941L10.6935 8.15419C9.93371 7.70561 9 8.28947 9 9.21316V14.7868C9 15.7105 9.93371 16.2944 10.6935 15.8458Z" fill="white"></path>
                        </svg>
                    </span>
                </button>

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
    const result = await apiPost(
        `/api/scenes?action=execute&id=${id}`,
        {}
    );

    console.log("test");

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
            <div class="form-group" style="flex-direction: row;">
                <label>Name</label>
                <input
                    id="sceneName"
                    class="textbox"
                    value="${scene?.name ?? ""}">
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


            <div style="display: flex;gap: 1rem;">
                <label class="checkbox">
                    <input id="sceneNotification" type="checkbox" ${scene?.notificationSend ? "checked" : ""}>اعلان
                </label>
                <input id="sceneNotificationText" class="textbox" maxlength="63" value="${scene?.notificationText ?? ""}">
            </div>

            <div class="form-group">
                <label>Icon</label>
                <div id="sceneIconPicker"
                     class="icon-picker">
                </div>
            </div>

            <hr>
            <div style="display: flex;align-items: center;justify-content: space-between;padding: 1rem;">
                <h3>اکشن ها</h3>
                <button class="btn" id="btnAddAction" style="border-radius: 5rem;height: 4rem;width: 4rem;"> 
                    <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round">
                    <path d="M12 5v14"></path>
                    <path d="M5 12h14"></path>
                    </svg>
                </button>
            </div>
            <div id="sceneActions"></div>
        `,

        onSave: async () => { return await saveScene(scene?.id); }
    });

    selectedSceneIcon = scene?.icon ?? "bolt";
    //buildSceneIconPicker();
    sceneActions = structuredClone(scene?.actions ?? []);
    if(channels.length === 0) channels = await apiGet("/api/channels") ?? [];
    renderSceneActions();
    $("btnAddAction").onclick = () =>
    {
        sceneActions.push({
            channelId: channels.length ? channels[0].id : 0,
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
        const options = (channels ?? []).map(ch => `
            <option value="${ch.id}" ${Number(action.channelId)===Number(ch.id)?"selected":""}>
                ${ch.name}
            </option>
        `).join("");
        row.innerHTML = `
            
        <div style="display: flex">
            <select class="textbox actionChannel"> ${options} </select>
            <select style="width: auto; margin-right: 1rem;" class="textbox actionState">
                <option value="1" ${action.state?"selected":""}>
                    ON
                </option>
                <option value="0" ${!action.state?"selected":""}>
                    OFF
                </option>
            </select>
        </div>
        <div style="display: grid;gap: 1rem;margin: 1rem 0;">
            <div class="time-column">
                <small>تاخیر</small>
                <input class="textbox actionDelay" type="number" min="0" value="${action.delayMs}">            
            </div>

            <div class="time-column">
                <small>مدت</small>
                <input class="textbox actionDuration" type="number" min="0" value="${action.durationMs}">
            </div>
        </div>


            <button class="btn btn-danger actionDelete"> ${icon("delete",30)} </button>
        `;

        row.querySelector(".actionChannel").oninput=e=>
        {
            action.channelId = Number(e.target.value);
        };

        row.querySelector(".actionState").oninput=e=>
        {
            action.state = Number(e.target.value)===1;
        };

        row.querySelector(".actionDuration").oninput=e=>
        {
            action.durationMs = Number(e.target.value) || 0;
        };
        row.querySelector(".actionDelay").oninput = e =>
        {
            action.delayMs = Number(e.target.value) || 0;
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
    //--------------------------------------------------
    // Validation
    //--------------------------------------------------
    if(body.name === "")
    {
        showToast("Name is required","error");
        $("sceneName").focus();
        return false;
    }

    if(body.actions.length === 0)
    {
        showToast("At least one action is required","error");
        return false;
    }
    for(const action of body.actions)
    {
        if(Number(action.channelId) <= 0)
        {
            showToast("Select channel for all actions","error");
            return false;
        }
        action.channelId  = Number(action.channelId);
        action.delayMs    = Number(action.delayMs)    || 0;
        action.durationMs = Number(action.durationMs) || 0;
        action.state = Number(action.state) === 1 || action.state === true;
    }
    //--------------------------------------------------
    // Send request
    //--------------------------------------------------
    let result;
    if(id)
    {
        result = await apiPut(`/api/scenes?id=${id}`,body);
    }
    else
    {
        result = await apiPost("/api/scenes",body);
    }

    if(!result || result.success === false)
    {
        showToast(result?.message ?? "Save failed","error");
        return false;
    }
    //--------------------------------------------------
    // Refresh list
    //--------------------------------------------------
    await initScenes();
    showToast("Scene saved","success");
    Dialog.close();
    return true;
}
//==============================================================

//==============================================================

//==============================================================

//==============================================================

)rawliteral";

#endif
