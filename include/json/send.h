#ifndef JSON_SEND_H
#define JSON_SEND_H
#include <Arduino.h>
void restartVM(String node, int vmid);
void restartContainer(String node, int containerid);
void restartNode(String node);

#endif /* JSON_SEND_H */
