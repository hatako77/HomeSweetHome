#ifndef APP_JS_H
#define APP_JS_H

#include <pgmspace.h>

const char APP_JS[] PROGMEM = R"rawliteral(

async function loadVersion()
{
    try
    {
        const res = await fetch("/api/ota/version");
        const data = await res.json();

        const el = document.getElementById("version");

        if(el)
            el.innerText = "Version: " + data.current;
    }
    catch(e)
    {
        console.log(e);
    }
}

async function loadIO()
{
    try
    {
        const res = await fetch("/api/io");
        const list = await res.json();

        const container = document.getElementById("ioContainer");

        if(!container)
            return;

        container.innerHTML = "";

        list.forEach(io =>
        {
            const card = document.createElement("div");
            card.className = "card";

            card.innerHTML = `
                <div class="title">${io.name}</div>
                <div class="state">${io.state ? "ON" : "OFF"}</div>
            `;

            if(io.state)
                card.classList.add("on");

            card.onclick = async () =>
            {
                await fetch("/api/io/toggle?id=" + io.id,{
                    method:"POST"
                });

                loadIO();
            };

            container.appendChild(card);
        });
    }
    catch(e)
    {
        console.log(e);
    }
}

window.onload = function()
{
    loadVersion();
    loadIO();
};

)rawliteral";

#endif
