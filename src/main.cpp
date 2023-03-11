#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <odroid_go.h>
#include <server.h>
#include <utils.h>
#include <menu.h>
#include <json/retrieve.h>

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