#ifndef DOM_JS_H
#define DOM_JS_H

#include <pgmspace.h>

const char DOM_JS[] PROGMEM = R"rawliteral(

function $(id)
{
    return document.getElementById(id);
}

function create(tag, cls = "")
{
    const e = document.createElement(tag);

    if(cls)
        e.className = cls;

    return e;
}

function clear(el)
{
    while(el.firstChild)
        el.removeChild(el.firstChild);
}

function icon(name)
{
    return Icons[name] || Icons.generic;
}

function log()
{
    console.log.apply(console, arguments);
}

)rawliteral";

#endif
