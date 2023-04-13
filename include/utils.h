#ifndef UTILS_H
#define UTILS_H
#include <Arduino.h>

void displayError(String message);
void connectWiFi();
bool enterPin();
int pinListener();
#endif /* UTILS_H */
