#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <server.h>
#include <utils.h>

// todo make the bottom part of these into one function
// ######################################
// ######################################
// ######################################
// ######################################
// ######################################
JsonArray getNodeInfo() {
  HTTPClient http;
  String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/";
  String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
  char authc[auth.length() + 1];
  auth.toCharArray(authc, auth.length() + 1);
  http.begin(apiAddress);
  http.setAuthorization(authc);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String json = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);
    JsonArray nodes = doc["data"].as<JsonArray>();
    return nodes;
  }

  displayError("Error getting data.");
  return getNodeInfo();
}

JsonArray getContainerInfo(String node) {
  HTTPClient http;
  String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/" + node + "/lxc";
  String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
  char authc[auth.length() + 1];
  auth.toCharArray(authc, auth.length() + 1);
  http.begin(apiAddress);
  http.setAuthorization(authc);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String json = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);
    JsonArray nodes = doc["data"].as<JsonArray>();
    return nodes;
  }

  displayError("Error getting data.");
  return getNodeInfo();
}

JsonArray getVMInfo(String node) {
  HTTPClient http;
  String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/" + node + "/qemu";
  String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
  char authc[auth.length() + 1];
  auth.toCharArray(authc, auth.length() + 1);
  http.begin(apiAddress);
  http.setAuthorization(authc);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String json = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);
    JsonArray nodes = doc["data"].as<JsonArray>();
    return nodes;
  }

  displayError("Error getting data.");
  return getNodeInfo();
}
