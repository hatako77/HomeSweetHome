#ifndef API_JS_H
#define API_JS_H

#include <pgmspace.h>

const char API_JS[] PROGMEM = R"rawliteral(

async function api(url, options = {})
{
    try
    {
        const response = await fetch(url,
        {
            cache: "no-store",
            ...options
        });

        if(response.status === 401)
        {
            throw new Error("Unauthorized");
        }

        if(!response.ok)
        {
            throw new Error(response.statusText || response.status);
        }

        const text = await response.text();

        if(text === "")
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

        if(typeof showToast === "function")
        {
            showToast("Connection error", "error");
        }

        return null;
    }
}

async function apiGet(url)
{
    return api(url,
    {
        method: "GET"
    });
}

async function apiPost(url, body = null)
{
    const options =
    {
        method: "POST",
        headers:
        {
            "Content-Type":"application/json"
        }
    };

    if(body != null)
    {
        options.body = JSON.stringify(body);
    }

    return api(url, options);
}

async function apiPut(url, body = null)
{
    const options =
    {
        method: "PUT",
        headers:
        {
            "Content-Type":"application/json"
        }
    };

    if(body != null)
    {
        options.body = JSON.stringify(body);
    }

    return api(url, options);
}

async function apiDelete(url)
{
    return api(url,
    {
        method: "DELETE"
    });
}

)rawliteral";

#endif
