#ifndef JSON_RETRIEVE_H
#define JSON_RETRIEVE_H
#include <ArduinoJson.h>

JsonArray getNodeInfo();
JsonArray getContainerInfo(String node);
JsonArray getVMInfo(String node);

#endif /* JSON_RETRIEVE_H */
