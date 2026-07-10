#include "Web/ApiScene.h"
#include <ArduinoJson.h>
#include "Core/SceneManager.h"
#include "Web/Message.h"
#include "Web/WebSocketService.h"

extern SceneManager sceneManager;
extern WebSocketService websocket;

void ApiScene::registerRoutes(AsyncWebServer& server)
{

  server.on("/api/scenes/remove",HTTP_POST,[](AsyncWebServerRequest *request)
  {
      if(!request->hasParam("id", true))
      {
          request->send(400,"application/json","{\"success\":false}");
          return;
      }  
      uint16_t id =request->getParam("id", true)->value().toInt();  
      bool ok = sceneManager.remove(id);  
      request->send(ok ? 200 : 404,"application/json",ok? "{\"success\":true}": "{\"success\":false}");
  });


  
  server.on("/api/scenes/get", HTTP_GET,[](AsyncWebServerRequest *request)
    {
    if(!request->hasParam("id"))
      {
          request->send(400,"application/json","{\"success\":false}");
          return;
      }  
      uint16_t id = request->getParam("id")->value().toInt();  
      const Scene* scene = sceneManager.get(id);  
      if(scene == nullptr)
      {
          request->send(404, "application/json", "{\"success\":false}");
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
      request->send(200,"application/json",json);
  });

  


  server.on("/api/scenes/save",HTTP_POST,[](AsyncWebServerRequest *request){},nullptr,[](AsyncWebServerRequest *request,
  uint8_t *data, size_t len, size_t index, size_t total)
  {
      JsonDocument doc;  
      DeserializationError err = deserializeJson(doc, data, len);  
      if(err)
      {
          request->send(400,"application/json","{\"success\":false}");
          return;
      }  
      Scene scene;  
      scene.id = doc["id"] | 0; 
      scene.name = doc["name"] | ""; 
      scene.icon = doc["icon"] | "bolt";  
      scene.favorite = doc["favorite"] | false;  
      scene.enabled = doc["enabled"] | true;  
      scene.actionCount = 0;  
      JsonArray actions = doc["actions"].as<JsonArray>();  
      for(JsonObject a : actions)
      {
          if(scene.actionCount >= Scene::MAX_ACTIONS) break;  
          SceneAction& action = scene.actions[scene.actionCount++];  
          action.channelId = a["channelId"] | 0;  
          action.state = a["state"] | false;  
          action.delayMs = a["delayMs"] | 0;
      }  
      bool ok = sceneManager.saveScene(scene);  
      request->send(ok ? 200 : 500,"application/json", ok ? "{\"success\":true}" : "{\"success\":false}");
  });




  
  
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
