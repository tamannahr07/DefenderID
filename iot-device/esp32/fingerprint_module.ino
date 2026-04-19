/*
 * Fingerprint Module — AS608 / R307 via UART (Serial2)
 *
 * Wiring (Sensor → ESP32):
 *   Red  (VCC)  → 3.3V or 5V (check your sensor datasheet)
 *   Black (GND) → GND
 *   Yellow (TX) → ESP32 GPIO16 (FP_RX_PIN)
 *   Green  (RX) → ESP32 GPIO17 (FP_TX_PIN)
 *   White (touched) → optional interrupt pin
 *
 * Note: If using 5V sensor with 3.3V ESP32, use a logic level
 *       converter on the data lines.
 *
 * Enrollment: Set ENROLL_MODE true, upload, follow serial prompts,
 *             then set back to false for normal operation.
 */

#include <Adafruit_Fingerprint.h>

#define ENROLL_MODE false   // Set to true to enroll new fingerprints
#define ENROLL_ID   1       // Template slot ID to enroll into (1-127)

HardwareSerial fpSerial(2); // UART2
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fpSerial);

void fingerprintSetup() {
  fpSerial.begin(57600, SERIAL_8N1, FP_RX_PIN, FP_TX_PIN);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("🖐 Fingerprint sensor found and verified.");
    Serial.print("   Enrolled fingerprints: ");
    finger.getTemplateCount();
    Serial.println(finger.templateCount);
  } else {
    Serial.println("⚠️  Fingerprint sensor not found! Check wiring.");
  }

  if (ENROLL_MODE) {
    Serial.println("─── ENROLLMENT MODE ───");
    enrollFingerprint(ENROLL_ID);
  }
}

/*
 * Attempt to match a placed finger.
 * Returns:
 *   0   = Match found (verified)
 *   1   = No match
 *   255 = No finger placed (sensor timeout)
 */
uint8_t fingerprintVerify() {
  uint8_t p = finger.getImage();
  if (p == FINGERPRINT_NOFINGER) return 255;
  if (p != FINGERPRINT_OK) {
    Serial.println("🖐 Image error: " + String(p));
    return 1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    Serial.println("🖐 Conversion error");
    return 1;
  }

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("🖐 Match found! ID=" + String(finger.fingerID) +
                   " Confidence=" + String(finger.confidence));
    return 0;
  }

  Serial.println("🖐 No match found.");
  return 1;
}

/*
 * Enroll a fingerprint into slot 'id'
 * Call from setup() when ENROLL_MODE is true.
 */
void enrollFingerprint(uint8_t id) {
  Serial.println("Enrolling ID #" + String(id));
  Serial.println("Place finger on sensor...");

  while (finger.getImage() != FINGERPRINT_OK) delay(100);
  if (finger.image2Tz(1) != FINGERPRINT_OK) { Serial.println("ERR image2tz 1"); return; }
  Serial.println("Remove finger");
  delay(2000);
  while (finger.getImage() != FINGERPRINT_NOFINGER) delay(100);
  Serial.println("Place same finger again...");
  while (finger.getImage() != FINGERPRINT_OK) delay(100);
  if (finger.image2Tz(2) != FINGERPRINT_OK) { Serial.println("ERR image2tz 2"); return; }
  if (finger.createModel() != FINGERPRINT_OK) { Serial.println("ERR createModel"); return; }
  if (finger.storeModel(id) == FINGERPRINT_OK) {
    Serial.println("✅ Fingerprint enrolled as ID #" + String(id));
  } else {
    Serial.println("❌ Store failed");
  }
}
