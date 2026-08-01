#include "Web/ApiScene.h"
#include <ArduinoJson.h>
#include "Core/SceneManager.h"
#include "Web/Message.h"
#include "Web/WebSocketService.h"

extern SceneManager sceneManager;
extern WebSocketService websocket;

void ApiScene::registerRoutes(WebServerService& server)
{
    AsyncWebServer& web = server.server();
    web.on("/api/scenes", HTTP_GET,[](AsyncWebServerRequest *request)
    {
        // GET /api/scenes?id=123
        if(request->hasParam("id"))
        {
            uint16_t id = request->getParam("id")->value().toInt();  
            const Scene* scene = sceneManager.get(id);  
            if(!scene)
            {
                request->send(404,"application/json","{\"success\":false,\"message\":\"Scene not found\"}");
                return;
            }  
            JsonDocument doc;  
            doc["id"] = scene->id;
            doc["name"] = scene->name;
            doc["icon"] = scene->icon;
            doc["favorite"] = scene->favorite;
            doc["enabled"] = scene->enabled;
            doc["notificationSend"] = scene->notificationSend;
            doc["notificationText"] = scene->notificationText;  
            JsonArray actions = doc["actions"].to<JsonArray>();  
            for(uint8_t i = 0; i < scene->actionCount; i++)
            {
                JsonObject a = actions.add<JsonObject>();  
                a["channelId"]  = scene->actions[i].channelId;
                a["state"]      = scene->actions[i].state;
                a["durationMs"] = scene->actions[i].durationMs;
                a["delayMs"]    = scene->actions[i].delayMs;
            }  
            String json;
            serializeJson(doc, json);  
            request->send(200, "application/json", json);
            return;
        }  
        // GET /api/scenes
        JsonDocument doc;
        JsonArray array = doc.to<JsonArray>();  
        for(uint16_t i = 0; i < sceneManager.count(); i++)
        {
            const Scene* scene = sceneManager.getAt(i);  
            if(!scene)continue;  
            JsonObject obj = array.add<JsonObject>();  
            obj["id"] = scene->id;
            obj["name"] = scene->name;
            obj["icon"] = scene->icon;
            obj["favorite"] = scene->favorite;
            obj["enabled"] = scene->enabled;
            obj["notificationSend"] = scene->notificationSend;
            obj["notificationText"] = scene->notificationText;
            obj["actionCount"] = scene->actionCount;
        }  
        String json;
        serializeJson(doc, json);  
        request->send(200, "application/json", json);
    });
//=============================================================================
    web.on("/api/scenes",HTTP_POST,[](AsyncWebServerRequest* request){},nullptr,
        [](AsyncWebServerRequest* request,uint8_t* data,size_t len,size_t,size_t)
        {
            // ---------- Execute ----------
            if (request->hasParam("action") && request->getParam("action")->value() == "execute")
            {
                if (!request->hasParam("id"))
                {
                    request->send(400,"application/json","{\"success\":false,\"message\":\"Missing id\"}");
                    return;
                }
                uint16_t id =request->getParam("id")->value().toInt();
                if (!sceneManager.execute(id))
                {
                    request->send(404,"application/json","{\"success\":false,\"message\":\"Scene not found\"}");
                    return;
                }
                request->send(200,"application/json","{\"success\":true}");
                return;
            }
            // ---------- Create ----------
            JsonDocument doc;
            if (deserializeJson(doc, data, len))
            {
                request->send(400,"application/json","{\"success\":false,\"message\":\"Invalid JSON\"}");
                return;
            }
        
            Scene scene{};
            scene.name = doc["name"] | "";
            scene.icon = doc["icon"] | "bolt";
            scene.enabled = doc["enabled"] | true;
            scene.favorite = doc["favorite"] | false;
            scene.notificationSend = doc["notificationSend"] | false;
            strlcpy(scene.notificationText,doc["notificationText"] | "",sizeof(scene.notificationText));
            scene.actionCount = 0;
            JsonArray actions = doc["actions"].as<JsonArray>();
            for (JsonObject a : actions)
            {
                if (scene.actionCount >= Scene::MAX_ACTIONS)break;
                SceneAction& action =scene.actions[scene.actionCount++];
                action.channelId  = a["channelId"]  | 0;
                action.state      = a["state"]      | false;
                action.durationMs = a["durationMs"] | 0;
                action.delayMs    = a["delayMs"]    | 0;
            }
            if (!sceneManager.saveScene(scene))
            {
                request->send(500,"application/json","{\"success\":false}");
                return;
            }
            JsonDocument res;
            res["success"] = true;
            res["id"] = scene.id;
            String json;
            serializeJson(res, json);
            request->send(201, "application/json", json);
        });
//=============================================================================
    web.on("/api/scenes",HTTP_PUT,[](AsyncWebServerRequest* request) {},nullptr,
      [](AsyncWebServerRequest* request,uint8_t* data,size_t len,size_t,size_t)
      {
          if (!request->hasParam("id"))
          {
              request->send(400,"application/json","{\"success\":false,\"message\":\"Missing id\"}");
              return;
          }  
          uint16_t id =request->getParam("id")->value().toInt();  
          Scene* current = sceneManager.get(id);  
          if (!current)
          {
              request->send(404,"application/json","{\"success\":false,\"message\":\"Scene not found\"}");
              return;
          }  
          JsonDocument doc;  
          if (deserializeJson(doc, data, len))
          {
              request->send(400,"application/json","{\"success\":false,\"message\":\"Invalid JSON\"}");
              return;
          }  
          Scene updated = *current;  
          updated.name =doc["name"] | updated.name;  
          updated.icon =doc["icon"] | updated.icon;  
          updated.enabled =doc["enabled"] | updated.enabled;  
          updated.favorite =doc["favorite"] | updated.favorite;  
          updated.notificationSend =doc["notificationSend"] |updated.notificationSend;  
          strlcpy(updated.notificationText,doc["notificationText"] |updated.notificationText,sizeof(updated.notificationText));  
          if (doc["actions"].is<JsonArray>())
          {
              updated.actionCount = 0;  
              JsonArray actions =doc["actions"].as<JsonArray>();  
              for (JsonObject a : actions)
              {
                  if (updated.actionCount >= Scene::MAX_ACTIONS)break;  
                  SceneAction& action =updated.actions[updated.actionCount++];  
                  action.channelId  =a["channelId"]  | 0;  
                  action.state      =a["state"]      | false;  
                  action.durationMs =a["durationMs"] | 0;
                  action.delayMs    =a["delayMs"]    | 0;
              }
          }  
          if (!sceneManager.update(updated))
          {
              request->send(500,"application/json","{\"success\":false}");
              return;
          }
          JsonDocument res;
          res["success"] = true;
          res["id"] = updated.id;          
          String json;
          serializeJson(res, json);          
          request->send(200, "application/json", json);
      }
  );
//=============================================================================
  web.on("/api/scenes",HTTP_DELETE,[](AsyncWebServerRequest* request)
      {
          if (!request->hasParam("id"))
          {
              request->send(400,"application/json","{\"success\":false,\"message\":\"Missing id\"}");
              return;
          }  
          uint16_t id =request->getParam("id")->value().toInt();  
          if (!sceneManager.remove(id))
          {
              request->send(404,"application/json","{\"success\":false,\"message\":\"Scene not found\"}");
              return;
          }  
          request->send(200,"application/json","{\"success\":true}");
      }
  );
//=============================================================================

  
}
