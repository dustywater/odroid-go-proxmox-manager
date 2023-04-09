#include <menu.h>
#include <Arduino.h>
#include <odroid_go.h>
#include <ArduinoJson.h>
#include <statistics.h>
#include <global.h>
#include <manage.h>

const int MAIN_TEXT_COLOR = WHITE;
const int MAIN_TEXT_SIZE = 2;
const int ITEMS_PER_PAGE = 12;

int selectedItem = 0;
int selectedPage = 0;
String selectedNode = "";
int selectedLXC = 0;
int selectedVM = 0;
String selectedDisk = "";
String selectedPool = "";

// Array of the main menu items
MenuItem mainMenuItems[] = {
    {"Node Information", &nodeInfo},
    {"Container Information", &containerInfo},
    {"VM Information", &vmInfo},
    {"Disk Information", &diskInfo},
    {"ZFS Pool Information", &poolInfo},
    {"Manage VM", &manageVMMenu},
    {"Manage Container", &manageContainerMenu}};


MenuItem manageVMItems[] = {
    {"Restart", &vmRestart},
    {"Start", &vmStart},
    {"Stop", &vmStop},
};

MenuItem manageContainerItems[] = {
    {"Restart", &containerRestart},
    {"Start", &containerStart},
    {"Stop", &containerStop},
};

int buttonListener(int numItems)
{
  delay(300);
  while (true)
  {
    GO.update();
    if (GO.JOY_Y.isAxisPressed() == 1 && selectedItem < (numItems - 1))
    {
      selectedItem++;
      Serial.println(selectedItem);
      break;
    }
    if (GO.JOY_Y.isAxisPressed() == 2 && selectedItem > 0)
    {
      selectedItem--;
      Serial.println(selectedItem);
      break;
    }
    if (GO.JOY_X.isAxisPressed() == 1 && selectedPage + 1 < (numItems / ITEMS_PER_PAGE))
    {
      selectedPage++;
      selectedItem = selectedPage * ITEMS_PER_PAGE;
      Serial.println(selectedPage);
      break;
    }
    if (GO.JOY_X.isAxisPressed() == 2 && selectedPage > 0)
    {
      selectedPage--;
      selectedItem = selectedPage * ITEMS_PER_PAGE;
      Serial.println(selectedPage);
      break;
    }
    if (GO.BtnA.isPressed() == 1)
    {
      delay(200);
      return 1;
    }
    if (GO.BtnB.isPressed() == 1)
    {
      delay(200);
      return 2;
    }
    delay(50);
  }
  return 0;
}

void drawMenu(MenuItem menuItems[], int numItems, String menuTitle)
{
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(MAIN_TEXT_COLOR);
  GO.lcd.setTextSize(MAIN_TEXT_SIZE);
  GO.lcd.println(menuTitle);
  GO.lcd.println("--------------------------");

  for (int i = 0; i < numItems; i++)
  {
    if (selectedItem == i)
    {
      GO.lcd.print("> ");
    }
    GO.lcd.println(menuItems[i].name);
  }

  switch (buttonListener(numItems))
  {
  case 1:
    Serial.println("selected " + selectedItem);
    menuItems[selectedItem].function();
    break;
  case 2:
    Serial.println("back");
    break;
  default:
    drawMenu(menuItems, numItems, menuTitle);
    break;
  }
}

void mainMenu()
{
  int numItems = sizeof(mainMenuItems) / sizeof(MenuItem);
  drawMenu(mainMenuItems, numItems, "Main Menu");
}


void manageVMMenu()
{
  selectedItem = 0;
  int numItems = sizeof(manageVMItems) / sizeof(MenuItem);
  drawMenu(manageVMItems, numItems, "Manage VM");
  mainMenu();
}

void manageContainerMenu()
{
  selectedItem = 0;
  int numItems = sizeof(manageContainerItems) / sizeof(MenuItem);
  drawMenu(manageContainerItems, numItems, "Manage Container");
  mainMenu();
}

void listNodes(Node *nodes, int numItems)
{
  GO.lcd.clearDisplay();
  GO.lcd.setTextColor(MAIN_TEXT_COLOR);
  GO.lcd.setTextSize(MAIN_TEXT_SIZE);
  GO.lcd.setCursor(0, 0);
  GO.lcd.println("Select Node (Page " + String(selectedPage + 1) + ")");
  GO.lcd.println("--------------------------");

  for (int i = selectedPage * ITEMS_PER_PAGE; i < numItems && i < (selectedPage + 1) * ITEMS_PER_PAGE; i++)
  {
    Serial.println("running loop");
    if (selectedItem == i)
    {
      GO.lcd.print("> ");
    }
    GO.lcd.println(nodes[i].name);
  }
  Serial.println("completed loop");
  switch (buttonListener(numItems))
  {
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

void listContainers(Container *containers, int numItems)
{
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(MAIN_TEXT_COLOR);
  GO.lcd.setTextSize(MAIN_TEXT_SIZE);
  GO.lcd.println("Select Container (Page " + String(selectedPage + 1) + ")");
  GO.lcd.println("--------------------------");

  for (int i = selectedPage * ITEMS_PER_PAGE; i < numItems && i < (selectedPage + 1) * ITEMS_PER_PAGE; i++)
  {
    if (selectedItem == i)
    {
      GO.lcd.print("> ");
    }
    GO.lcd.println(String(containers[i].id) + ": " + containers[i].name);
  }

  switch (buttonListener(numItems))
  {
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

void listVMs(VM *vms, int numItems)
{
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(MAIN_TEXT_COLOR);
  GO.lcd.setTextSize(MAIN_TEXT_SIZE);
  GO.lcd.println("Select VM (Page " + String(selectedPage + 1) + ")");
  GO.lcd.println("--------------------------");

  for (int i = selectedPage * ITEMS_PER_PAGE; i < numItems && i < (selectedPage + 1) * ITEMS_PER_PAGE; i++)
  {
    if (selectedItem == i)
    {
      GO.lcd.print("> ");
    }
    GO.lcd.println(String(vms[i].id) + ": " + vms[i].name);
  }

  switch (buttonListener(numItems))
  {
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

void listDisks(Disk *disks, int numItems)
{
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(MAIN_TEXT_COLOR);
  GO.lcd.setTextSize(MAIN_TEXT_SIZE);
  GO.lcd.println("Select Disk (Page " + String(selectedPage + 1) + ")");
  GO.lcd.println("--------------------------");

  for (int i = selectedPage * ITEMS_PER_PAGE; i < numItems && i < (selectedPage + 1) * ITEMS_PER_PAGE; i++)
  {
    if (selectedItem == i)
    {
      GO.lcd.print("> ");
    }
    GO.lcd.println(disks[i].devpath);
  }

  switch (buttonListener(numItems))
  {
  case 1:
    selectedDisk = disks[selectedItem].devpath;
    break;
  case 2:
    Serial.println("back");
    break;
  default:
    listDisks(disks, numItems);
    break;
  }
}

void listPools(Pool *pools, int numItems)
{
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(MAIN_TEXT_COLOR);
  GO.lcd.setTextSize(MAIN_TEXT_SIZE);
  GO.lcd.println("Select Pool (Page " + String(selectedPage + 1) + ")");
  GO.lcd.println("--------------------------");

  for (int i = selectedPage * ITEMS_PER_PAGE; i < numItems && i < (selectedPage + 1) * ITEMS_PER_PAGE; i++)
  {
    if (selectedItem == i)
    {
      GO.lcd.print("> ");
    }
    GO.lcd.println(pools[i].name);
  }

  switch (buttonListener(numItems))
  {
  case 1:
    selectedPool = pools[selectedItem].name;
    break;
  case 2:
    Serial.println("back");
    break;
  default:
    listPools(pools, numItems);
    break;
  }
}