#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <server.h>
#include <utils.h>
#include <json/retrieve.h>
#include <stdexcept>

/// @brief Function to compare nodes by the uptime.
/// @param a The first node to compare
/// @param b The node to compare to
/// @return True if the uptime of node b is higher, false if uptime of a is higher.
bool compareNode(const Node &a, const Node &b) {
  return a.uptime < b.uptime;
}

/// @brief Function to compare containers by ID.
/// @param a The first container to compare
/// @param b The container to compare to
/// @return True if the ID of container b is higher, false if uptime of a is higher.
bool compareContainer(const Container &a, const Container &b) {
  return a.id < b.id;
}


/// @brief Function to compare VMs by ID.
/// @param a The first VM to compare
/// @param b The VM to compare to
/// @return True if the ID of VM b is higher, false if ID of a is higher.
bool compareVM(const VM &a, const VM &b) {
  return a.id < b.id;
}


/// @brief This is a callback function to process the json data of a node provided by the API into an array of Node structs. Also sorts the nodes based on uptime.
/// @param doc A reference to the json document containing the data from the API.
/// @param numNodes A reference to an integer containing the number of nodes retrieved from the API.
/// @param nodeArray A pointer to the array used to hold the nodes once they have been processed.
void processNodeData(DynamicJsonDocument &doc, int &numNodes, void *&nodeArray)
{
  JsonArray nodes = doc["data"].as<JsonArray>();
  Node *array = new Node[nodes.size()];

  for (int i = 0; i < nodes.size(); i++)
  {
    array[i].name = nodes[i]["node"].as<String>();
    array[i].cpu = nodes[i]["cpu"].as<float>();
    array[i].threads = nodes[i]["maxcpu"].as<int>();
    array[i].onlineStatus = nodes[i]["status"].as<String>();
    array[i].mem = nodes[i]["mem"].as<long long>();
    array[i].maxmem = nodes[i]["maxmem"].as<long long>();
    array[i].disk = nodes[i]["disk"].as<long long>();
    array[i].maxdisk = nodes[i]["maxdisk"].as<long long>();
    array[i].uptime = nodes[i]["uptime"].as<long long>();
  }

  // Sort the array
  std::sort(array, array + nodes.size(), compareNode);

  numNodes = nodes.size();
  nodeArray = array;
}

/// @brief This is a callback function to process the json data of a container provided by the api into an array of Container structs. Also sorts the containers based on id.
/// @param doc A reference to the json document containing the data from the API.
/// @param numContainers A reference to an integer containing the number of containers retrieved from the API.
/// @param containerArray A pointer to the array used to hold the containers once they have been processed.
void processContainerData(DynamicJsonDocument &doc, int &numContainers, void *&containerArray)
{
  JsonArray containers = doc["data"].as<JsonArray>();
  Container *array = new Container[containers.size()];

  for (int i = 0; i < containers.size(); i++)
  {
    array[i].name = containers[i]["name"].as<String>();
    array[i].id = containers[i]["vmid"].as<int>();
    array[i].onlineStatus = containers[i]["status"].as<String>();
    array[i].maxmem = containers[i]["maxmem"].as<long long>();
    array[i].maxdisk = containers[i]["maxdisk"].as<long long>();
    array[i].uptime = containers[i]["uptime"].as<long long>();
  }

    // Record the starting time
  unsigned long startTime = micros();

  // Run the function you want to measure
  std::sort(array, array + containers.size(), compareContainer);

  // Calculate the elapsed time
  unsigned long elapsedTime = micros() - startTime;

  // Print the elapsed time in microseconds
  Serial.print("Elapsed time: ");
  Serial.print(elapsedTime);
  Serial.println(" microseconds");


  // Sort the array
  //std::sort(array, array + containers.size(), compareContainer);

  numContainers = containers.size();
  containerArray = array;
}

/// @brief This is a callback function to process the json data of a VM provided by the api into an array of VM structs. Also sorts the VMs based on id.
/// @param doc A reference to the json document containing the data from the API.
/// @param numVMs A reference to an integer containing the number of VMs retrieved from the API.
/// @param vmArray A pointer to the array used to hold the VMs once they have been processed.
void processVMData(DynamicJsonDocument &doc, int &numVMs, void *&vmArray)
{
  JsonArray vms = doc["data"].as<JsonArray>();
  VM *array = new VM[vms.size()];

  for (int i = 0; i < vms.size(); i++)
  {
    array[i].name = vms[i]["name"].as<String>();
    array[i].id = vms[i]["vmid"].as<int>();
    array[i].onlineStatus = vms[i]["status"].as<String>();
    array[i].maxmem = vms[i]["maxmem"].as<long long>();
    array[i].maxdisk = vms[i]["maxdisk"].as<long long>();
    array[i].uptime = vms[i]["uptime"].as<long long>();
    array[i].netin = vms[i]["netin"].as<long long>();
    array[i].netout = vms[i]["netout"].as<long long>();
  }

  // Sort the array
  std::sort(array, array + vms.size(), compareVM);

  numVMs = vms.size();
  vmArray = array;
}

/// @brief This is a callback function to process the json data of a disk provided by the api into an array of disk structs.
/// @param doc A reference to the json document containing the data from the API.
/// @param numDisks A reference to an integer containing the number of disks retrieved from the API.
/// @param diskArray A pointer to the array used to hold the disks once they have been processed.
void processDiskData(DynamicJsonDocument &doc, int &numDisks, void *&diskArray)
{
  JsonArray disks = doc["data"].as<JsonArray>();
  Disk *array = new Disk[disks.size()];

  for (int i = 0; i < disks.size(); i++)
  {
    array[i].devpath = disks[i]["devpath"].as<String>();
    array[i].size = disks[i]["size"].as<long long>();
    array[i].used = disks[i]["used"].as<String>();
    array[i].serial = disks[i]["serial"].as<String>();
    array[i].model = disks[i]["model"].as<String>();
    array[i].vendor = disks[i]["vendor"].as<String>();
    array[i].health = disks[i]["health"].as<String>();
  }

  numDisks = disks.size();
  diskArray = array;
}

/// @brief This is a callback function to process the json data of a ZFS pool provided by the api into an array of Pool structs.
/// @param doc A reference to the json document containing the data from the API.
/// @param numPools A reference to an integer containing the number of pools retrieved from the API.
/// @param poolArray A pointer to the array used to hold the pools once they have been processed.
void processPoolData(DynamicJsonDocument &doc, int &numPools, void *&poolArray)
{
  JsonArray pools = doc["data"].as<JsonArray>();
  Pool *array = new Pool[pools.size()];

  for (int i = 0; i < pools.size(); i++)
  {
    array[i].name = pools[i]["name"].as<String>();
    array[i].free = pools[i]["free"].as<long long>();
    array[i].size = pools[i]["size"].as<long long>();
    array[i].health = pools[i]["health"].as<String>();
  }

  numPools = pools.size();
  poolArray = array;
}

/// @brief This is the main function to retrieve data from the API. Is is a generic function to handle retrieving all the data for the program.
/// @tparam T The type of the item being retrieved.
/// @param apiAddress The API path to get the appropriate data.
/// @param numItems A reference to an integer to store the number of items retrieved from the API.
/// @param processData A callback function to process the data coming in from the API.
/// @return The array of items populated by the process function is returned in the type defined by T.
template <typename T>
T *apiCall(const String &apiAddress, int &numItems, ProcessDataCallback processData)
{
  HTTPClient http;
  http.begin(PROXMOX_ADDRESS + apiAddress);
  String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
  char authc[auth.length() + 1];
  auth.toCharArray(authc, auth.length() + 1);
  http.setAuthorization(authc);
  int httpCode = http.GET();

  if (httpCode > 0)
  {
    String json = http.getString();
    DynamicJsonDocument doc(32768);
    deserializeJson(doc, json);

    void *itemArray = nullptr;
    processData(doc, numItems, itemArray);

    return static_cast<T *>(itemArray);
  }
  throw std::runtime_error("HTTP request failed with code: " + httpCode);
}

/// @brief Function to retrieve node data from the API. Calls the apiCall() function with the appropriate parameters for nodes.
/// @param numNodes A reference to an integer to hold the number of nodes retrieved.
/// @return The array of nodes created by the apiCall() function.
Node *getNodeInfo(int &numNodes)
{
  String apiAddress = "/api2/json/nodes/";
  return apiCall<Node>(apiAddress, numNodes, processNodeData);
}

/// @brief Function to retrieve container data from the API. Calls the apiCall() function with the appropriate parameters for containers.
/// @param numContainers A reference to an integer to hold the number of containers retrieved.
/// @param node A reference to a string which defines which node we are retrieving container data from.
/// @return The array of containers created by the apiCall() function.
Container *getContainerInfo(int &numContainers, const String &node)
{
  String apiAddress = "/api2/json/nodes/" + node + "/lxc";
  return apiCall<Container>(apiAddress, numContainers, processContainerData);
}

/// @brief Function to retrieve VM data from the API. Calls the apiCall() function with the appropriate parameters for VMs.
/// @param numVMs A reference to an integer to hold the number of VMs retrieved.
/// @param node A reference to a string which defines which node we are retrieving VM data from.
/// @return The array of VMs created by the apiCall() function.
VM *getVMInfo(int &numVMs, const String &node)
{
  String apiAddress = "/api2/json/nodes/" + node + "/qemu";
  return apiCall<VM>(apiAddress, numVMs, processVMData);
}

/// @brief Function to retrieve disk data from the API. Calls the apiCall() function with the appropriate parameters for disks.
/// @param numDisks A reference to an integer to hold the number of disks retrieved.
/// @param node A reference to a string which defines which node we are retrieving disk data from.
/// @return The array of disks created by the apiCall() function.
Disk *getDiskInfo(int &numDisks, const String &node)
{
  String apiAddress = "/api2/json/nodes/" + node + "/disks/list";
  return apiCall<Disk>(apiAddress, numDisks, processDiskData);
}

/// @brief Function to retrieve pool data from the API. Calls the apiCall() function with the appropriate parameters for pools.
/// @param numPools A reference to an integer to hold the number of pools retrieved.
/// @param node A reference to a string which defines which node we are retrieving pool data from.
/// @return The array of pools created by the apiCall() function.
Pool *getPoolInfo(int &numPools, const String &node)
{
  String apiAddress = "/api2/json/nodes/" + node + "/disks/zfs";
  return apiCall<Pool>(apiAddress, numPools, processPoolData);
}
