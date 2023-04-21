#include <menu.h>
#include <Arduino.h>
#include <odroid_go.h>
#include <ArduinoJson.h>
#include <statistics.h>
#include <global.h>
#include <manage.h>

// UI constants for the menu system.
const int MAIN_TEXT_COLOR = WHITE;
const int MAIN_TEXT_SIZE = 2;
const int ITEMS_PER_PAGE = 12;
const int OFFLINE_COLOR = RED;
const int ONLINE_COLOR = GREEN;

// Initializing variables which hold the selections.
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

// Array of the VM managment menu items.
MenuItem manageVMItems[] = {
    {"Restart", &vmRestart},
    {"Start", &vmStart},
    {"Stop", &vmStop},
};

// Array of the container management menu items.
MenuItem manageContainerItems[] = {
    {"Restart", &containerRestart},
    {"Start", &containerStart},
    {"Stop", &containerStop},
};

/// @brief Function to listen for button presses. Once called it will loop until a button is pressed. For the directional pad the selected page or item is changed and 0 is returned. For the A/B buttons 1/2 is returned to indicate that the user wants to select something or go back.
/// @param numItems A reference an integer which holds the number of items for the user to pick from in the menu.
/// @return An integer indicating what action was taken. 1 for the A button, 2 for the B button, 0 for any other input.
int buttonListener(const int &numItems)
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

/** @brief A function to draw a menu on the screen. This is for static menus which have a function tied to each item. 
 * Loops through the menu items and draws a ">" before the currently selected item. 
 * After printing the items, listens for button inputs. If the A button is pressed then the function of the currently selected item is run. If the B button is pressed the function stops running (returning the previous menu).
 * If the directional pad buttons are pressed it means that the selected item or page has changed, so the function is run again to redraw the menu with the new values. 
* @param menuItems An array of menuItems used to draw the menu.
* @param numItems A reference to an int which holds the number of items in the menu.
* @param menuTitle A reference to a string which holds the title of the menu.
**/
void drawMenu(MenuItem menuItems[], const int &numItems, const String &menuTitle)
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
    } else {
      GO.lcd.print("  ");
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

/**
 * @brief Function to draw the main menu. Calls the draw menu function using the array of main menu items.
 */
void mainMenu()
{
  selectedItem = 0;
  int numItems = sizeof(mainMenuItems) / sizeof(MenuItem);
  drawMenu(mainMenuItems, numItems, "Main Menu");
}

/**
 * @brief Function to draw the VM management menu. Calls the draw menu function using the array of VM mangement menu items.
 */
void manageVMMenu()
{
  selectedItem = 0;
  int numItems = sizeof(manageVMItems) / sizeof(MenuItem);
  drawMenu(manageVMItems, numItems, "Manage VM");
  mainMenu();
}

/**
 * @brief Function to draw the container management menu. Calls the draw menu function using the array of container management menu items.
 */
void manageContainerMenu()
{
  selectedItem = 0;
  int numItems = sizeof(manageContainerItems) / sizeof(MenuItem);
  drawMenu(manageContainerItems, numItems, "Manage Container");
  mainMenu();
}

void containerMenuPrint(void *container)
{
  Container *cont = static_cast<Container *>(container);

  if ((*cont).onlineStatus == "running")
  {
    GO.lcd.setTextColor(ONLINE_COLOR);
  }
  else
  {
    GO.lcd.setTextColor(OFFLINE_COLOR);
  }
  GO.lcd.print("O ");
  GO.lcd.setTextColor(MAIN_TEXT_COLOR);
  GO.lcd.println(String((*cont).id) + ": " + (*cont).name);
};

void vmMenuPrint(void *vm)
{
  VM *v = static_cast<VM *>(vm);

  if ((*v).onlineStatus == "running")
  {
    GO.lcd.setTextColor(ONLINE_COLOR);
  }
  else
  {
    GO.lcd.setTextColor(OFFLINE_COLOR);
  }
  GO.lcd.print("O ");
  GO.lcd.setTextColor(MAIN_TEXT_COLOR);
  GO.lcd.println(String((*v).id) + ": " + (*v).name);
};

void nodeMenuPrint(void *node)
{
  Node *n = static_cast<Node *>(node);

  if ((*n).onlineStatus == "online")
  {
    GO.lcd.setTextColor(ONLINE_COLOR);
  }
  else
  {
    GO.lcd.setTextColor(OFFLINE_COLOR);
  }
  GO.lcd.print("O ");
  GO.lcd.setTextColor(MAIN_TEXT_COLOR);
  GO.lcd.println((*n).name);
}

void diskMenuPrint(void *disk)
{
  Disk *d = static_cast<Disk *>(disk);
  GO.lcd.println((*d).devpath);
}

void poolMenuPrint(void *pool)
{
  Pool *p = static_cast<Pool *>(pool);
  GO.lcd.println((*p).name);
}

void listItems(void *items, void **item, const String title, const int &numItems, const int &itemSize, const bool &backEnabled, MenuPrintCallback printMachine)
{
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(MAIN_TEXT_COLOR);
  GO.lcd.setTextSize(MAIN_TEXT_SIZE);
  GO.lcd.println(title + " (Page " + String(selectedPage + 1) + ")");
  GO.lcd.println("--------------------------");

  // Cast items to char for pointer calculation
  char *itemsAsBytes = static_cast<char *>(items);

  for (int i = selectedPage * ITEMS_PER_PAGE; i < numItems && i < (selectedPage + 1) * ITEMS_PER_PAGE; i++)
  {
    if (selectedItem == i)
    {
      GO.lcd.print("> ");
    } else {
      GO.lcd.print("  ");
    }
    printMachine(itemsAsBytes + i * itemSize);
  }

  switch (buttonListener(numItems))
  {
  case 1:
    *item = itemsAsBytes + selectedItem * itemSize;
    break;
  case 2:
    if (backEnabled)
    {
      Serial.println("back");
      break;
    }
  default:
    listItems(items, item, title, numItems, itemSize, backEnabled, printMachine);
    break;
  }
}

void listNodes(Node *nodes, const int &numItems)
{
  void *item;
  listItems(nodes, &item, "Select Node", numItems, sizeof(Node), false, nodeMenuPrint);
  Node *node = static_cast<Node *>(item);
  Serial.println((*node).name);
  selectedNode = (*node).name;
}

void listContainers(Container *containers, const int &numItems)
{
  void *item;
  listItems(containers, &item, "Select Container", numItems, sizeof(Container), true, containerMenuPrint);
  Container *container = static_cast<Container *>(item);
  selectedLXC = (*container).id;
}

void listVMs(VM *vms, const int &numItems)
{
  void *item;
  listItems(vms, &item, "Select VM", numItems, sizeof(VM), true, vmMenuPrint);
  VM *vm = static_cast<VM *>(item);
  selectedVM = (*vm).id;
}

void listDisks(Disk *disks, const int &numItems)
{
  void *item;
  listItems(disks, &item, "Select Disk", numItems, sizeof(Disk), true, diskMenuPrint);
  Disk *disk = static_cast<Disk *>(item);
  selectedDisk = (*disk).devpath;
}

void listPools(Pool *pools, const int &numItems)
{
  void *item;
  listItems(pools, &item, "Select Pool", numItems, sizeof(Pool), true, poolMenuPrint);
  Pool *pool = static_cast<Pool *>(item);
  selectedPool = (*pool).name;
}
