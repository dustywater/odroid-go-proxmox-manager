#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <server.h>
#include <utils.h>
#include <json/retrieve.h>
#include <stdexcept>

void restartVM(String node, int vmid)
{
    HTTPClient http;
    String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/" + node + "/qemu/" + vmid + "/status/reboot";
    String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
    char authc[auth.length() + 1];
    auth.toCharArray(authc, auth.length() + 1);
    http.begin(apiAddress);
    http.setAuthorization(authc);
    int httpCode = http.POST("");
    http.end();

    if(httpCode == 0) {
        throw std::runtime_error("Can't restart VM. Error code:  " + httpCode);
    }
}

void restartContainer(String node, int containerid)
{
    HTTPClient http;
    String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/" + node + "/lxc/" + containerid + "/status/reboot";
    String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
    char authc[auth.length() + 1];
    auth.toCharArray(authc, auth.length() + 1);
    http.begin(apiAddress);
    http.setAuthorization(authc);
    int httpCode = http.POST("");
    http.end();

    if(httpCode == 0) {
        throw std::runtime_error("Can't restart container. Error code:  " + httpCode);
    }
}

void startVM(String node, int vmid)
{
    HTTPClient http;
    String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/" + node + "/qemu/" + vmid + "/status/start";
    String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
    char authc[auth.length() + 1];
    auth.toCharArray(authc, auth.length() + 1);
    http.begin(apiAddress);
    http.setAuthorization(authc);
    int httpCode = http.POST("");
    http.end();

    if(httpCode == 0) {
        throw std::runtime_error("Can't start VM. Error code:  " + httpCode);
    }
}

void startContainer(String node, int containerid)
{
    HTTPClient http;
    String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/" + node + "/lxc/" + containerid + "/status/start";
    String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
    char authc[auth.length() + 1];
    auth.toCharArray(authc, auth.length() + 1);
    http.begin(apiAddress);
    http.setAuthorization(authc);
    int httpCode = http.POST("");
    http.end();

    if(httpCode == 0) {
        throw std::runtime_error("Can't start container. Error code:  " + httpCode);
    }
}

void stopVM(String node, int vmid)
{
    HTTPClient http;
    String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/" + node + "/qemu/" + vmid + "/status/stop";
    String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
    char authc[auth.length() + 1];
    auth.toCharArray(authc, auth.length() + 1);
    http.begin(apiAddress);
    http.setAuthorization(authc);
    int httpCode = http.POST("");
    http.end();

    if(httpCode == 0) {
        throw std::runtime_error("Can't stop VM. Error code:  " + httpCode);
    }
}

void stopContainer(String node, int containerid)
{
    HTTPClient http;
    String apiAddress = PROXMOX_ADDRESS + "/api2/json/nodes/" + node + "/lxc/" + containerid + "/status/stop";
    String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
    char authc[auth.length() + 1];
    auth.toCharArray(authc, auth.length() + 1);
    http.begin(apiAddress);
    http.setAuthorization(authc);
    int httpCode = http.POST("");
    http.end();

    if(httpCode == 0) {
        throw std::runtime_error("Can't stop container. Error code:  " + httpCode);
    }
}