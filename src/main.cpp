#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <odroid_go.h>
#include <server.h>
#include <utils.h>
#include <menu.h>
#include <json/retrieve.h>


void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    displayError("WiFi Connection Lost");
    connectWiFi();
  }

  try
  {
    int numNodes;
    Node *nodes = getNodeInfo(&numNodes);
    listNodes(nodes, numNodes);
    delete[] nodes;
    selectedItem = 0;
    selectedPage = 0;
    mainMenu();
  }
  catch (const std::exception &e)
  {
    Serial.println(e.what());
    displayError(e.what());
  }
}

void setup()
{
  Serial.begin(115200);
  GO.begin();
  GO.lcd.setTextWrap(false);
  bool pinCorrect = false;
  while (!pinCorrect) {
    pinCorrect = enterPin();
  }
  connectWiFi();
  Serial.println("done setup");
}

