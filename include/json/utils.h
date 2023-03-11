#ifndef JSON_UTILS_H
#define JSON_UTILS_H
#include <ArduinoJson.h>

JsonObject getNode(String name, JsonArray nodes);
JsonObject getContainer(int id, JsonArray containers);

#endif /* JSON_UTILS_H */
