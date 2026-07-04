#include "web.h"
#include "version.h"
#include "webpages.h"
#include "webpages/index.h"
#include "webpages/ota.h"
#include "webpages/style.h"
#include "webpages/script.h"

Web::Web() :
server(80)
{
}

void Web::begin()
{
    setupRoutes();
    server.begin();
}

void Web::handle()
{
    server.handleClient();
}

void Web::setupRoutes()
{
    server.on("/", HTTP_GET,
    [this]()
    {
        root();
    });

    server.on("/api/info", HTTP_GET,
    [this]()
    {
        apiInfo();
    });

    server.on("/app.css", HTTP_GET, [this]()
    {
        server.send_P(
            200,
            "text/css",
            STYLE_CSS
        );
    });
    
    server.on("/app.js", HTTP_GET, [this]()
    {
        server.send_P(
            200,
            "application/javascript",
            SCRIPT_JS
        );
    });
    
    server.on("/api/check", HTTP_GET,
    [this]()
    {
        apiCheck();
    });

    server.on("/api/update", HTTP_POST,
    [this]()
    {
        apiUpdate();
    });

    server.on("/api/progress", HTTP_GET,
    [this]()
    {
        apiProgress();
    });
}

void Web::root()
{
    server.send(
        200,
        "text/html",
        INDEX_HTML
    );
}

void Web::apiInfo()
{
    String json="{";

    json+="\"name\":\"";
    json+=FIRMWARE_NAME;
    json+="\",";

    json+="\"version\":\"";
    json+=FIRMWARE_VERSION;
    json+="\"";

    json+="}";

    server.send(
        200,
        "application/json",
        json
    );
}

void Web::apiCheck()
{
    String json="{";

    if(github.checkVersion())
    {
        json+="\"success\":true,";
        json+="\"latest\":\"";
        json+=github.latestVersion();
        json+="\"";
    }
    else
    {
        json+="\"success\":false";
    }

    json+="}";

    server.send(
        200,
        "application/json",
        json
    );
}

void Web::apiUpdate()
{
    bool ok=ota.begin(
        github.firmwareURL()
    );

    String json="{";

    json+="\"success\":";
    json+=ok?"true":"false";

    json+="}";

    server.send(
        200,
        "application/json",
        json
    );
}

void Web::apiProgress()
{
    String json="{";

    json+="\"progress\":";
    json+=String(
        ota.progress()
    );

    json+=",";

    json+="\"running\":";
    json+=ota.running()?
        "true":"false";

    json+=",";

    json+="\"status\":\"";
    json+=ota.status();
    json+="\"";

    json+="}";

    server.send(
        200,
        "application/json",
        json
    );
}
