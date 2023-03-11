#include <ArduinoJson.h>
#include <utils.h>

/**
   Get a specific node from an array of nodes
*/
JsonObject getNode(String name, JsonArray nodes) {
  for (JsonObject node : nodes) {
    if (node["node"] == name) {
      return node;
    }
  }
  displayError("Node not found!");

}

JsonObject getContainer(int id, JsonArray containers) {
  for (JsonObject container : containers) {
    Serial.println(id);
    Serial.println(container["vmid"].as<int>());
    if (container["vmid"].as<int>() == id) {
      return container;
    }
  }
  displayError("Machine not found!");

}
