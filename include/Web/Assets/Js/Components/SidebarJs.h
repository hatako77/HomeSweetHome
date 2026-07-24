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

        <div class="menu-item" data-page="rooms">
            ${icon("rooms",22)}
            <span>خانه</span>
        </div>

        <div class="menu-item" data-page="channels">
            ${icon("channels",22)}
            <span>کانال ها</span>
        </div>

        <div class="menu-item" data-page="scenes">
            ${icon("سناریوها",22)}
            <span>Scenes</span>
        </div>

        <div class="menu-item" data-page="schedules">
            ${icon("schedules",22)}
            <span>زمان بندی ها</span>
        </div>

        <div class="menu-item" data-page="sensors">
            ${icon("sensors",22)}
            <span>سنسورها</span>
        </div>

        <div class="menu-item" data-page="ota">
            ${icon("ota",22)}
            <span>بروزرسانی</span>
        </div>

        <div class="menu-item" data-page="settings">
            ${icon("settings",22)}
            <span>تنظیمات</span>
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
