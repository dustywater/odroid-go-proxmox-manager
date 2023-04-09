#include <odroid_go.h>
#include <wifi_info.h>
#include <WiFi.h>
#include <pin.h>
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
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  GO.lcd.print("Connecting");


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    GO.lcd.print(".");
  }

}


int pinListener()
{
  delay(300);
  while (true)
  {
    GO.update();
    if (GO.BtnA.isPressed() == 1)
    {
      return 0;
    }

    if (GO.BtnB.isPressed() == 1)
    {
      return 1;
    }

    if (GO.JOY_Y.isAxisPressed() == 1)
    {
      return 2;
    }

    if (GO.JOY_Y.isAxisPressed() == 2)
    {
      return 3;
    }

    if (GO.JOY_X.isAxisPressed() == 1)
    {
      return 4;
    }

    if (GO.JOY_X.isAxisPressed() == 2)
    {
      return 5;
    }

    if (GO.BtnStart.isPressed() == 1)
    {
      return 6;
    }

    if (GO.BtnMenu.isPressed() == 1)
    {
      return 7;
    }
    delay(50);
  }
  return 7;
}

bool enterPin()
{
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(WHITE);
  GO.lcd.setTextSize(2);
  GO.lcd.println("Enter pin");
  GO.lcd.println("--------------------------");
  GO.lcd.setTextSize(1);
  GO.lcd.println("Start to Confirm | Menu to Reset");
  GO.lcd.setTextSize(2);
  GO.lcd.println("--------------------------");
  bool done = false;
  String pin = "";

  while (!done)
  {
    if (pin != ""){
      GO.lcd.print("*");
    }
    Serial.println(pin);
    switch (pinListener())
    {
    case 6:
      done = true;
      break;
    case 0:
      pin += "0";
      break;
    case 1:
      pin += "1";
      break;
    case 2:
      pin += "2";
      break;
    case 3:
      pin += "3";
      break;
    case 4:
      pin += "4";
      break;
    case 5:
      pin += "5";
      break;
    default:
      pin = "";
      done = true;
      break;
    }
  }

  if (pin == LOCK_PIN) {
    return true;
  }
  Serial.println("incorrect: " + pin);

  if (pin != "") {
    displayError("Incorrect Pin");
  }
  return false;

}