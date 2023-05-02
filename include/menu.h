#ifndef MENU_H_
#define MENU_H_

#include <Arduino.h>
#include <json/retrieve.h>
// Struct for static menu items such as on the main menu. Has the name of the menu item and a function to run when it is selected.
typedef struct {
  String name;
  void (*function)();
} MenuItem;

void listNodes(Node* nodes, const int &numItems);
int listContainers(Container* containers, const int &numItems);
int listVMs(VM* vms, const int &numItems);
String listDisks(Disk* disks, const int &numItems);
String listPools(Pool* pools, const int &numItems);
void mainMenu();
void manageContainerMenu();
void manageVMMenu();

typedef void (*MenuPrintCallback)(void*);

extern int selectedItem;
extern int selectedPage;

extern String selectedNode;

#endif