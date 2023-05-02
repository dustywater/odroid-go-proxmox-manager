#include <Arduino.h>

#ifndef SERVER_H_
#define SERVER_H_

/**
 * Enter WiFi network and Proxmox information
 * 
*/

/// @brief Address of the Proxmox server being connected to. Do not add a trailing '/'. E.g. https://10.10.10.10:8006
const String PROXMOX_ADDRESS = "";

/// @brief Proxmox token user. E.g. root@pam
const String PROXMOX_TOKEN_USER = "";

/// @brief Name of the Proxmox token.
const String PROXMOX_TOKEN_NAME = "";

/// @brief Proxmox token secret key. E.g. 0ec1060d-a4a0-4975-a624-ea5cc49ee04e
const String PROXMOX_TOKEN_SECRET = "";

#endif