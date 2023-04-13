#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <odroid_go.h>
#include <server.h>
#include <utils.h>
#include <menu.h>
#include <json/retrieve.h>
#include <pin.h>

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
  if (LOCK_PIN != "")
  {
    bool pinCorrect = false;
    while (!pinCorrect)
    {
      pinCorrect = enterPin();
    }
  } else {
    displayError("Warning: No pin set");
  }

  connectWiFi();
  Serial.println("done setup");
}
