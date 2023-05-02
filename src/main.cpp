#include <Arduino.h>
#include <WiFi.h>
#include <odroid_go.h>
#include <server.h>
#include <utils.h>
#include <menu.h>
#include <json/retrieve.h>
#include <pin.h>

void loop()
{
  // Check if still connected to a WiFi network. If not then display this to the user and then attempt to reconnect.
  if (WiFi.status() != WL_CONNECTED)
  {
    displayError("WiFi Connection Lost");
    connectWiFi();
  }


  //Try catch to handle errors in the program. If an error is found it is displayed to the user before the program continues.
  try
  {
    // Display the list of nodes for the user to choose from. 
    int numNodes;
    Node *nodes = getNodeInfo(numNodes);
    listNodes(nodes, numNodes);
    delete[] nodes;

    // Reset menu selection variables before continuing to the main menu.
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

  /**
   * Prompt user to enter pin if one is set. Will only continue if the pin is correct.
   * If a pin has not been set then warn the user before continuing.
   **/ 
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

  // Connect to the configured WiFi network.
  connectWiFi();
}
