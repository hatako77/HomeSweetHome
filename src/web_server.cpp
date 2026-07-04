#include "web_server.h"
#include "version.h"
#include "config.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>

extern OTAHandler otaHandler;

WebServer::WebServer() 
    : m_server(WEB_SERVER_PORT)
    , m_events("/events")
    , m_updateInProgress(false)
    , m_updateStatus("Ready")
    , m_updateProgress(0) {
}

WebServer::~WebServer() {
}

bool WebServer::begin() {
    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        DEBUG_PRINTLN("❌ SPIFFS Mount Failed");
        return false;
    }
    DEBUG_PRINTLN("✅ SPIFFS mounted successfully");
    
    // Setup routes
    setupRoutes();
    serveStaticFiles();
    
    // Add event source handler
    m_server.addHandler(&m_events);
    
    // Start server
    m_server.begin();
    DEBUG_PRINTLN("✅ Web Server started on port " + String(WEB_SERVER_PORT));
    DEBUG_PRINT("🌐 URL: http://");
    DEBUG_PRINTLN(WiFi.localIP());
    
    return true;
}

void WebServer::setupRoutes() {
    // Main pages
    m_server.on("/", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->handleRoot(request);
    });
    
    m_server.on("/ota", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->handleOTA(request);
    });
    
    // API endpoints
    m_server.on("/api/version", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->handleVersionCheck(request);
    });
    
    m_server.on("/api/update", HTTP_POST, [this](AsyncWebServerRequest* request) {
        this->handleStartUpdate(request);
    });
    
    m_server.on("/api/restart", HTTP_POST, [this](AsyncWebServerRequest* request) {
        this->handleRestart(request);
    });
    
    // Upload endpoint
    m_server.on("/api/upload", HTTP_POST, 
        [](AsyncWebServerRequest* request) {},
        [this](AsyncWebServerRequest* request, 
               const String& filename, 
               size_t index, 
               uint8_t* data, 
               size_t len, 
               bool final) {
            this->handleUpload(request, filename, index, data, len, final);
        }
    );
    
    // Event source
    m_events.onConnect([this](AsyncEventSourceClient* client) {
        this->handleEvents(client);
    });
    
    // 404 handler
    m_server.onNotFound([this](AsyncWebServerRequest* request) {
        this->handleNotFound(request);
    });
}

void WebServer::serveStaticFiles() {
    // Serve static files from SPIFFS
    m_server.serveStatic("/css/", SPIFFS, "/css/");
    m_server.serveStatic("/js/", SPIFFS, "/js/");
    m_server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
}

void WebServer::handleRoot(AsyncWebServerRequest* request) {
    if (!isAuthenticated(request)) {
        request->send(401);
        return;
    }
    request->send(SPIFFS, "/index.html", "text/html");
}

void WebServer::handleOTA(AsyncWebServerRequest* request) {
    if (!isAuthenticated(request)) {
        request->send(401);
        return;
    }
    request->send(SPIFFS, "/ota.html", "text/html");
}

void WebServer::handleVersionCheck(AsyncWebServerRequest* request) {
    DynamicJsonDocument doc(256);
    doc["current"] = FIRMWARE_VERSION;
    doc["latest"] = otaHandler.getLatestVersion();
    doc["updateAvailable"] = otaHandler.isUpdateAvailable();
    doc["status"] = otaHandler.getStatusMessage();
    doc["progress"] = otaHandler.getProgress();
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

void WebServer::handleStartUpdate(AsyncWebServerRequest* request) {
    if (!otaHandler.isUpdateAvailable()) {
        DynamicJsonDocument doc(128);
        doc["success"] = false;
        doc["error"] = "No update available";
        String response;
        serializeJson(doc, response);
        request->send(400, "application/json", response);
        return;
    }
    
    if (m_updateInProgress) {
        DynamicJsonDocument doc(128);
        doc["success"] = false;
        doc["error"] = "Update already in progress";
        String response;
        serializeJson(doc, response);
        request->send(409, "application/json", response);
        return;
    }
    
    m_updateInProgress = true;
    bool success = otaHandler.startUpdate(
        "http://" + String(OTA_HOST) + OTA_PATH + OTA_FIRMWARE_FILE
    );
    m_updateInProgress = false;
    
    DynamicJsonDocument doc(128);
    doc["success"] = success;
    if (!success) {
        doc["error"] = otaHandler.getStatusMessage();
    }
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
    
    if (success) {
        delay(1000);
        ESP.restart();
    }
}

void WebServer::handleUpload(AsyncWebServerRequest* request, 
                            const String& filename, 
                            size_t index, 
                            uint8_t* data, 
                            size_t len, 
                            bool final) {
    if (!isAuthenticated(request)) {
        request->send(401);
        return;
    }
    
    if (m_updateInProgress) {
        request->send(409, "application/json", "{\"success\":false,\"error\":\"Update in progress\"}");
        return;
    }
    
    if (!filename.endsWith(".bin")) {
        request->send(400, "application/json", "{\"success\":false,\"error\":\"Invalid file type\"}");
        return;
    }
    
    m_updateInProgress = true;
    bool success = otaHandler.processManualUpdate(data, len, final);
    
    if (final) {
        m_updateInProgress = false;
        DynamicJsonDocument doc(128);
        doc["success"] = success;
        if (!success) {
            doc["error"] = otaHandler.getStatusMessage();
        }
        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
        
        if (success) {
            if (m_onUpdateComplete) {
                m_onUpdateComplete();
            }
            delay(1000);
            ESP.restart();
        }
    } else {
        // Send progress
        DynamicJsonDocument doc(64);
        doc["success"] = true;
        doc["progress"] = otaHandler.getProgress();
        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    }
}

void WebServer::handleRestart(AsyncWebServerRequest* request) {
    request->send(200, "application/json", "{\"success\":true}");
    delay(500);
    ESP.restart();
}

void WebServer::handleNotFound(AsyncWebServerRequest* request) {
    request->send(404, "text/plain", "Not Found");
}

void WebServer::handleEvents(AsyncEventSourceClient* client) {
    DEBUG_PRINTLN("✅ EventSource client connected");
    client->send("{\"type\":\"connected\",\"message\":\"Connected to event stream\"}", "message", millis());
}

void WebServer::sendUpdateProgress(int progress, const String& status) {
    m_updateProgress = progress;
    m_updateStatus = status;
    
    DynamicJsonDocument doc(256);
    doc["type"] = (progress >= 100) ? "complete" : "progress";
    doc["progress"] = progress;
    doc["status"] = status;
    if (progress >= 100) {
        doc["message"] = "Update completed successfully!";
    }
    
    String json;
    serializeJson(doc, json);
    m_events.send(json.c_str(), "message", millis());
}

String WebServer::getContentType(const String& filename) {
    if (filename.endsWith(".html")) return "text/html";
    if (filename.endsWith(".css")) return "text/css";
    if (filename.endsWith(".js")) return "application/javascript";
    if (filename.endsWith(".json")) return "application/json";
    if (filename.endsWith(".png")) return "image/png";
    if (filename.endsWith(".jpg")) return "image/jpeg";
    if (filename.endsWith(".ico")) return "image/x-icon";
    return "text/plain";
}

bool WebServer::isAuthenticated(AsyncWebServerRequest* request) {
    // Simple authentication - can be extended
    return true;
}
