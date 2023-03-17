#include <menu.h>
#include <Arduino.h>
#include <odroid_go.h>
#include <ArduinoJson.h>
#include <statistics.h>
#include <global.h>

const int MAIN_TEXT_COLOR = WHITE;
const int MAIN_TEXT_SIZE = 2;


int selectedItem = 0;
String selectedNode = "";
int selectedLXC = 0;
int selectedVM = 0;





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
      Serial.println(selectedItem);
      break;
    }
    if (GO.JOY_Y.isAxisPressed() == 2 && selectedItem > 0) {
      selectedItem--;
      Serial.println(selectedItem);
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
  GO.lcd.setTextColor(MAIN_TEXT_COLOR);
  GO.lcd.setTextSize(MAIN_TEXT_SIZE);
  

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
      break;
  }

}

void mainMenu(){
    int numItems = sizeof(mainMenuItems) / sizeof(MenuItem);
    drawMenu(mainMenuItems, numItems);
}

void listNodes(Node* nodes, int numItems) {
  GO.lcd.clearDisplay();
  GO.lcd.setTextColor(MAIN_TEXT_COLOR);
  GO.lcd.setTextSize(MAIN_TEXT_SIZE);
  GO.lcd.setCursor(0, 0);

  for (int i = 0; i < numItems; i++) {
    if (selectedItem == i) {
      GO.lcd.print("> ");
    }
    GO.lcd.println(nodes[i].name);
  }

  switch (buttonListener(numItems)) {
    case 1:
      Serial.println("selected " + selectedItem);
      selectedNode = nodes[selectedItem].name;
      break;
    case 2:
      Serial.println("back");
      break;
    default:
      listNodes(nodes, numItems);
      break;
  }
}

void listContainers(Container* containers, int numItems) {
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(MAIN_TEXT_COLOR);
  GO.lcd.setTextSize(MAIN_TEXT_SIZE);

  for (int i = 0; i < numItems; i++) {
    if (selectedItem == i) {
      GO.lcd.print("> ");
    }
    GO.lcd.println(String(containers[i].id) + ": " + containers[i].name);
  }

  switch (buttonListener(numItems)) {
    case 1:
      selectedLXC = containers[selectedItem].id;
      break;
    case 2:
      Serial.println("back");
      break;
    default:
      listContainers(containers, numItems);
      break;
  }

}


void listVMs(VM* vms, int numItems) {
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(MAIN_TEXT_COLOR);
  GO.lcd.setTextSize(MAIN_TEXT_SIZE);

  for (int i = 0; i < numItems; i++) {
    if (selectedItem == i) {
      GO.lcd.print("> ");
    }
    GO.lcd.println(String(vms[i].id) + ": " + vms[i].name);
  }

  switch (buttonListener(numItems)) {
    case 1:
      selectedVM = vms[selectedItem].id;
      break;
    case 2:
      Serial.println("back");
      break;
    default:
      listVMs(vms, numItems);
      break;
  }

}
