#include <json/retrieve.h>
#include <json/utils.h>
#include <menu.h>
#include <odroid_go.h>
#include <utils.h>

// Constants for UI
const int BACKGROUND_COLOR = WHITE;
const int TEXT_COLOR = BLACK;
const int TEXT_SIZE = 2;

// How often to update the statistics on the screen.
const int UPDATE_INTERVAL = 3000;

/**
 * @brief Function to convert bytes to more human readable formats. Works by checking if the value is over a certain size before dividing it and adding the appropriate label.
 * Casts the value to a double before dividing so that the value can be displayed more accurately.
 *
 * @param value The value in bytes to convert.
 * @return String The converted value in a human readable string.
 */
String convertBytes(const long long &value)
{
  if (value > 1099511627776)
  {
    double result = static_cast<double>(value) / 1099511627776.0;
    return String(result, 2) + " TiB";
  }
  if (value > 1073741824)
  {
    double result = static_cast<double>(value) / 1073741824.0;
    return String(result, 2) + " GiB";
  }
  if (value > 1048576)
  {
    double result = static_cast<double>(value) / 1048576.0;
    return String(result, 2) + " MiB";
  }
  if (value > 1024)
  {
    double result = static_cast<double>(value) / 1024.0;
    return String(result, 2) + " KiB";
  }
  return String(value) + " Bytes";
}

/**
 * @brief Function convert seconds to more human readable formats. Works by checking if the value is over a certain number of seconds and then divides it before adding a label.
 *
 * @param value The time in seconds.
 * @return String The converted time as a human readable string.
 */
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

/**
 * @brief Function to print the stats for a node. Uses convertTime() and convertBytes() to make values human friendly.
 *
 * @param node The node to print the statistics of.
 */
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

/**
 * @brief Function to print the stats for a container. Uses convertTime() and convertBytes() to make values human friendly.
 *
 * @param container The container to print the statistics of.
 */
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

/**
 * @brief Function to print the stats for a VM. Uses convertTime() and convertBytes() to make values human friendly.
 *
 * @param vm The VM to print the statistics of.
 */
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

/**
 * @brief Function to print the stats for a disk. Uses convertBytes() to make values human friendly.
 *
 * @param disk The disk to print the statistics of.
 */
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

/**
 * @brief Function to print the stats for a pool. Uses convertBytes() to make values human friendly.
 *
 * @param pool The pool to print statistics of.
 */
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

/**
 * @brief The calling function to print and retrieve node information.
 * Retrieves the node statistics from the API by calling the getNode() function before using the printNodeStats() function to update the statistics on the screen at the specified interval.
 * Returns to the main menu if the back button is pressed.
 */
void nodeInfo()
{
  Serial.println("nodeinfo");
  unsigned long lastUpdate = 0;
  while (true)
  {
    if (lastUpdate + UPDATE_INTERVAL < millis())
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

/**
 * @brief The calling function to print and retrieve container information.
 * Retrieves the list of containers from the API by calling the getContainerInfo() function. Then asks the user to select a container using listContainers().
 * After a container is selected, uses the getContainer() function to retrieve the container statistics and then prints them on the screen using the printContainerStats() function at the specified interval.
 *
 * Returns to the previous screen if the back button is pressed.
 */
void containerInfo()
{
  Serial.println("container info");
  selectedItem = 0;
  selectedPage = 0;
  int numContainers;

  Container *containers = getContainerInfo(numContainers, selectedNode);

  int selectedLXC = listContainers(containers, numContainers);
  delete[] containers;
  unsigned long lastUpdate = 0;

  while (selectedLXC > 0)
  {
    if (lastUpdate + UPDATE_INTERVAL < millis())
    {
      printContainerStats(getContainer(selectedLXC, selectedNode));
      lastUpdate = millis();
    }

    GO.update();
    if (GO.BtnB.isPressed())
    {
      containerInfo();
      break;
    }
  }
  
  mainMenu();
}

/**
 * @brief The calling function to print and retrieve VM information.
 * Retrieves the list of VMs from the API by calling the getVMInfo() function. Then asks the user to select a VM using listVMs().
 * After a VM is selected, uses the getVM() function to retrieve the VM statistics and then prints them on the screen using the printVMStats() function at the specified interval.
 *
 * Returns to the previous screen if the back button is pressed.
 */
void vmInfo()
{
  Serial.println("vm info");
  selectedItem = 0;
  selectedPage = 0;
  int numVMs;
  VM *vms = getVMInfo(numVMs, selectedNode);
  int selectedVM = listVMs(vms, numVMs);
  delete[] vms;
  unsigned long lastUpdate = 0;

  while (selectedVM > 0)
  {
    if (lastUpdate + UPDATE_INTERVAL < millis())
    {
      printVMStats(getVM(selectedVM, selectedNode));
      lastUpdate = millis();
    }
    GO.update();
    if (GO.BtnB.isPressed())
    {
      vmInfo();
      break;
    }
  }
  mainMenu();
}

/**
 * @brief The calling function to print and retrieve disk information.
 * Retrieves the list of disks from the API by calling the getDiskInfo() function. Then asks the user to select a disk using listDisks().
 * After a disk is selected, uses the getDisk() function to retrieve the disk statistics and then prints them on the screen using the printDiskStats() function.
 *
 * Returns to the previous screen if the back button is pressed.
 */
void diskInfo()
{
  Serial.println("disk info");
  selectedItem = 0;
  selectedPage = 0;

  int numDisks;
  Disk *disks = getDiskInfo(numDisks, selectedNode);

  String selectedDisk = listDisks(disks, numDisks);
  delete[] disks;

  if (selectedDisk != "")
  {
    printDiskStats(getDisk(selectedDisk, selectedNode));
    while (true)
    {
      GO.update();
      if (GO.BtnB.isPressed())
      {
        diskInfo();
        break;
      }
    }
    
  }
  mainMenu();
}

/**
 * @brief The calling function to print and retrieve pool information.
 * Retrieves the list of pools from the API by calling the getPoolInfo() function. Then asks the user to select a pool using listPools().
 * After a pool is selected, uses the getPool() function to retrieve the pool statistics and then prints them on the screen using the printPoolStats() function.
 *
 * Returns to the previous screen if the back button is pressed.
 */
void poolInfo()
{
  Serial.println("pool info");
  selectedItem = 0;
  selectedPage = 0;

  int numPools;
  Pool *pools = getPoolInfo(numPools, selectedNode);

  String selectedPool = listPools(pools, numPools);
  delete[] pools;

  if (selectedPool != "")
  {
    printPoolStats(getPool(selectedPool, selectedNode));
    while (true)
    {
      GO.update();
      if (GO.BtnB.isPressed())
      {
        poolInfo();
        break;
      }
    }
  }
  mainMenu();
}