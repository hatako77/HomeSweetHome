#ifndef OTA_JS_H
#define OTA_JS_H

#include <pgmspace.h>

const char OTA_JS[] PROGMEM = R"rawliteral(

async function initOTA()
{
    App.currentPage = "ota";

    $("content").innerHTML = `
    <h2>Firmware Update</h2>

    <div class="card">

        <div class="info-row">
            <span>Current Version</span>
            <b id="otaCurrent">-</b>
        </div>

        <div class="info-row">
            <span>Latest Version</span>
            <b id="otaRemote">-</b>
        </div>

        <div class="info-row">
            <span>Status</span>
            <b id="otaState">Idle</b>
        </div>

        <progress
            id="otaProgress"
            value="0"
            max="100"
            style="width:100%;margin-top:20px;">
        </progress>

        <div style="display:flex;justify-content:space-between;margin-top:12px;">
            <span id="otaPercent">0%</span>
            <span id="otaSpeed">0 KB/s</span>
            <span id="otaEta">ETA --</span>
        </div>

        <div style="margin-top:10px;text-align:center;">
            <span id="otaSize">0 / 0 MB</span>
        </div>

        <div
            id="otaMessage"
            style="margin-top:20px;text-align:center;font-weight:bold;">
            Ready
        </div>

        <div
            id="otaError"
            style="margin-top:10px;color:#ff4d4d;text-align:center;">
        </div>

        <div style="display:flex;gap:10px;margin-top:25px;">

            <button
                class="btn"
                id="otaCheckButton"
                onclick="checkOTA()">
                Check
            </button>

            <button
                class="btn"
                id="otaUpdateButton"
                onclick="startOTA()"
                disabled>
                Update
            </button>

        </div>

    </div>
    `;

    checkOTA();
}

function checkOTA()
{
    $("otaError").innerText = "";
    $("otaMessage").innerText = "Checking...";

    $("otaCheckButton").disabled = true;
    $("otaUpdateButton").disabled = true;

    wsSend("ota","check");
}

function startOTA()
{
    $("otaError").innerText = "";
    $("otaMessage").innerText = "Starting update...";

    $("otaCheckButton").disabled = true;
    $("otaUpdateButton").disabled = true;

    wsSend("ota","update");
}

function updateOTA(data)
{
    if(!data)
        return;

    if($("otaCurrent"))
        $("otaCurrent").innerText = data.current ?? "-";

    if($("otaRemote"))
        $("otaRemote").innerText = data.remote ?? "-";

    if($("otaState"))
        $("otaState").innerText = data.state ?? "-";

    if($("otaProgress"))
        $("otaProgress").value = data.percent ?? 0;

    if($("otaPercent"))
        $("otaPercent").innerText =
            (data.percent ?? 0) + "%";

    if($("otaSpeed"))
    {
        const speed = Number(data.speed ?? 0);
        $("otaSpeed").innerText =
            speed.toFixed(1) + " KB/s";
    }

    if($("otaEta"))
    {
        if((data.eta ?? 0) > 0)
            $("otaEta").innerText =
                "ETA " + data.eta + " s";
        else
            $("otaEta").innerText = "ETA --";
    }

    if($("otaSize"))
    {
        const downloaded =
            Number(data.downloaded ?? 0) / 1024 / 1024;

        const total =
            Number(data.total ?? 0) / 1024 / 1024;

        $("otaSize").innerText =
            downloaded.toFixed(2) +
            " / " +
            total.toFixed(2) +
            " MB";
    }

    if($("otaMessage"))
        $("otaMessage").innerText =
            data.state ?? "";

    if(data.error && $("otaError"))
        $("otaError").innerText = data.error;

    if(data.running)
    {
        $("otaCheckButton").disabled = true;
        $("otaUpdateButton").disabled = true;
        return;
    }

    if(data.state === "Update Available")
    {
        $("otaCheckButton").disabled = false;
        $("otaUpdateButton").disabled = false;
        $("otaMessage").innerText =
            "New firmware available.";
        return;
    }

    if(data.state === "Up To Date")
    {
        $("otaCheckButton").disabled = false;
        $("otaUpdateButton").disabled = true;
        $("otaMessage").innerText =
            "Firmware is already up to date.";
        return;
    }

    if(data.state === "Completed")
    {
        $("otaCheckButton").disabled = true;
        $("otaUpdateButton").disabled = true;

        let sec = 5;

        $("otaMessage").innerText =
            "Restarting in " + sec + "...";

        const timer = setInterval(() =>
        {
            sec--;

            $("otaMessage").innerText =
                "Restarting in " + sec + "...";

            if(sec <= 0)
            {
                clearInterval(timer);
            }

        },1000);

        return;
    }

    $("otaCheckButton").disabled = false;
}

)rawliteral";

#endif
