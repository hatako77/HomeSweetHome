#ifndef DOM_JS_H
#define DOM_JS_H

#include <pgmspace.h>

const char DOM_JS[] PROGMEM = R"rawliteral(

function $(id)
{
    return document.getElementById(id);
}

function $$(selector)
{
    return document.querySelector(selector);
}

function $$$ (selector)
{
    return document.querySelectorAll(selector);
}

function create(tag, cls = "")
{
    const element = document.createElement(tag);

    if(cls)
        element.className = cls;

    return element;
}

function clear(element)
{
    if(!element)
        return;

    element.innerHTML = "";
}

function show(element)
{
    if(element)
        element.style.display = "";
}

function hide(element)
{
    if(element)
        element.style.display = "none";
}

function text(element, value)
{
    if(element)
        element.innerText = value;
}

function html(element, value)
{
    if(element)
        element.innerHTML = value;
}

function on(element, event, callback)
{
    if(element)
        element.addEventListener(event, callback);
}

function icon(id, size = 22)
{
    const key = typeof id === "number"
        ? (iconMap[id] ?? "generic")
        : id;

    return `
        <span class="svg-icon" style="width:${size}px;height:${size}px">
            ${Icons[key] ?? Icons.generic}
        </span>
    `;
}

function log()
{
    console.log.apply(console, arguments);
}

)rawliteral";

#endif
