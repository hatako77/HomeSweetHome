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

        const text = await response.text();        
        let result = null;        
        if(text !== "")
        {
            try
            {
                result = JSON.parse(text);
            }
            catch
            {
                result = text;
            }
        }
        
        if(!response.ok)
        {
            return result ??
            {
                success:false,
                message:response.statusText
            };
        }        
        return result;
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
