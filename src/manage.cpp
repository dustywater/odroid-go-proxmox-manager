#include <json/retrieve.h>
#include <json/utils.h>
#include <json/send.h>
#include <menu.h>
#include <odroid_go.h>
#include <utils.h>

/// @brief User interface function to restart a VM. Retrieves an array of all VMs on the selected node, calls the list function to allow the user to pick a VM and then calls the restartVM() function on that VM to send the request to the API. Returns to the manage VM menu afterwards.
void vmRestart()
{
    Serial.println("vm restart");
    selectedItem = 0;

    int numVMs;
    VM *vms = getVMInfo(numVMs, selectedNode);
    int selectedVM = listVMs(vms, numVMs);
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

/// @brief User interface function to restart a container. Retrieves an array of all containers on the selected node, calls the list function to allow the user to pick a container and then calls the restartContainer() function on that container to send the request to the API. Returns to the manage container menu afterwards.
void containerRestart()
{
    Serial.println("lxc restart");
    selectedItem = 0;
    int numContainers;
    Container *containers = getContainerInfo(numContainers, selectedNode);
    int selectedLXC = listContainers(containers, numContainers);
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

/// @brief User interface function to start a VM. Retrieves an array of all VMs on the selected node, calls the list function to allow the user to pick a VM and then calls the startVM() function on that VM to send the request to the API. Returns to the manage VM menu afterwards.
void vmStart()
{
    Serial.println("vm start");
    selectedItem = 0;
    int numVMs;
    VM *vms = getVMInfo(numVMs, selectedNode);
    int selectedVM = listVMs(vms, numVMs);
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

/// @brief User interface function to start a container. Retrieves an array of all containers on the selected node, calls the list function to allow the user to pick a container and then calls the startContainer() function on that container to send the request to the API. Returns to the manage container menu afterwards.
void containerStart()
{
    Serial.println("lxc start");
    selectedItem = 0;
    int numContainers;
    Container *containers = getContainerInfo(numContainers, selectedNode);
    int selectedLXC = listContainers(containers, numContainers);
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

/// @brief User interface function to stop a VM. Retrieves an array of all VMs on the selected node, calls the list function to allow the user to pick a VM and then calls the startVM() function on that VM to send the request to the API. Returns to the manage VM menu afterwards.

void vmStop()
{
    Serial.println("vm stop");
    selectedItem = 0;
    int numVMs;
    VM *vms = getVMInfo(numVMs, selectedNode);
    int selectedVM = listVMs(vms, numVMs);
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

/// @brief User interface function to stop a container. Retrieves an array of all containers on the selected node, calls the list function to allow the user to pick a container and then calls the stopContainer() function on that container to send the request to the API. Returns to the manage container menu afterwards.
void containerStop()
{
    Serial.println("lxc stop");
    selectedItem = 0;
    int numContainers;
    Container *containers = getContainerInfo(numContainers, selectedNode);
    int selectedLXC = listContainers(containers, numContainers);
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