#include <ArduinoJson.h>
#include <json/utils.h>
#include <utils.h>
#include <stdexcept>

/// @brief Function to retrieve a specific node. Retrieves an array of the nodes and uses a for loop to check the name of each one until it finds one that matches the provided name.
/// @param name A reference to a string containing the name of the node being searched for.
/// @return The node which matches the name provided.
Node getNode(const String &name)
{
  int numNodes;

  Node *nodes = getNodeInfo(numNodes);
  for (int i = 0; i < numNodes; i++)
  {
    if (nodes[i].name == name)
    {
      Node node = nodes[i];
      delete[] nodes;
      return node;
    }
  }

  throw std::runtime_error("Can't find the requested node in the array");
}

/// @brief Function to retrieve a specific container. Retrieves an array of the containers and uses a for loop to check the id of each one until it finds one that matches the provided id.
/// @param id A reference to an integer which contains the id of the container being searched for.
/// @param node A reference to a string which contains the name of the node being searched on.
/// @return A container that matches the id provided.
Container getContainer(const int &id, const String &node)
{

  int numContainers;
  Container *containers = getContainerInfo(numContainers, node);
  for (int i = 0; i < numContainers; i++)
  {
    if (containers[i].id == id)
    {
      Container container = containers[i];
      delete[] containers;
      return container;
    }
  }

  throw std::runtime_error("Can't find the requested container in the array");
}

/// @brief Function to retrieve a specific VM. Retrieves an array of the VMs and uses a for loop to check the id of each one until it finds one that matches the provided id.
/// @param id A reference to an integer which contains the id of the VM being searched for.
/// @param node A reference to a string which contains the name of the node being searched on.
/// @return A VM that matches the id provided.
VM getVM(const int &id, const String &node)
{
  int numVMs;

  VM *vms = getVMInfo(numVMs, node);
  for (int i = 0; i < numVMs; i++)
  {
    if (vms[i].id == id)
    {
      return vms[i];
    }
  }

  throw std::runtime_error("Can't find the requested vm in the array");
}

/// @brief Function to retrieve a specific disk. Retrieves an array of the containers and uses a for loop to check the devpath (e.g /dev/sda) of each one until it finds one that matches the provided devpath.
/// @param devpath A reference to a string which contains the devpath of the container being searched for.
/// @param node A reference to a string which contains the name of the node being searched on.
/// @return A disk that matches the devpath provided.
Disk getDisk(const String &devpath, const String &node)
{
  int numDisks;

  Disk *disks = getDiskInfo(numDisks, node);
  for (int i = 0; i < numDisks; i++)
  {
    if (disks[i].devpath == devpath)
    {
      return disks[i];
    }
  }

  throw std::runtime_error("Can't find the requested disk in the array");
}

/// @brief Function to retrieve a specific ZFS pool. Retrieves an array of the pools and uses a for loop to check the name of each one until it finds one that matches the provided name.
/// @param name A reference to a string which contains the name of the pool being searched for.
/// @param node A reference to a string which contains the name of the node being searched on.
/// @return A pool that matches the name provided.
Pool getPool(const String &name, const String &node)
{
  int numPools;

  Pool *pools = getPoolInfo(numPools, node);
  for (int i = 0; i < numPools; i++)
  {
    if (pools[i].name == name)
    {
      return pools[i];
    }
  }

  throw std::runtime_error("Can't find the requested pool in the array");
}