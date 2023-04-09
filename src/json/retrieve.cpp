#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <server.h>
#include <utils.h>
#include <json/retrieve.h>
#include <stdexcept>

Node *getNodeInfo(int *numNodes)
{
  HTTPClient http;
  String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/";
  String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
  char authc[auth.length() + 1];
  auth.toCharArray(authc, auth.length() + 1);
  http.begin(apiAddress);
  http.setAuthorization(authc);
  int httpCode = http.GET();

  if (httpCode > 0)
  {
    String json = http.getString();
    DynamicJsonDocument doc(8096);
    deserializeJson(doc, json);

    JsonArray nodes = doc["data"].as<JsonArray>();
    Node *nodeArray = new Node[nodes.size()];

    for (int i = 0; i < nodes.size(); i++)
    {
      nodeArray[i].name = nodes[i]["node"].as<String>();
      nodeArray[i].cpu = nodes[i]["cpu"].as<float>();
      nodeArray[i].threads = nodes[i]["maxcpu"].as<int>();
      nodeArray[i].onlineStatus = nodes[i]["status"].as<String>();
      nodeArray[i].mem = nodes[i]["mem"].as<long long>();
      nodeArray[i].maxmem = nodes[i]["maxmem"].as<long long>();
      nodeArray[i].disk = nodes[i]["disk"].as<long long>();
      nodeArray[i].maxdisk = nodes[i]["maxdisk"].as<long long>();
      nodeArray[i].uptime = nodes[i]["uptime"].as<long long>();
    }

    *numNodes = nodes.size();
    return nodeArray;
  }
  throw std::runtime_error("Can't get node info: HTTP request failed with code: " + httpCode);
}

Container *getContainerInfo(int *numContainers, String node)
{
  HTTPClient http;
  String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/" + node + "/lxc";
  String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
  char authc[auth.length() + 1];
  auth.toCharArray(authc, auth.length() + 1);
  http.begin(apiAddress);
  http.setAuthorization(authc);
  int httpCode = http.GET();

  if (httpCode > 0)
  {
    String json = http.getString();
    DynamicJsonDocument doc(8096);
    deserializeJson(doc, json);
    JsonArray containers = doc["data"].as<JsonArray>();
    Container *containerArray = new Container[containers.size()];

    for (int i = 0; i < containers.size(); i++)
    {
      containerArray[i].name = containers[i]["name"].as<String>();
      containerArray[i].id = containers[i]["vmid"].as<int>();
      containerArray[i].onlineStatus = containers[i]["status"].as<String>();
      containerArray[i].maxmem = containers[i]["maxmem"].as<long long>();
      containerArray[i].maxdisk = containers[i]["maxdisk"].as<long long>();
      containerArray[i].uptime = containers[i]["uptime"].as<long long>();
    }

    *numContainers = containers.size();
    return containerArray;
  }

  throw std::runtime_error("Can't get container info: HTTP request failed with code: " + httpCode);
}

VM *getVMInfo(int *numVMs, String node)
{
  HTTPClient http;
  String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/" + node + "/qemu";
  String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
  char authc[auth.length() + 1];
  auth.toCharArray(authc, auth.length() + 1);
  http.begin(apiAddress);
  http.setAuthorization(authc);
  int httpCode = http.GET();

  if (httpCode > 0)
  {
    String json = http.getString();
    DynamicJsonDocument doc(8096);
    deserializeJson(doc, json);
    JsonArray vms = doc["data"].as<JsonArray>();
    VM *vmArray = new VM[vms.size()];

    for (int i = 0; i < vms.size(); i++)
    {
      vmArray[i].name = vms[i]["name"].as<String>();
      vmArray[i].id = vms[i]["vmid"].as<int>();
      vmArray[i].onlineStatus = vms[i]["status"].as<String>();
      vmArray[i].maxmem = vms[i]["maxmem"].as<long long>();
      vmArray[i].maxdisk = vms[i]["maxdisk"].as<long long>();
      vmArray[i].uptime = vms[i]["uptime"].as<long long>();
      vmArray[i].netin = vms[i]["netin"].as<long long>();
      vmArray[i].netout = vms[i]["netout"].as<long long>();
    }

    *numVMs = vms.size();
    return vmArray;
  }
  throw std::runtime_error("Can't get VM info: HTTP request failed with code: " + httpCode);
}

Disk *getDiskInfo(int *numDisks, String node)
{
  HTTPClient http;
  String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/" + node + "/disks/list";
  String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
  char authc[auth.length() + 1];
  auth.toCharArray(authc, auth.length() + 1);
  http.begin(apiAddress);
  http.setAuthorization(authc);
  int httpCode = http.GET();

  if (httpCode > 0)
  {
    String json = http.getString();
    DynamicJsonDocument doc(32768);
    deserializeJson(doc, json);
    JsonArray disks = doc["data"].as<JsonArray>();
    Disk *diskArray = new Disk[disks.size()];

    for (int i = 0; i < disks.size(); i++)
    {
      Serial.println(disks[i]["devpath"].as<String>());
      diskArray[i].devpath = disks[i]["devpath"].as<String>();
      diskArray[i].size = disks[i]["size"].as<long long>();
      diskArray[i].used = disks[i]["used"].as<String>();
      diskArray[i].serial = disks[i]["serial"].as<String>();
      diskArray[i].model = disks[i]["model"].as<String>();
      diskArray[i].vendor = disks[i]["vendor"].as<String>();
      diskArray[i].health = disks[i]["health"].as<String>();
    }

    *numDisks = disks.size();
    return diskArray;
  }
  throw std::runtime_error("Can't get disk info: HTTP request failed with code: " + httpCode);
}

Pool *getPoolInfo(int *numPools, String node)
{
  HTTPClient http;
  String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/" + node + "/disks/zfs";
  String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
  char authc[auth.length() + 1];
  auth.toCharArray(authc, auth.length() + 1);
  http.begin(apiAddress);
  http.setAuthorization(authc);
  int httpCode = http.GET();

  if (httpCode > 0)
  {
    String json = http.getString();
    DynamicJsonDocument doc(32768);
    deserializeJson(doc, json);
    JsonArray pools = doc["data"].as<JsonArray>();
    Pool *poolArray = new Pool[pools.size()];

    for (int i = 0; i < pools.size(); i++)
    {
      poolArray[i].name = pools[i]["name"].as<String>();
      poolArray[i].free = pools[i]["free"].as<long long>();
      poolArray[i].size = pools[i]["size"].as<long long>();
      poolArray[i].health = pools[i]["health"].as<String>();
    }

    *numPools = pools.size();
    return poolArray;
  }
  throw std::runtime_error("Can't get pool info: HTTP request failed with code: " + httpCode);
}