/*
 * 4x4 Matrix Keypad Module
 *
 * Wiring (Keypad → ESP32):
 *   Row1-R4 → GPIOs 32, 33, 25, 26   (outputs)
 *   Col1-C4 → GPIOs 27, 14, 12, 13   (inputs with pull-up)
 *
 * Key layout:
 *   1  2  3  A
 *   4  5  6  B
 *   7  8  9  C
 *   *  0  #  D
 *
 *  * = Clear/Backspace
 *  # = Submit/Enter
 */

#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Adjust these pin numbers to match your actual wiring
byte rowPins[ROWS] = {32, 33, 25, 26};
byte colPins[COLS] = {27, 14, 12, 13};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void keypadSetup() {
  Serial.println("⌨️  Keypad 4x4 initialized");
}

/*
 * Blocking function — waits for 4 numeric digits then '#' to confirm.
 * '*' clears the buffer.
 * Returns the 4-digit string, or "" on timeout/error.
 */
String keypadGetOTP() {
  String input = "";
  unsigned long startTime = millis();
  const unsigned long KEYPAD_TIMEOUT = 60000; // 60 sec

  Serial.println("⌨️  Waiting for OTP on keypad...");

  while (millis() - startTime < KEYPAD_TIMEOUT) {
    char key = keypad.getKey();
    if (!key) continue;

    Serial.println("Key pressed: " + String(key));

    if (key == '#') {
      // Submit
      if (input.length() == 4) {
        Serial.println("OTP submitted: " + input);
        return input;
      } else {
        Serial.println("⚠️  Need exactly 4 digits. Got: " + input);
      }
    } else if (key == '*') {
      // Clear
      input = "";
      Serial.println("🔴 OTP cleared");
      lcdMsg("Enter OTP:", "");
    } else if (isDigit(key) && input.length() < 4) {
      input += key;
      // Show masked input on LCD
      String masked = "";
      for (int i = 0; i < input.length(); i++) masked += "*";
      lcdMsg("OTP: " + masked, "#=Submit *=Clear");
    }
  }

  Serial.println("⏱ Keypad timeout");
  return "";
}
