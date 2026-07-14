#include "Web/Pages.h"
#include "Web/WebServerService.h"

#include "WebUI/index_html.h"
#include "WebUI/ota_html.h"

#include "Web/Assets/Css/Css.h"
#include "Web/Assets/Js/Js.h"

#include <LittleFS.h>

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

		server.on("/style.css", HTTP_GET,
		[](AsyncWebServerRequest* request)
		{
			request->send(
				200,
				"text/css",
				CSS
			);
		});

		server.on("/app.js", HTTP_GET,
		[](AsyncWebServerRequest* request)
		{
			request->send(
				200,
				"application/javascript",
				JS
			);
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
