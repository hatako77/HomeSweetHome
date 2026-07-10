#include "Web/ApiScene.h"
#include <ArduinoJson.h>
#include "Core/SceneManager.h"
#include "Web/Message.h"
#include "Web/WebSocketService.h"

extern SceneManager sceneManager;
extern WebSocketService websocket;

void ApiScene::registerRoutes(AsyncWebServer& server)
{
  
  server.on("/api/scenes", HTTP_GET, [](AsyncWebServerRequest *request)
  {
      JsonDocument doc;  
      JsonArray array = doc.to<JsonArray>();  
      for(uint16_t i=0; i<sceneManager.count(); i++)
      {
          const Scene* scene = sceneManager.getAt(i);  
          if(scene == nullptr) continue;  
          JsonObject obj = array.add<JsonObject>();  
          obj["id"] = scene->id;  
          obj["name"] = scene->name;  
          obj["icon"] = scene->icon;  
          obj["favorite"] = scene->favorite; 
          obj["enabled"] = scene->enabled;  
          obj["actions"] = scene->actionCount;
      }  
      String json;  
      serializeJson(doc, json);  
      request->send(200, "application/json", json);
  });


  
  server.on("/api/scenes", HTTP_GET,[](AsyncWebServerRequest* request)
  {
      JsonDocument doc;  
      JsonArray array = doc.to<JsonArray>();  
      for(uint16_t i=0;i<sceneManager.count();i++)
      {
          const Scene* scene = sceneManager.getAt(i);  
          if(scene==nullptr) continue;  
          JsonObject obj = array.add<JsonObject>(); 
          obj["id"] = scene->id;
          obj["name"] = scene->name;
          obj["icon"] = scene->icon;
          obj["favorite"] = scene->favorite;
          obj["enabled"] = scene->enabled;
          obj["actions"] = scene->actionCount;
      }  
      String json;  
      serializeJson(doc,json);  
      request->send(200, "application/json", json);
  }); 



  
  server.on("/api/scenes/run", HTTP_POST,[](AsyncWebServerRequest* request)
  {
      if(!request->hasParam("id", true))
      {
          request->send(400);  
          return;
      }  
      uint16_t id = request
          ->getParam("id",true)
          ->value()
          .toInt();  
      bool ok = sceneManager.execute(id);  
      request->send(
          ok ? 200 : 404,
          "application/json",
          ok
          ? "{\"success\":true}"
          : "{\"success\":false}"
      );
  });




  
  server.on("/api/scenes/get", HTTP_GET, [](AsyncWebServerRequest *request)
  {
      if(!request->hasParam("id"))
      {
          request->send(400);
          return;
      }  
      uint16_t id = request->getParam("id")->value().toInt();  
      Scene* scene = sceneManager.get(id);  
      if(scene == nullptr)
      {
          request->send(404);
          return;
      }  
      JsonDocument doc;  
      doc["id"] = scene->id;
      doc["name"] = scene->name;
      doc["icon"] = scene->icon;
      doc["favorite"] = scene->favorite;
      doc["enabled"] = scene->enabled;  
      JsonArray actions = doc["actions"].to<JsonArray>();  
      for(uint8_t i=0;i<scene->actionCount;i++)
      {
          JsonObject a = actions.add<JsonObject>();  
          a["channelId"] = scene->actions[i].channelId;  
          a["state"] = scene->actions[i].state;  
          a["delayMs"] = scene->actions[i].delayMs;
      }  
      String json;  
      serializeJson(doc,json);  
      request->send(
          200,
          "application/json",
          json
      );
  });


  







  
}
