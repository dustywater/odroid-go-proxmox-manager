#include <json/retrieve.h>
#include <json/utils.h>
#include <json/send.h>
#include <global.h>
#include <menu.h>
#include <odroid_go.h>
#include <utils.h>

void vmRestart()
{
    Serial.println("vm restart");
    selectedItem = 0;
    selectedVM = 0;
    int numVMs;
    VM *vms = getVMInfo(&numVMs, selectedNode);
    listVMs(vms, numVMs);
    delete[] vms;
    if (selectedVM > 0)
    {
        restartVM(selectedNode, selectedVM);
        GO.lcd.clearDisplay();
        GO.lcd.setCursor(0, 0);
        GO.lcd.println("done");
        delay(2000);   
    }
    manageVMMenu();
}

void containerRestart()
{
    Serial.println("lxc restart");
    selectedItem = 0;
    selectedLXC = 0;
    int numContainers;
    Container *containers = getContainerInfo(&numContainers, selectedNode);
    listContainers(containers, numContainers);
    delete[] containers;
    if (selectedLXC > 0)
    {
        restartContainer(selectedNode, selectedLXC);
        GO.lcd.clearDisplay();
        GO.lcd.setCursor(0, 0);
        GO.lcd.println("done");
        delay(2000);

    }
    manageContainerMenu();
}


void vmStart()
{
    Serial.println("vm start");
    selectedItem = 0;
    selectedVM = 0;
    int numVMs;
    VM *vms = getVMInfo(&numVMs, selectedNode);
    listVMs(vms, numVMs);
    delete[] vms;
    if (selectedVM > 0)
    {
        

        startVM(selectedNode, selectedVM);

        GO.lcd.clearDisplay();
        GO.lcd.println("done");

        delay(2000);
    }
    manageVMMenu();
}

void containerStart()
{
    Serial.println("lxc start");
    selectedItem = 0;
    selectedLXC = 0;
    int numContainers;
    Container *containers = getContainerInfo(&numContainers, selectedNode);
    listContainers(containers, numContainers);
    delete[] containers;
    if (selectedLXC > 0)
    {
        startContainer(selectedNode, selectedLXC);
        GO.lcd.clearDisplay();
        GO.lcd.println("done");

        delay(2000);
    }
    manageContainerMenu();
}

void vmStop()
{
    Serial.println("vm stop");
    selectedItem = 0;
    selectedVM = 0;
    int numVMs;
    VM *vms = getVMInfo(&numVMs, selectedNode);
    listVMs(vms, numVMs);
    delete[] vms;
    if (selectedVM > 0)
    {
        

        stopVM(selectedNode, selectedVM);

        GO.lcd.clearDisplay();
        GO.lcd.println("done");

        delay(2000);
    }
    manageVMMenu();
}

void containerStop()
{
    Serial.println("lxc stop");
    selectedItem = 0;
    selectedLXC = 0;
    int numContainers;
    Container *containers = getContainerInfo(&numContainers, selectedNode);
    listContainers(containers, numContainers);
    delete[] containers;
    if (selectedLXC > 0)
    {
        stopContainer(selectedNode, selectedLXC);
        GO.lcd.clearDisplay();
        GO.lcd.println("done");

        delay(2000);
    }
    manageContainerMenu();
}