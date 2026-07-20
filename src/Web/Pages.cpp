#include "Web/Pages.h"
#include "Web/WebServerService.h"
#include "WebUI/index_html.h"
#include <LittleFS.h>

#include "Web/Assets/Css/BaseCss.h"
#include "Web/Assets/Css/LayoutCss.h"
#include "Web/Assets/Css/SidebarCss.h"
#include "Web/Assets/Css/CardCss.h"
#include "Web/Assets/Css/ButtonCss.h"
#include "Web/Assets/Css/FormCss.h"
#include "Web/Assets/Css/DashboardCss.h"
#include "Web/Assets/Css/RoomCss.h"
#include "Web/Assets/Css/TileCss.h"
#include "Web/Assets/Css/DialogCss.h"
#include "Web/Assets/Css/ToastCss.h"
#include "Web/Assets/Css/LoaderCss.h"
#include "Web/Assets/Css/OTACss.h"
#include "Web/Assets/Css/ResponsiveCss.h"

#include "Web/Assets/Js/StateJs.h"
#include "Web/Assets/Js/SortableJs.h"
#include "Web/Assets/Js/ChannelJs.h"
#include "Web/Assets/Js/RouterJs.h"
#include "Web/Assets/Js/AppJs.h"
#include "Web/Assets/Js/DashboardJs.h"
#include "Web/Assets/Js/RoomJs.h"
#include "Web/Assets/Js/SceneJs.h"
#include "Web/Assets/Js/ScheduleJs.h"
#include "Web/Assets/Js/SensorJs.h"
#include "Web/Assets/Js/OTAJs.h"
#include "Web/Assets/Js/SettingsJs.h"

#include "Web/Assets/Js/Utils/DomJs.h"
#include "Web/Assets/Js/Utils/ApiJs.h"
#include "Web/Assets/Js/Utils/IconJs.h"
#include "Web/Assets/Js/Utils/WebSocketJs.h"

#include "Web/Assets/Js/Components/SidebarJs.h"
#include "Web/Assets/Js/Components/DialogJs.h"
#include "Web/Assets/Js/Components/ModalJs.h"
#include "Web/Assets/Js/Components/ToastJs.h"
#include "Web/Assets/Js/Components/LoaderJs.h"

namespace Pages
{
void registerRoutes(WebServerService& web)
{
    Serial.println("Pages Register");

    auto& server=web.server();

    if(!LittleFS.begin(true))
    {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    server.on("/",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"text/html",INDEX_HTML);
    });

    server.on("/style.css",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        AsyncResponseStream* response=request->beginResponseStream("text/css");
        response->print(BASE_CSS);
        response->print(LAYOUT_CSS);
        response->print(SIDEBAR_CSS);
        response->print(CARD_CSS);
        response->print(BUTTON_CSS);
        response->print(FORM_CSS);
        response->print(DASHBOARD_CSS);
        response->print(ROOM_CSS);
        response->print(TILE_CSS);
        response->print(DIALOG_CSS);
        response->print(TOAST_CSS);
        response->print(LOADER_CSS);
        response->print(OTA_CSS);
        response->print(RESPONSIVE_CSS);
        request->send(response);
    });

    server.on("/js/libs/sortable.min.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",SORTABLE_JS);
    });
    server.on("/js/utils/icons.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",ICON_JS);
    });

    server.on("/js/utils/svgicons.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",SVG_ICONS);
    });

    server.on("/js/channel.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",CHANNEL_JS);
    });

    server.on("/js/state.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",STATE_JS);
    });

    server.on("/js/router.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",ROUTER_JS);
    });

    server.on("/js/app.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",APP_JS);
    });

    server.on("/js/dashboard.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",DASHBOARD_JS);
    });

    server.on("/js/room.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",ROOM_JS);
    });

    server.on("/js/scene.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",SCENE_JS);
    });

    server.on("/js/schedule.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",SCHEDULE_JS);
    });

    server.on("/js/sensor.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",SENSOR_JS);
    });

    server.on("/js/ota.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",OTA_JS);
    });

    server.on("/js/settings.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",SETTINGS_JS);
    });

    server.on("/js/utils/dom.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",DOM_JS);
    });

    server.on("/js/utils/api.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",API_JS);
    });

    server.on("/js/utils/icons.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",ICONS_JS);
    });

    server.on("/js/utils/websocket.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",WEBSOCKET_JS);
    });

    server.on("/js/components/sidebar.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",SIDEBAR_JS);
    });

    server.on("/js/components/dialog.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",DIALOG_JS);
    });

    server.on("/js/components/modal.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",MODAL_JS);
    });

    server.on("/js/components/toast.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",TOAST_JS);
    });

    server.on("/js/components/loader.js",HTTP_GET,[](AsyncWebServerRequest* request)
    {
        request->send(200,"application/javascript",LOADER_JS);
    });

}
}
