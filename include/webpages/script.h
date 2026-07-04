#pragma once

#include <Arduino.h>

const char SCRIPT_JS[] PROGMEM = R"rawliteral(

const $ = id => document.getElementById(id);

async function api(url, method = "GET")
{
    const response = await fetch(url,{
        method:method,
        cache:"no-store"
    });

    return await response.json();
}

function setText(id,text)
{
    const e=$(id);

    if(e)
        e.textContent=text;
}

function setProgress(value)
{
    const bar=$("progressBar");

    if(bar)
        bar.style.width=value+"%";

    setText("progressText",value+"%");
}

function setStatus(text)
{
    setText("status",text);
}

function disableUpdate(disable)
{
    const btn=$("btnUpdate");

    if(btn)
        btn.disabled=disable;
}

async function loadInfo()
{
    try
    {
        const data=await api("/api/info");

        setText("fwName",data.name);

        setText("fwVersion",data.version);
    }
    catch(e)
    {
        console.log(e);
    }
}

async function checkUpdate()
{
    setStatus("Checking GitHub...");

    disableUpdate(true);

    try
    {
        const data=await api("/api/check");

        if(!data.success)
        {
            setStatus("Unable to connect");

            return;
        }

        setText("latestVersion",data.latest);

        if(data.latest==$("fwVersion").textContent)
        {
            setStatus("You already have the latest version.");

            disableUpdate(true);
        }
        else
        {
            setStatus("New firmware available.");

            disableUpdate(false);
        }
    }
    catch(e)
    {
        setStatus("Communication Error");
    }
}

async function startUpdate()
{
    disableUpdate(true);

    setStatus("Starting OTA...");

    try
    {
        const data=await api("/api/update","POST");

        if(!data.success)
        {
            setStatus("Unable to start update.");

            disableUpdate(false);

            return;
        }

        monitorOTA();
    }
    catch(e)
    {
        setStatus("Update Error");

        disableUpdate(false);
    }
}

let otaTimer=null;

function monitorOTA()
{
    if(otaTimer)
        clearInterval(otaTimer);

    otaTimer=setInterval(async()=>{

        try
        {
            const data=await api("/api/progress");

            setProgress(data.progress);

            setStatus(data.status);

            if(!data.running)
            {
                clearInterval(otaTimer);

                if(data.progress==100)
                {
                    setStatus("Update Complete. Restarting...");
                }
            }

        }
        catch(e)
        {

        }

    },500);
}

window.onload=()=>{

    loadInfo();

    checkUpdate();

};

)rawliteral";
