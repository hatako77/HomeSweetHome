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
            ${icon("home",30)}
            <span>خانه</span>
        </div>

        <div class="menu-item" data-page="channels">
            ${icon("channels",30)}
            <span>کانال ها</span>
        </div>

        <div class="menu-item" data-page="scenes">
            ${icon("scenes",30)}
            <span>سناریوها</span>
        </div>

        <div class="menu-item" data-page="schedules">
            ${icon("schedules",30)}
            <span>زمان بندی ها</span>
        </div>

        <div class="menu-item" data-page="sensors">
            ${icon("sensors",30)}
            <span>سنسورها</span>
        </div>

        <div class="menu-item" data-page="ota">
            ${icon("ota",30)}
            <span>بروزرسانی</span>
        </div>

        <div class="menu-item" data-page="settings">
            ${icon("settings",30)}
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
