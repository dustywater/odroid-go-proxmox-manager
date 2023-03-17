#ifndef JSON_UTILS_H
#define JSON_UTILS_H
#include <ArduinoJson.h>
#include <json/retrieve.h>
//JsonObject getNode(String name, JsonArray nodes);
Node getNode(String name);
Container getContainer(int id, String node);
VM getVM(int id, String node);

#endif /* JSON_UTILS_H */
