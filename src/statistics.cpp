#include <json/retrieve.h>
#include <json/utils.h>
#include <global.h>
#include <menu.h>
#include <odroid_go.h>
#include <utils.h>

const int BACKGROUND_COLOR = WHITE;
const int TEXT_COLOR = BLACK;
const int TEXT_SIZE = 2;

String convertBytes(const long long &value)
{
  if (value > 1099511627776)
  {
    return String(value / 1099511627776) + " TB";
  }
  if (value > 1073741824)
  {
    return String(value / 1073741824) + " GB";
  }
  if (value > 1048576)
  {
    return String(value / 1048576) + " MB";
  }
  if (value > 1024)
  {
    return String(value / 1024) + " KB";
  }
  return String(value) + " Bytes";
}

String convertTime(const long long &value)
{
  if (value > 3600)
  {
    return String(value / 3600) + " hrs";
  }
  if (value > 60)
  {
    return String(value / 60) + " mins";
  }
  return String(value) + " secs";
}

void printNodeStats(const Node &node)
{
  GO.lcd.fillScreen(BACKGROUND_COLOR);
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(TEXT_COLOR);

  GO.lcd.setTextSize(TEXT_SIZE);
  GO.lcd.println(node.name);

  GO.lcd.println("--------------------------");
  GO.lcd.println("Status: " + node.onlineStatus);
  GO.lcd.println("Uptime: " + convertTime(node.uptime));
  GO.lcd.println("CPU: " + String(node.cpu * 100) + "%");
  GO.lcd.println("Threads: " + String(node.threads));
  GO.lcd.println("RAM: " + convertBytes(node.mem));
  GO.lcd.println("Max RAM: " + convertBytes(node.maxmem));
  GO.lcd.println("Disk: " + convertBytes(node.disk));
  GO.lcd.println("Max Disk: " + convertBytes(node.maxdisk));
}

void printContainerStats(const Container &container)
{

  GO.lcd.fillScreen(BACKGROUND_COLOR);
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(TEXT_COLOR);

  GO.lcd.setTextSize(TEXT_SIZE);
  GO.lcd.println(String(container.id) + ": " + container.name);
  GO.lcd.println("--------------------------");
  GO.lcd.println("Status: " + container.onlineStatus);
  GO.lcd.println("Uptime: " + convertTime(container.uptime));
  GO.lcd.println("Max RAM: " + convertBytes(container.maxmem));
  GO.lcd.println("Max Disk: " + convertBytes(container.maxdisk));
}

void printVMStats(const VM &vm)
{

  GO.lcd.fillScreen(BACKGROUND_COLOR);
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(TEXT_COLOR);
  GO.lcd.setTextSize(TEXT_SIZE);
  GO.lcd.println(String(vm.id) + ": " + vm.name);
  GO.lcd.println("--------------------------");
  GO.lcd.println("Status: " + vm.onlineStatus);
  GO.lcd.println("Uptime: " + convertTime(vm.uptime));
  GO.lcd.println("Max RAM: " + convertBytes(vm.maxmem));
  GO.lcd.println("Max Disk: " + convertBytes(vm.maxdisk));
}

void printDiskStats(const Disk &disk)
{

  GO.lcd.fillScreen(BACKGROUND_COLOR);
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(TEXT_COLOR);
  GO.lcd.setTextSize(TEXT_SIZE);

  GO.lcd.println(disk.devpath);
  GO.lcd.println("--------------------------");
  GO.lcd.println("Vendor: " + disk.vendor);
  GO.lcd.println("Model: " + disk.model);
  GO.lcd.println("Serial: " + disk.serial);
  GO.lcd.println("Size: " + convertBytes(disk.size));
  GO.lcd.println("Used For: " + disk.used);
  GO.lcd.println("Health: " + disk.health);
}

void printPoolStats(const Pool &pool)
{

  GO.lcd.fillScreen(BACKGROUND_COLOR);
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(TEXT_COLOR);
  GO.lcd.setTextSize(TEXT_SIZE);

  GO.lcd.println(pool.name);
  GO.lcd.println("--------------------------");
  GO.lcd.println("Free: " + convertBytes(pool.free));
  GO.lcd.println("Size: " + convertBytes(pool.size));
  GO.lcd.println("Health: " + pool.health);
}

void nodeInfo()
{
  Serial.println("nodeinfo");
  unsigned long lastUpdate = millis();
  int updateInterval = 3000;
  while (true)
  {
    if (lastUpdate + updateInterval < millis())
    {
      printNodeStats(getNode(selectedNode));
      lastUpdate = millis();
    }

    GO.update();
    if (GO.BtnB.isPressed())
    {
      break;
    }
  }
  mainMenu();
}

void containerInfo()
{
  Serial.println("container info");
  selectedItem = 0;
  selectedPage = 0;
  int numContainers;

  Container *containers = getContainerInfo(numContainers, selectedNode);

  listContainers(containers, numContainers);
  delete[] containers;
  unsigned long lastUpdate = millis();
  int updateInterval = 3000;

  while (true)
  {
    if (lastUpdate + updateInterval < millis())
    {
      printContainerStats(getContainer(selectedLXC, selectedNode));
      lastUpdate = millis();
    }

    GO.update();
    if (GO.BtnB.isPressed())
    {
      mainMenu();
      break;
    }
  }
}

void vmInfo()
{
  Serial.println("vm info");
  selectedItem = 0;
  selectedPage = 0;
  int numVMs;
  VM *vms = getVMInfo(numVMs, selectedNode);
  listVMs(vms, numVMs);
  delete[] vms;
  unsigned long lastUpdate = millis();
  int updateInterval = 3000;
  while (true)
  {
    if (lastUpdate + updateInterval < millis())
    {
      printVMStats(getVM(selectedVM, selectedNode));
      lastUpdate = millis();
    }
    GO.update();
    if (GO.BtnB.isPressed())
    {
      mainMenu();
      break;
    }
  }
}

void diskInfo()
{
  Serial.println("disk info");
  selectedItem = 0;
  selectedPage = 0;
  int numDisks;
  Disk *disks = getDiskInfo(numDisks, selectedNode);

  listDisks(disks, numDisks);
  delete[] disks;

  printDiskStats(getDisk(selectedDisk, selectedNode));
  while (true)
  {
    GO.update();
    if (GO.BtnB.isPressed())
    {
      mainMenu();
      break;
    }
  }
}

void poolInfo()
{
  Serial.println("pool info");
  selectedItem = 0;
  selectedPage = 0;
  int numPools;
  Pool *pools = getPoolInfo(numPools, selectedNode);

  listPools(pools, numPools);
  delete[] pools;

  printPoolStats(getPool(selectedPool, selectedNode));
  while (true)
  {
    GO.update();
    if (GO.BtnB.isPressed())
    {
      mainMenu();
      break;
    }
  }
}