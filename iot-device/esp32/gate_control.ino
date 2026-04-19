/*
 * Gate Control Module — SG90 Servo Motor
 *
 * Wiring (Servo → ESP32):
 *   Brown  (GND)    → GND
 *   Red    (VCC)    → 5V (use external 5V supply for heavy servos!)
 *   Orange (Signal) → GPIO13 (SERVO_PIN)
 *
 * Note: SG90 draws up to 500mA. If gate is heavy or servo stalls,
 *       power from a separate 5V/2A supply and share GND with ESP32.
 *
 * Gate behavior:
 *   - Closed position: 0°  (or adjust SERVO_CLOSED)
 *   - Open position:   90° (or adjust SERVO_OPEN)
 */

#include <ESP32Servo.h>

Servo gateServo;

void gateSetup() {
  ESP32PWM::allocateTimer(0);
  gateServo.setPeriodHertz(50);           // Standard 50Hz servo
  gateServo.attach(SERVO_PIN, 500, 2400); // Min/max pulse width (µs)
  gateClose();                            // Ensure gate starts closed
  Serial.println("🔒 Gate servo initialized (closed)");
}

void gateOpen() {
  Serial.println("🔓 Opening gate...");
  // Sweep smoothly from closed to open
  int from = SERVO_CLOSED;
  int to   = SERVO_OPEN;
  for (int pos = from; pos <= to; pos += 2) {
    gateServo.write(pos);
    delay(15);
  }
  Serial.println("🔓 Gate OPEN");
}

void gateClose() {
  Serial.println("🔒 Closing gate...");
  // Sweep smoothly from open to closed
  int from = SERVO_OPEN;
  int to   = SERVO_CLOSED;
  for (int pos = from; pos >= to; pos -= 2) {
    gateServo.write(pos);
    delay(15);
  }
  Serial.println("🔒 Gate CLOSED");
}
