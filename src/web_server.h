#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESPAsyncWebServer.h>
#include <functional>
#include "ota_handler.h"

class WebServer {
public:
    using UpdateCallback = std::function<void(int progress, const String& status)>;
    using CompleteCallback = std::function<void()>;

    WebServer();
    ~WebServer();

    bool begin();
    void sendUpdateProgress(int progress, const String& status);
    void setUpdateCompleteCallback(CompleteCallback callback) { m_onUpdateComplete = callback; }

private:
    AsyncWebServer m_server;
    AsyncEventSource m_events;
    CompleteCallback m_onUpdateComplete;
    
    bool m_updateInProgress;
    String m_updateStatus;
    int m_updateProgress;
    
    void setupRoutes();
    void serveStaticFiles();
    void handleRoot(AsyncWebServerRequest* request);
    void handleOTA(AsyncWebServerRequest* request);
    void handleVersionCheck(AsyncWebServerRequest* request);
    void handleStartUpdate(AsyncWebServerRequest* request);
    void handleUpload(AsyncWebServerRequest* request, 
                     const String& filename, 
                     size_t index, 
                     uint8_t* data, 
                     size_t len, 
                     bool final);
    void handleRestart(AsyncWebServerRequest* request);
    void handleNotFound(AsyncWebServerRequest* request);
    void handleEvents(AsyncEventSourceClient* client);
    
    String getContentType(const String& filename);
    bool isAuthenticated(AsyncWebServerRequest* request);
};

#endif
