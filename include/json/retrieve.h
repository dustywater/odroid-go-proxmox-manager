#ifndef JSON_RETRIEVE_H
#define JSON_RETRIEVE_H
#include <ArduinoJson.h>

typedef struct {
  String name;
  int cpu;
  int threads;
  String onlineStatus;
  long long mem;
  long long maxmem;
  long long disk;
  long long maxdisk;
  long long uptime;
} Node;

typedef struct {
  String name;
  int id;
  String onlineStatus;
  long long maxmem;
  long long maxdisk;
  long long uptime;
} Container;

typedef struct {
  String name;
  int id;
  String onlineStatus;
  long long maxmem;
  long long maxdisk;
  long long uptime;
  long long netin;
  long long netout;
} VM;

Node *getNodeInfo(int *numNodes);
Container *getContainerInfo(int *numContainers, String node);
VM *getVMInfo(int *numVMs, String node);


#endif /* JSON_RETRIEVE_H */
