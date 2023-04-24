#include <odroid_go.h>
#include <wifi_info.h>
#include <WiFi.h>
#include <pin.h>
#include <json/retrieve.h>


/**
 * @brief Function to display an error on the screen. Displays the error in red text for 3 seconds before continuing.
 * 
 * @param message The message to display.
 */
void displayError(String message) {
  GO.lcd.clearDisplay();
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextSize(2);
  GO.lcd.setTextColor(RED);
  GO.lcd.print(message);
  delay(3000);
}

/**
 * @brief Function to connect to a WiFi network. Attempts to connect to the network using the SSID and password defined in the header file. 
 * Displays a connecting message until the network has connected.
 * 
 */
void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  GO.lcd.clearDisplay();
  GO.lcd.setTextColor(WHITE);
  GO.lcd.setTextSize(2);
  GO.lcd.setCursor(0,0);
  GO.lcd.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    GO.lcd.print(".");
  }

}

/**
 * @brief Function to listen for input when entering a pin. Waits for the user to press a button and then returns a value based on the button they have pressed.
 * 
 * @return int The value indicating which button was pressed.
 */
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

/**
 * @brief The main function for the pin system. Uses the pinListener() function to listen for input. Adds the user's input to a string. 
 * Once the user is done and a start button press is detected, compares the string of the entered pin to the one defines in the header file.
 * If the menu button is pressed the function sets the entered pin to blank and does not show an incorrect pin error. This will mean that the input is reset and the function starts again.
 * 
 * @return true The entered pin is correct.
 * @return false The entered pin is incorrect.
 */
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
