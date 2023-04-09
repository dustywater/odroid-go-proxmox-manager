#include <ArduinoJson.h>
#include <json/utils.h>
#include <utils.h>
#include <stdexcept>

Node getNode(String name)
{
  int numNodes;

  Node *nodes = getNodeInfo(&numNodes);
  for (int i = 0; i < numNodes; i++)
  {
    if (nodes[i].name == name)
    {
      return nodes[i];
    }
  }

  throw std::runtime_error("Can't find the requested node in the array");
}

Container getContainer(int id, String node)
{

  int numContainers;
  Container *containers = getContainerInfo(&numContainers, node);
  for (int i = 0; i < numContainers; i++)
  {
    if (containers[i].id == id)
    {
      return containers[i];
    }
  }

  throw std::runtime_error("Can't find the requested container in the array");
}

VM getVM(int id, String node)
{
  int numVMs;

  VM *vms = getVMInfo(&numVMs, node);
  for (int i = 0; i < numVMs; i++)
  {
    if (vms[i].id == id)
    {
      return vms[i];
    }
  }

  throw std::runtime_error("Can't find the requested vm in the array");
}

Disk getDisk(String devpath, String node)
{
  int numDisks;

  Disk *disks = getDiskInfo(&numDisks, node);
  for (int i = 0; i < numDisks; i++)
  {
    if (disks[i].devpath == devpath)
    {
      return disks[i];
    }
  }

  throw std::runtime_error("Can't find the requested disk in the array");
}

Pool getPool(String name, String node)
{
  int numPools;

  Pool *pools = getPoolInfo(&numPools, node);
  for (int i = 0; i < numPools; i++)
  {
    if (pools[i].name == name)
    {
      return pools[i];
    }
  }

  throw std::runtime_error("Can't find the requested pool in the array");
}