#include <odroid_go.h>
#include <wifi_info.h>
#include <WiFi.h>
/**
   Display an error on the screen
*/
void displayError(String message) {
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextSize(2);
  GO.lcd.setTextColor(RED);
  GO.lcd.print(message);
  delay(3000);
}

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  GO.lcd.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    GO.lcd.print(".");
  }

}