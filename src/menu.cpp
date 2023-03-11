#include <menu.h>
#include <Arduino.h>
#include <odroid_go.h>
#include <ArduinoJson.h>
#include <global.h>
#include <statistics.h>

int selectedItem = 0;
String selectedNode = "";



// Array of the main menu items
MenuItem mainMenuItems[] = {
  {"Node Information", &nodeInfo},
  {"Container Information", &containerInfo},
  {"VM Information", &vmInfo}
};


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

void drawMenu(MenuItem menuItems[], int numItems) {
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  

  for (int i = 0; i < numItems; i++) {
    if (selectedItem == i) {
      GO.lcd.print("> ");
    }
    GO.lcd.println(menuItems[i].name);
  }

  switch (buttonListener(numItems)) {
    case 1:
      Serial.println("selected " + selectedItem);
      menuItems[selectedItem].function();
      break;
    case 2:
      Serial.println("back");
      break;
    default:
      drawMenu(menuItems, numItems);
  }

}

void mainMenu(){
    int numItems = sizeof(mainMenuItems) / sizeof(MenuItem);
    drawMenu(mainMenuItems, numItems);
}

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

int listContainers(JsonArray containers) {
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  int numItems = containers.size();
  int selectedLXC = 0;
  for (int i = 0; i < numItems; i++) {
    if (selectedItem == i) {
      GO.lcd.print("> ");
    }
    GO.lcd.println(containers[i]["vmid"].as<String>() + ": " + containers[i]["name"].as<String>());
  }

  switch (buttonListener(numItems)) {
    case 1:
      selectedLXC = containers[selectedItem]["vmid"].as<int>();
      Serial.println(selectedLXC);
      break;
    case 2:
      Serial.println("back");
      break;
    default:
      listContainers(containers);
  }

  Serial.println(selectedLXC);
  Serial.println("end");
  return selectedLXC;
  
}
