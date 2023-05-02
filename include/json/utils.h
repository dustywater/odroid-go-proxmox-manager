#ifndef JSON_UTILS_H
#define JSON_UTILS_H
#include <ArduinoJson.h>
#include <json/retrieve.h>

// Function declarations
Node getNode(const String &name);
Container getContainer(const int &id, const String &node);
VM getVM(const int &id, const String &node);
Disk getDisk(const String &devpath, const String &node);
Pool getPool(const String &name, const String &node);

#endif /* JSON_UTILS_H */
