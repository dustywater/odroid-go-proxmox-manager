#ifndef JSON_SEND_H
#define JSON_SEND_H
#include <Arduino.h>
void restartVM(String node, int vmid);
void restartContainer(String node, int containerid);
void startVM(String node, int vmid);
void startContainer(String node, int containerid);
void stopVM(String node, int vmid);
void stopContainer(String node, int containerid);


#endif /* JSON_SEND_H */
