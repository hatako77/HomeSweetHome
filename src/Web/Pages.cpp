#include "Web/Pages.h"
#include "Web/WebServerService.h"
#include "WebUI/index_html.h"
#include "WebUI/ota_html.h"
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
#include "Web/Assets/Js/Utils/IconsJs.h"
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

		auto& server = web.server();

		if(!LittleFS.begin(true))
		{
			Serial.println("LittleFS Mount Failed");
			return;
		}

		server.on("/", HTTP_GET,
		[](AsyncWebServerRequest* request)
		{
			request->send(
				200,
				"text/html",
				INDEX_HTML
			);
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

		server.on("/app.js",HTTP_GET,[](AsyncWebServerRequest* request)
		{
			AsyncResponseStream* response=request->beginResponseStream("application/javascript");		
			response->print(APP_JS);
			response->print(DOM_JS);
			response->print(API_JS);
			response->print(ICONS_JS);
			response->print(WEBSOCKET_JS);			
			response->print(SIDEBAR_JS);
			response->print(DIALOG_JS);
			response->print(MODAL_JS);
			response->print(TOAST_JS);
			response->print(LOADER_JS);			
			response->print(DASHBOARD_JS);
			response->print(ROOM_JS);
			response->print(SCENE_JS);
			response->print(SCHEDULE_JS);
			response->print(SENSOR_JS);
			response->print(OTA_JS);
			response->print(SETTINGS_JS);
			request->send(response);
		});

		server.on("/ota", HTTP_GET,
		[](AsyncWebServerRequest* request)
		{
			request->send(
				200,
				"text/html",
				OTA_HTML
			);
		});
	}
}
