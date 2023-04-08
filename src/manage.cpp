#include <json/retrieve.h>
#include <json/utils.h>
#include <json/send.h>
#include <global.h>
#include <menu.h>
#include <odroid_go.h>
#include <utils.h>

void nodeRestart()
{
    Serial.println("node restart");
    selectedItem = 0;

    restartNode(selectedNode);

    GO.lcd.clearDisplay();
    GO.lcd.println("done");

    delay(2000);
}

void vmRestart()
{
    Serial.println("vm restart");
    selectedItem = 0;
    int numVMs;
    VM *vms = getVMInfo(&numVMs, selectedNode);
    if (vms != NULL)
    {
        listVMs(vms, numVMs);

        restartVM(selectedNode, selectedVM);

        GO.lcd.clearDisplay();
        GO.lcd.println("done");

        delay(2000);
    }
}

void containerRestart()
{
    Serial.println("lxc restart");
    selectedItem = 0;
    int numContainers;
    Container *containers = getContainerInfo(&numContainers, selectedNode);
    if (containers != NULL)
    {
        listContainers(containers, numContainers);

        restartContainer(selectedNode, selectedLXC);
        GO.lcd.clearDisplay();
        GO.lcd.println("done");

        delay(2000);
    }
}


// void nodeStart()
// {
//     Serial.println("node start");
//     selectedItem = 0;

//     startNode(selectedNode);

//     GO.lcd.clearDisplay();
//     GO.lcd.println("done");

//     delay(2000);
// }

// void vmStart()
// {
//     Serial.println("vm start");
//     selectedItem = 0;
//     int numVMs;
//     VM *vms = getVMInfo(&numVMs, selectedNode);
//     if (vms != NULL)
//     {
//         listVMs(vms, numVMs);

//         startVM(selectedNode, selectedVM);

//         GO.lcd.clearDisplay();
//         GO.lcd.println("done");

//         delay(2000);
//     }
// }

// void containerStart()
// {
//     Serial.println("lxc start");
//     selectedItem = 0;
//     int numContainers;
//     Container *containers = getContainerInfo(&numContainers, selectedNode);
//     if (containers != NULL)
//     {
//         listContainers(containers, numContainers);

//         startContainer(selectedNode, selectedLXC);
//         GO.lcd.clearDisplay();
//         GO.lcd.println("done");

//         delay(2000);
//     }
// }


// void nodeStop()
// {
//     Serial.println("node stop");
//     selectedItem = 0;

//     stopNode(selectedNode);

//     GO.lcd.clearDisplay();
//     GO.lcd.println("done");

//     delay(2000);
// }

// void vmStop()
// {
//     Serial.println("vm stop");
//     selectedItem = 0;
//     int numVMs;
//     VM *vms = getVMInfo(&numVMs, selectedNode);
//     if (vms != NULL)
//     {
//         listVMs(vms, numVMs);

//         stopVM(selectedNode, selectedVM);

//         GO.lcd.clearDisplay();
//         GO.lcd.println("done");

//         delay(2000);
//     }
// }

// void containerStop()
// {
//     Serial.println("lxc stop");
//     selectedItem = 0;
//     int numContainers;
//     Container *containers = getContainerInfo(&numContainers, selectedNode);
//     if (containers != NULL)
//     {
//         listContainers(containers, numContainers);

//         stopContainer(selectedNode, selectedLXC);
//         GO.lcd.clearDisplay();
//         GO.lcd.println("done");

//         delay(2000);
//     }
// }