#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <odroid_go.h>

const char* WIFI_SSID = "";
const char* WIFI_PASS = "";
const String PROXMOX_ADDRESS = "";
const String PROXMOX_TOKEN_USER = "";
const String PROXMOX_TOKEN_NAME = "";
const String PROXMOX_TOKEN_SECRET = "";

int selectedItem = 0;

String selectedNode;
// These ones are int as we use the ID instead of the name
int selectedVM;
int selectedLXC;


void listNodes(JsonArray nodes) {
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  int numItems = nodes.size();
  for (int i = 0; i < numItems; i++) {
    if (selectedItem == i) {
      GO.lcd.print("> ");
    }
    GO.lcd.println(nodes[i]["node"].as<String>());
  }

  switch (buttonListener(numItems)) {
    case 1:
      Serial.println("selected " + selectedItem);
      selectedNode = nodes[selectedItem]["node"].as<String>();
      break;
    case 2:
      Serial.println("back");
      break;
    default:
      listNodes(nodes);
  }
}

void listContainers(JsonArray containers) {
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  int numItems = containers.size();
  for (int i = 0; i < numItems; i++) {
    if (selectedItem == i) {
      GO.lcd.print("> ");
    }
    GO.lcd.println(containers[i]["vmid"].as<String>() + ": " + containers[i]["name"].as<String>());
  }

  switch (buttonListener(numItems)) {
    case 1:
      Serial.println("selected " + selectedItem);
      selectedLXC = containers[selectedItem]["vmid"].as<int>();
      break;
    case 2:
      Serial.println("back");
      break;
    default:
      listContainers(containers);
  }
}

void listVMs(JsonArray vms) {
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  int numItems = vms.size();
  for (int i = 0; i < numItems; i++) {
    if (selectedItem == i) {
      GO.lcd.print("> ");
    }
    GO.lcd.println(vms[i]["vmid"].as<String>() + ": " + vms[i]["name"].as<String>());
  }

  switch (buttonListener(numItems)) {
    case 1:
      Serial.println("selected " + selectedItem);
      selectedVM = vms[selectedItem]["vmid"].as<int>();
      break;
    case 2:
      Serial.println("back");
      break;
    default:
      listVMs(vms);
  }
}

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


void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  GO.lcd.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    GO.lcd.print(".");
  }

}

// Struct for static menu items such as on the main menu. Has the name of the menu item and a function to run when it is selected.
typedef struct {
  char *name;
  void (*function)();
} MenuItem;

// Array of the main menu items
MenuItem mainMenuItems[] = {
  {"Node Information", &nodeInfo},
  {"Container Information", &containerInfo},
  {"VM Information", &vmInfo}
};



void nodeInfo() {
  Serial.println("nodeinfo");
  while (true) {
    printNodeStats(getNode(selectedNode, getNodeInfo()));
    //printAllNodeCPU(getNodeInfo());
    delay(2000);
  }
}

void containerInfo() {
  Serial.println("container info");
  JsonArray containerArray = getContainerInfo(selectedNode);
  selectedItem = 0;
  listContainers(containerArray);
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
  listVMs(vmArray);
  while (true) {
    printVMStats(getContainer(selectedVM, vmArray));
    delay(2000);
  }
  
}

void mainMenu() {
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  int numItems = sizeof(mainMenuItems) / sizeof(MenuItem);

  for (int i = 0; i < numItems; i++) {
    if (selectedItem == i) {
      GO.lcd.print("> ");
    }
    GO.lcd.println(mainMenuItems[i].name);
  }

  switch (buttonListener(numItems)) {
    case 1:
      Serial.println("selected " + selectedItem);
      mainMenuItems[selectedItem].function();
      break;
    case 2:
      Serial.println("back");
      break;
    default:
      mainMenu();
  }

}

int buttonListener(int numItems) {
  delay(200);
  while (true) {
    GO.update();
    if (GO.JOY_Y.isAxisPressed() == 1 && selectedItem < (numItems - 1)) {
      selectedItem++;
      break;
    }
    if (GO.JOY_Y.isAxisPressed() == 2 && selectedItem > 0) {
      selectedItem--;
      break;
    }
    if (GO.BtnA.isPressed() == 1) {
      delay(300);
      return 1;
    }
    if (GO.BtnB.isPressed() == 1) {
      delay(300);
      return 2;
    }

  }
  return 0;
}

// todo make the bottom part of these into one function
// ######################################
// ######################################
// ######################################
// ######################################
// ######################################
JsonArray getNodeInfo() {
  HTTPClient http;
  String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/";
  String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
  char authc[auth.length() + 1];
  auth.toCharArray(authc, auth.length() + 1);
  http.begin(apiAddress);
  http.setAuthorization(authc);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String json = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);
    JsonArray nodes = doc["data"].as<JsonArray>();
    return nodes;
  }

  displayError("Error getting data.");
  return getNodeInfo();
}

JsonArray getContainerInfo(String node) {
  HTTPClient http;
  String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/" + node + "/lxc";
  String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
  char authc[auth.length() + 1];
  auth.toCharArray(authc, auth.length() + 1);
  http.begin(apiAddress);
  http.setAuthorization(authc);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String json = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);
    JsonArray nodes = doc["data"].as<JsonArray>();
    return nodes;
  }

  displayError("Error getting data.");
  return getNodeInfo();
}

JsonArray getVMInfo(String node) {
  HTTPClient http;
  String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/" + node + "/qemu";
  String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
  char authc[auth.length() + 1];
  auth.toCharArray(authc, auth.length() + 1);
  http.begin(apiAddress);
  http.setAuthorization(authc);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String json = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);
    JsonArray nodes = doc["data"].as<JsonArray>();
    return nodes;
  }

  displayError("Error getting data.");
  return getNodeInfo();
}


/**
   Get a specific node from an array of nodes
*/
JsonObject getNode(String name, JsonArray nodes) {
  for (JsonObject node : nodes) {
    if (node["node"] == name) {
      return node;
    }
  }
  displayError("Node not found!");

}

JsonObject getContainer(int id, JsonArray containers) {
  for (JsonObject container : containers) {
    Serial.println(id);
    Serial.println(container["vmid"].as<int>());
    if (container["vmid"].as<int>() == id) {
      return container;
    }
  }
  displayError("Machine not found!");

}


/**
   Display an error on the screen
*/
void displayError(String message) {
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextSize(2);
  GO.lcd.setTextColor(RED);
  GO.lcd.print(message);
  delay(3000);
}


void printAllNodeCPU(JsonArray nodes) {
  GO.lcd.fillScreen(WHITE);
  GO.lcd.setCursor(0, 0);
  for (JsonObject node : nodes) {
    Serial.print("CPU Usage for node ");
    Serial.print(node["node"].as<String>());
    Serial.print(": ");
    Serial.println(String(round(node["cpu"].as<float>() * 100)) + "%");

    GO.lcd.setTextSize(2);
    GO.lcd.setTextColor(BLACK);
    GO.lcd.print("CPU Usage for node ");
    GO.lcd.print(node["node"].as<String>());
    GO.lcd.println(": ");
    GO.lcd.println(String(round(node["cpu"].as<float>() * 100)) + "%");
  }
}


void setup() {
  Serial.begin(115200);
  GO.begin();
  connectWiFi();
}


void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    displayError("WiFi Connection Lost");
    connectWiFi();
  }
  listNodes(getNodeInfo());
  selectedItem = 0;
  mainMenu();

  
}
