#include <json/retrieve.h>
#include <json/utils.h>
#include <global.h>
#include <menu.h>
#include <odroid_go.h>
#include <utils.h>

String convertBytes(long long value) {
  if (value > 1099511627776) {
    return String(value / 1099511627776) + " TB";
  }
  if (value > 1073741824) {
    return String(value / 1073741824) + " GB";
  }
  if (value > 1048576) {
    return String(value / 1048576) + " MB";
  }
  if (value > 1024) {
    return String(value / 1024) + " KB";
  }
  return String(value) + " Bytes";
}


String convertTime(long long value) {
  if (value > 3600) {
    return String(value / 3600) + " hrs";
  }
  if (value > 60) {
    return String(value / 60) + " mins";
  }
  return String(value) + " secs";
}

void printNodeStats(Node node)
{
  GO.lcd.fillScreen(WHITE);
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(BLACK);

  GO.lcd.setTextSize(3);
  GO.lcd.println(node.name);
  GO.lcd.setTextSize(2);
  GO.lcd.println("--------------------------");
  GO.lcd.println("Status: " + node.onlineStatus);
  GO.lcd.println("Uptime: " + convertTime(node.uptime));
  GO.lcd.println("CPU: " + String(node.cpu) + "%");
  GO.lcd.println("Threads: " + String(node.threads));
  GO.lcd.println("RAM: " + convertBytes(node.mem));
  GO.lcd.println("Max RAM: " + convertBytes(node.maxmem));
  GO.lcd.println("Disk: " + convertBytes(node.disk));
  GO.lcd.println("Max Disk: " + convertBytes(node.maxdisk));
}

void printContainerStats(Container container)
{

  GO.lcd.fillScreen(WHITE);
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(BLACK);

  GO.lcd.setTextSize(3);
  GO.lcd.println(String(container.id) + ": " + container.name);
  GO.lcd.setTextSize(2);
  GO.lcd.println("--------------------------");
  GO.lcd.println("Status: " + container.onlineStatus);
  GO.lcd.println("Uptime: " + convertTime(container.uptime));
  GO.lcd.println("Max RAM: " + convertBytes(container.maxmem));
  GO.lcd.println("Max Disk: " + convertBytes(container.maxdisk));
}

void printVMStats(VM vm)
{

  GO.lcd.fillScreen(WHITE);
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(BLACK);

  GO.lcd.setTextSize(3);
  GO.lcd.println(String(vm.id) + ": " + vm.name);
  GO.lcd.setTextSize(2);
  GO.lcd.println("--------------------------");
  GO.lcd.println("Status: " + vm.onlineStatus);
  GO.lcd.println("Uptime: " + convertTime(vm.uptime));
  GO.lcd.println("Max RAM: " + convertBytes(vm.maxmem));
  GO.lcd.println("Max Disk: " + convertBytes(vm.maxdisk));
}

void nodeInfo()
{
  Serial.println("nodeinfo");
  while (true)
  {

    printNodeStats(getNode(selectedNode));
    delay(2000);
  }
}

void containerInfo()
{
  Serial.println("container info");
  selectedItem = 0;
  int numContainers;

  Container *containers = getContainerInfo(&numContainers, selectedNode);

  listContainers(containers, numContainers);

  while (true)
  {
    printContainerStats(getContainer(selectedLXC, selectedNode));
    delay(5000);
  }
}

void vmInfo()
{
  Serial.println("vm info");
  selectedItem = 0;
  int numVMs;
  VM *vms = getVMInfo(&numVMs, selectedNode);
  if (vms != NULL)
  {
    listVMs(vms, numVMs);

    while (true)
    {
      printVMStats(getVM(selectedVM, selectedNode));
      delay(5000);
    }
  }
}
