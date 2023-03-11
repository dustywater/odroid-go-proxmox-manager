#include <json/retrieve.h>
#include <json/utils.h>
#include <global.h>
#include <menu.h>
#include <odroid_go.h>


void printNodeStats(JsonObject node) {
  String name = node["node"].as<String>();
  int cpu = round(node["cpu"].as<float>() * 100);
  int threads = node["maxcpu"].as<int>();
  String onlineStatus = node["status"].as<String>();
  String mem = node["mem"].as<String>();
  String maxmem = node["maxmem"].as<String>();
  String disk = node["disk"].as<String>();
  String maxdisk = node["maxdisk"].as<String>();
  String uptime = node["uptime"].as<String>();
  // these are strings for now because they could be way too big

  GO.lcd.fillScreen(WHITE);
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(BLACK);

  GO.lcd.setTextSize(3);
  GO.lcd.println(name);
  GO.lcd.setTextSize(2);
  GO.lcd.println("--------------------------");
  GO.lcd.println("Status: " + onlineStatus);
  GO.lcd.println("Uptime: " + uptime + "s");
  GO.lcd.println("CPU: " + String(cpu) + "%");
  GO.lcd.println("Threads: " + String(threads));
  GO.lcd.println("RAM: " + mem);
  GO.lcd.println("Max RAM: " + maxmem);
  GO.lcd.println("Disk: " + disk);
  GO.lcd.println("Max Disk: " + maxdisk);
  
}

void printContainerStats(JsonObject container) {
  String name = container["name"].as<String>();
  String id = container["vmid"].as<String>();
  String onlineStatus = container["status"].as<String>();
  String maxmem = container["maxmem"].as<String>();
  String maxdisk = container["maxdisk"].as<String>();
  String uptime = container["uptime"].as<String>();
  // these are strings for now because they could be way too big

  GO.lcd.fillScreen(WHITE);
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(BLACK);

  GO.lcd.setTextSize(3);
  GO.lcd.println(id + ": " + name);
  GO.lcd.setTextSize(2);
  GO.lcd.println("--------------------------");
  GO.lcd.println("Status: " + onlineStatus);
  GO.lcd.println("Uptime: " + uptime + "s");
  GO.lcd.println("Max RAM: " + maxmem);
  GO.lcd.println("Max Disk: " + maxdisk);
  
}

void printVMStats(JsonObject vm) {
  String name = vm["name"].as<String>();
  String id = vm["vmid"].as<String>();
  int cpu = round(vm["cpu"].as<float>() * 100);
  int cores = vm["cpus"].as<int>();
  String onlineStatus = vm["status"].as<String>();
  String mem = vm["mem"].as<String>();
  String maxmem = vm["maxmem"].as<String>();
  String maxdisk = vm["maxdisk"].as<String>();
  String uptime = vm["uptime"].as<String>();
  String netin = vm["netin"].as<String>();
  String netout = vm["netout"].as<String>();
  // these are strings for now because they could be way too big

  GO.lcd.fillScreen(WHITE);
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(BLACK);

  GO.lcd.setTextSize(3);
  GO.lcd.println(id + ": " + name);
  GO.lcd.setTextSize(2);
  GO.lcd.println("--------------------------");
  GO.lcd.println("Status: " + onlineStatus);
  GO.lcd.println("Uptime: " + uptime + "s");
  GO.lcd.println("CPU: " + String(cpu) + "%");
  GO.lcd.println("Cores: " + String(cores));
  GO.lcd.println("RAM: " + mem);
  GO.lcd.println("Max RAM: " + maxmem);
  GO.lcd.println("Max Disk: " + maxdisk);
  GO.lcd.println("Net In: " + netin);
  GO.lcd.println("Net Out: " + netout);
  
  
}


void nodeInfo() {
  Serial.println("nodeinfo");
  while (true) {
    printNodeStats(getNode(selectedNode, getNodeInfo()));
    delay(2000);
  }
}

void containerInfo() {
  Serial.println("container info");
  JsonArray containerArray = getContainerInfo(selectedNode);
  selectedItem = 0;
  int selectedLXC = listContainers(containerArray);
  JsonObject container = getContainer(selectedLXC, containerArray);
  printContainerStats(container);
  delay(1000);

  // only redraw screen if changed
  while (true) {
    if (getContainer(selectedLXC, containerArray) != container) {
      containerInfo();
    }
  }
  
}

void vmInfo() {
  Serial.println("vm info");
  JsonArray vmArray = getVMInfo(selectedNode);
  selectedItem = 0;
  int selectedVM = listContainers(vmArray);
  Serial.println("vm");
  Serial.println(selectedVM);
  Serial.println(selectedVM);
  Serial.println(selectedVM);
  Serial.println(selectedVM);

  while (true) {
    printVMStats(getContainer(selectedVM, vmArray));
    delay(2000);
  }
  
}