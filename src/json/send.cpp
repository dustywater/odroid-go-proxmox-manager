#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <server.h>
#include <utils.h>
#include <json/retrieve.h>
#include <stdexcept>

/// @brief This is a function to send data to the API. Used for commands such as restarting. It also supports including a post payload to send additional information the API.
/// @param apiPath Refrence to a string which holds the appropriate path on the API to send data.
/// @param post Reference to a string which contains a payload to send to the API.
void apiSend(const String &apiPath, const String &post) {
    HTTPClient http;
    String apiAddress = PROXMOX_ADDRESS + apiPath;
    String auth = "PVEAPIToken=" + PROXMOX_TOKEN_USER + "!" + PROXMOX_TOKEN_NAME + "=" + PROXMOX_TOKEN_SECRET;
    char authc[auth.length() + 1];
    auth.toCharArray(authc, auth.length() + 1);
    http.begin(apiAddress);
    http.setAuthorization(authc);
    int httpCode = http.POST("");
    http.end();

    if(httpCode == 0) {
        throw std::runtime_error("Can't stop. Error code:  " + httpCode);
    }
}

/// @brief The function to restart a given VM. Calls the apiSend() function with the appropriate parameters to restart a VM.
/// @param node A reference to a string which contains the name of the node that is running the VM that is being restarted.
/// @param vmid A reference to an integer which holds the ID of the VM being restarted.
void restartVM(const String &node, const int &vmid)
{
    String apiPath = "/api2/json/nodes/" + node + "/qemu/" + vmid + "/status/reboot";
    apiSend(apiPath, "");
}

/// @brief The function to restart a given container. Calls the apiSend() function with the appropriate parameters to restart a container.
/// @param node A reference to a string which contains the name of the node that is running the container that is being restarted.
/// @param containerid A reference to an integer which holds the ID of the container being restarted.
void restartContainer(const String &node, const int &containerid)
{
    String apiPath = "/api2/json/nodes/" + node + "/lxc/" + containerid + "/status/reboot";
    apiSend(apiPath, "");
}

/// @brief The function to start a given VM. Calls the apiSend() function with the appropriate parameters to start a VM.
/// @param node A reference to a string which contains the name of the node that is running the VM that is being started.
/// @param vmid A reference to an integer which holds the ID of the VM being started.
void startVM(const String &node, const int &vmid)
{
    String apiPath = "/api2/json/nodes/" + node + "/qemu/" + vmid + "/status/start";
    apiSend(apiPath, "");
}

/// @brief The function to start a given container. Calls the apiSend() function with the appropriate parameters to start a container.
/// @param node A reference to a string which contains the name of the node that is running the container that is being started.
/// @param containerid A reference to an integer which holds the ID of the container being started.
void startContainer(const String &node, const int &containerid)
{
    String apiPath = "/api2/json/nodes/" + node + "/lxc/" + containerid + "/status/start";
    apiSend(apiPath, "");

}

/// @brief The function to stop a given VM. Calls the apiSend() function with the appropriate parameters to stop a VM.
/// @param node A reference to a string which contains the name of the node that is running the VM that is being stopped.
/// @param vmid A reference to an integer which holds the ID of the VM being stopped.
void stopVM(const String &node, const int &vmid)
{
    String apiPath =  "/api2/json/nodes/" + node + "/qemu/" + vmid + "/status/stop";
    apiSend(apiPath, "");
}

/// @brief The function to stop a given container. Calls the apiSend() function with the appropriate parameters to stop a container.
/// @param node A reference to a string which contains the name of the node that is running the container that is being stopped.
/// @param containerid A reference to an integer which holds the ID of the container being stopped.
void stopContainer(const String &node, const int &containerid)
{
    String apiPath = "/api2/json/nodes/" + node + "/lxc/" + containerid + "/status/stop";
    apiSend(apiPath, "");
}
