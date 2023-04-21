#ifndef JSON_SEND_H
#define JSON_SEND_H
#include <Arduino.h>
void restartVM(const String &node, const int &vmid);
void restartContainer(const String &node, const int &containerid);
void startVM(const String &node, const int &vmid);
void startContainer(const String &node, const int &containerid);
void stopVM(const String &node, const int &vmid);
void stopContainer(const String &node, const int &containerid);


#endif /* JSON_SEND_H */
