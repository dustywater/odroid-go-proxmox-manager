#ifndef JSON_RETRIEVE_H
#define JSON_RETRIEVE_H
#include <ArduinoJson.h>

typedef struct {
  String name;
  float cpu;
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


typedef struct {
  String devpath;
  long long size;
  String used;
  String serial;
  String model;
  String vendor;
  String health;
} Disk;

typedef struct {
  String name;
  long long free;
  long long size;
  String health;
} Pool;

Node *getNodeInfo(int &numNodes);
Container *getContainerInfo(int &numContainers, const String &node);
VM *getVMInfo(int &numVMs, const String &node);
Disk *getDiskInfo(int &numDisks, const String &node);
Pool *getPoolInfo(int &numPools, const String &node);

typedef void (*ProcessDataCallback)(DynamicJsonDocument&, int&, void*&);


#endif /* JSON_RETRIEVE_H */
