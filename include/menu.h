#ifndef MENU_H_
#define MENU_H_

#include <Arduino.h>
#include <json/retrieve.h>

/// @brief Struct for static menu items such as on the main menu. Has the name of the menu item and a function to run when it is selected.
typedef struct {
  String name;
  void (*function)();
} MenuItem;

// Function declarations
void listNodes(Node* nodes, const int &numItems);
int listContainers(Container* containers, const int &numItems);
int listVMs(VM* vms, const int &numItems);
String listDisks(Disk* disks, const int &numItems);
String listPools(Pool* pools, const int &numItems);
void mainMenu();
void manageContainerMenu();
void manageVMMenu();

/// @brief Template for a callback function to print a menu entry.
typedef void (*MenuPrintCallback)(void*);

/// @brief Variable to hold the currently selected menu item index.
extern int selectedItem;

/// @brief Variable to hold the currently selected page index.
extern int selectedPage;

/// @brief Variable to hold the name of the selected node.
extern String selectedNode;

#endif