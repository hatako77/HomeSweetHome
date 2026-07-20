#ifndef SIDEBAR_JS_H
#define SIDEBAR_JS_H

#include <pgmspace.h>

const char SIDEBAR_JS[] PROGMEM = R"rawliteral(

function initSidebar()
{
    const sidebar = $("sidebar");

    if(!sidebar)
        return;

    sidebar.innerHTML = `
    <div class="sidebar-menu">

        <div class="menu-item active" data-page="dashboard">
            <i class="fa-solid fa-gauge"></i>
            <span>Dashboard</span>
        </div>

        <div class="menu-item" data-page="rooms">
            <i class="fa-solid fa-house"></i>
            <span>Rooms</span>
        </div>
        <div class="menu-item" data-page="channels">
            <i class="fa-solid fa-lightbulb"></i>
            <span>Channels</span>
        </div>
        <div class="menu-item" data-page="scenes">
            <i class="fa-solid fa-wand-magic-sparkles"></i>
            <span>Scenes</span>
        </div>

        <div class="menu-item" data-page="schedules">
            <i class="fa-solid fa-clock"></i>
            <span>Schedules</span>
        </div>

        <div class="menu-item" data-page="sensors">
            <i class="fa-solid fa-temperature-half"></i>
            <span>Sensors</span>
        </div>

        <div class="menu-item" data-page="ota">
            <i class="fa-solid fa-upload"></i>
            <span>OTA</span>
        </div>

        <div class="menu-item" data-page="settings">
            <i class="fa-solid fa-gear"></i>
            <span>Settings</span>
        </div>

    </div>
    `;

    bindSidebar();
}

function bindSidebar()
{
    $$$(".menu-item").forEach(button =>
    {
        button.onclick = () =>
        {
            setActiveMenu(button.dataset.page);

            Router.navigate(button.dataset.page);
        };
    });
}

function setActiveMenu(page)
{
    $$$(".menu-item").forEach(item =>
    {
        item.classList.toggle(
            "active",
            item.dataset.page === page
        );
    });
}

)rawliteral";

#endif
