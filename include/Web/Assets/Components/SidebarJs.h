#ifndef SIDEBAR_JS_H
#define SIDEBAR_JS_H

#include <pgmspace.h>

const char SIDEBAR_JS[] PROGMEM = R"rawliteral(

function loadSidebar()
{
    document.getElementById("sidebar").innerHTML = `
        <div class="sidebar-menu">

            <div class="menu-item active" data-page="dashboard">
                <i class="fa-solid fa-gauge"></i>
                <span>Dashboard</span>
            </div>

            <div class="menu-item" data-page="rooms">
                <i class="fa-solid fa-house"></i>
                <span>Rooms</span>
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
    document.querySelectorAll(".menu-item").forEach(button =>
    {
        button.onclick = function()
        {
            document.querySelectorAll(".menu-item")
                .forEach(x => x.classList.remove("active"));

            this.classList.add("active");

            switch(this.dataset.page)
            {
                case "dashboard":
                    showDashboard();
                    break;

                case "rooms":
                    showRooms();
                    break;

                case "scenes":
                    showScenes();
                    break;

                case "schedules":
                    showSchedules();
                    break;

                case "sensors":
                    showSensors();
                    break;

                case "ota":
                    showOTA();
                    break;

                case "settings":
                    showSettings();
                    break;
            }
        };
    });
}

)rawliteral";

#endif
