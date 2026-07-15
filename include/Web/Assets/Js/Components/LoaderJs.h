#ifndef LOADER_JS_H
#define LOADER_JS_H

#include <pgmspace.h>

const char LOADER_JS[] PROGMEM = R"rawliteral(

function initLoader()
{
    if($("loader"))
        return;

    const loader = create("div");
    loader.id = "loader";

    loader.innerHTML = `
        <div class="loader-box">
            <div class="loader-spinner"></div>
            <div id="loaderText">Loading...</div>
        </div>
    `;

    document.body.appendChild(loader);

    hideLoader();
}

function showLoader(text = "Loading...")
{
    const loader = $("loader");

    if(!loader)
        return;

    loader.style.display = "flex";

    setLoaderText(text);
}

function hideLoader()
{
    const loader = $("loader");

    if(loader)
        loader.style.display = "none";
}

function setLoaderText(text)
{
    const label = $("loaderText");

    if(label)
        label.innerText = text;
}

)rawliteral";

#endif
