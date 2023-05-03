#include <menu.h>
#include <Arduino.h>
#include <odroid_go.h>
#include <statistics.h>
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
    if (GO.JOY_X.isAxisPressed() == 1 && selectedPage + 1 < (float(numItems) / ITEMS_PER_PAGE))
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
    }
    else
    {
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

/**
 * @brief Callback function to print a container in the menu. Prints an online indicator which is red if offline and green if online.
 * Then prints the id of the container followed by it's name.
 *
 * @param container The container to print.
 */
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

/**
 * @brief Callback function to print a VM in the menu. Prints an online indicator which is red if offline and green if online.
 * Then prints the id of the VM followed by it's name.
 *
 * @param vm The VM to print.
 */
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

/**
 * @brief Callback function to print a node in the menu. Prints an online indicator which is red if offline and green if online.
 * Then prints the name of the node.
 *
 * @param node
 */
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

/**
 * @brief Callback function to print a disk in the menu. Prints the devpath of the disk.
 *
 * @param disk The disk to print
 */
void diskMenuPrint(void *disk)
{
  Disk *d = static_cast<Disk *>(disk);
  GO.lcd.println((*d).devpath);
}

/**
 * @brief Callback function to print a ZFS pool in the menu. Prints the name of the pool.
 *
 * @param pool The pool to print.
 */
void poolMenuPrint(void *pool)
{
  Pool *p = static_cast<Pool *>(pool);
  GO.lcd.println((*p).name);
}

/**
 * @brief Main function to print a menu of items (such as nodes or containers). First prints a title, followed by page number.
 * Then prints the items starting at the page number multiplied by the number of items per page, and stops when the items per page is reached.
 * Prints a ">" on the currently selected item to indicate that it is selected.
 * Then uses the callback function to print the name of whatever type of item is being printed.
 *
 * After printing items listens for input from the user using the ButtonListener() function.
 *
 *
 * @param items The array of items to print.
 * @param item The item selected by the user.
 * @param title The menu title.
 * @param numItems The number of items in the array.
 * @param itemSize The size of each item. Used for pointer calculation.
 * @param backEnabled Whether the back button should be enabled for this menu.
 * @param printMachine The callback function to print the name of the item.
 */
void listItems(
    void *items, void **item,
    const String title,
    const int &numItems,
    const int &itemSize,
    const bool &backEnabled,
    MenuPrintCallback printMachine)
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
    }
    else
    {
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

/**
 * @brief The calling function for listing nodes. Calls the listItems() function with the appropriate parameters for nodes.
 * Passes an item pointer to the function which gets set to whatever node has been selected during the listItems() function.
 * Then sets the item pointer to the selected node.
 *
 * Passes false to the backEnabled parameter as the back button should be disabled on the node selection screen.
 *
 * Passes through nodeMenuPrint() as the callback function so that the correct names are printed.
 *
 * @param nodes The array of nodes being listed.
 * @param numItems The number of nodes being listed.
 */
void listNodes(Node *nodes, const int &numItems)
{
  selectedItem = 0;
  selectedPage = 0;
  void *item;
  listItems(nodes, &item, "Select Node", numItems, sizeof(Node), false, nodeMenuPrint);

  if (item != nullptr)
  {
    Node *node = static_cast<Node *>(item);
    selectedNode = (*node).name;
  }
}

/**
 * @brief The calling function for listing containers. Calls the listItems() function with the appropriate parameters for containers.
 * Passes an item pointer to the function which gets set to whatever container has been selected during the listItems() function.
 * Then sets the item pointer to the selected container.
 *
 * Passes true to the backEnabled parameter as the back button should be enabled on this screen.
 *
 * Passes through containerMenuPrint() as the callback function so that the correct names are printed.
 *
 * @param containers The array of containers to list.
 * @param numItems The number of containers to list.
 * @return The ID of the selected container, or 0 if none selected
 */
int listContainers(Container *containers, const int &numItems)
{
  selectedItem = 0;
  selectedPage = 0;
  void *item;
  listItems(containers, &item, "Select Container", numItems, sizeof(Container), true, containerMenuPrint);

  if (item != nullptr)
  {
    Container *container = static_cast<Container *>(item);
    return (*container).id;
  }
  return 0;
}

/**
 * @brief The calling function for listing VMs. Calls the listItems() function with the appropriate parameters for VMs.
 * Passes an item pointer to the function which gets set to whatever VM has been selected during the listItems() function.
 * Then sets the item pointer to the selected VM.
 *
 * Passes true to the backEnabled parameter as the back button should be enabled on this screen.
 *
 * Passes through vmMenuPrint() as the callback function so that the correct names are printed.
 *
 * @param vms The array of VMs to list.
 * @param numItems The number of VMs to list.
 * @return The ID of the selected VM, or 0 if none selected
 */
int listVMs(VM *vms, const int &numItems)
{
  selectedItem = 0;
  selectedPage = 0;
  void *item;
  listItems(vms, &item, "Select VM", numItems, sizeof(VM), true, vmMenuPrint);
  if (item != nullptr)
  {
    VM *vm = static_cast<VM *>(item);
    return (*vm).id;
  }
  return 0;
}

/**
 * @brief The calling function for listing disks. Calls the listItems() function with the appropriate parameters for disks.
 * Passes an item pointer to the function which gets set to whatever disk has been selected during the listItems() function.
 * Then sets the item pointer to the selected disk.
 *
 * Passes true to the backEnabled parameter as the back button should be enabled on this screen.
 *
 * Passes through diskMenuPrint() as the callback function so that the correct names are printed.
 *
 * @param disks The array of disks to list.
 * @param numItems The number of disks to list.
 * @return The devpath of the chosen disk, or an empty string if nothing has been selected
 */
String listDisks(Disk *disks, const int &numItems)
{
  selectedItem = 0;
  selectedPage = 0;
  void *item;
  listItems(disks, &item, "Select Disk", numItems, sizeof(Disk), true, diskMenuPrint);

  if (item != nullptr)
  {
    Disk *disk = static_cast<Disk *>(item);
    return (*disk).devpath;
  }
  return "";
}

/**
 * @brief The calling function for listing pools. Calls the listItems() function with the appropriate parameters for pools.
 * Passes an item pointer to the function which gets set to whatever pool has been selected during the listItems() function.
 * Then sets the item pointer to the selected pool.
 *
 * Passes true to the backEnabled parameter as the back button should be enabled on this screen.
 *
 * Passes through poolMenuPrint() as the callback function so that the correct names are printed.
 *
 * @param pools The array of pools to list.
 * @param numItems The number of pools to list.
 * @return The name of the chosen pool, or a blank string if no pool selected
 */
String listPools(Pool *pools, const int &numItems)
{
  selectedItem = 0;
  selectedPage = 0;
  void *item;
  listItems(pools, &item, "Select Pool", numItems, sizeof(Pool), true, poolMenuPrint);
  if (item != nullptr)
  {
    Pool *pool = static_cast<Pool *>(item);
    return (*pool).name;
  }
  return "";
}
