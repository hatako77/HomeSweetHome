#ifndef API_JS_H
#define API_JS_H

#include <pgmspace.h>

const char API_JS[] PROGMEM = R"rawliteral(

async function api(url, options = {})
{
    try
    {
        const response = await fetch(url, options);

        if(!response.ok)
            throw new Error(response.status);

        const text = await response.text();

        if(text.length === 0)
            return null;

        try
        {
            return JSON.parse(text);
        }
        catch
        {
            return text;
        }
    }
    catch(error)
    {
        console.error("API ERROR:", url, error);

        return null;
    }
}

async function apiGet(url)
{
    return await api(url);
}

async function apiPost(url, body = null)
{
    const options =
    {
        method: "POST"
    };

    if(body != null)
    {
        options.headers =
        {
            "Content-Type":"application/json"
        };

        options.body = JSON.stringify(body);
    }

    return await api(url, options);
}

)rawliteral";

#endif
