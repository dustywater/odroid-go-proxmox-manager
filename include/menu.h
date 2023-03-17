#ifndef MENU_H_
#define MENU_H_
#include <ArduinoJson.h>
#include <Arduino.h>
#include <json/retrieve.h>
// Struct for static menu items such as on the main menu. Has the name of the menu item and a function to run when it is selected.
typedef struct {
  String name;
  void (*function)();
} MenuItem;

void listNodes(Node* nodes, int numItems);
void listContainers(Container* containers, int numItems);
void listVMs(VM* vms, int numItems);
void mainMenu();

extern int selectedItem;

#endif