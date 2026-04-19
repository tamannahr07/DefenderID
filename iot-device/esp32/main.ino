/*
 * ============================================================
 *   Smart Gate Security System — ESP32 Main Controller
 * ============================================================
 *
 *  Military Security Flow:
 *    Step 1: Fingerprint → Match locally stored template
 *    Step 2: Server Auth → Validate access for the identified personnel
 *
 *  Board: ESP32 Dev Module
 * ============================================================
 */

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "../config/wifi_config.h"

// Include sub-modules (RFID removed)
#include "fingerprint_module.ino"
#include "keypad_module.ino"
#include "gate_control.ino"

// ── LCD Setup ─────────────────────────────────────────
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// ── State machine ─────────────────────────────────────
enum GateState {
  IDLE,
  WAITING_FINGERPRINT,
  PROCESSING_ACCESS,
  ACCESS_GRANTED,
  ACCESS_DENIED
};

GateState gateState = IDLE;
unsigned long stateTimer = 0;
const unsigned long STATE_TIMEOUT = 20000; // 20 seconds for fingerprint match

// ── Forward declarations ───────────────────────────────
void notifyServerGateOpen(int fingerId);
void lcdMsg(const String& line1, const String& line2 = "");
void setLEDs(bool green, bool red);
void beep(int freq, int duration);

// ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial.println("\n🚀 DefenderID Military Gate Starting...");

  // GPIO setup
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED,   OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);
  setLEDs(false, false);

  // LCD init
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcdMsg("DefenderID Sec", "Initializing...");

  // WiFi connect
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi Connected! IP: " + WiFi.localIP().toString());
    lcdMsg("WiFi Online", "Gate Secured");
  } else {
    Serial.println("\n⚠️  WiFi Offline — Strict Security Enabled");
    lcdMsg("WiFi Offline!", "Check Network");
  }
  delay(2000);

  // Initialize modules
  fingerprintSetup();
  keypadSetup(); // Still used for manual override if needed
  gateSetup();

  lcdMsg("System Ready", "Scan Fingerprint");
  Serial.println("✅ All modules initialized. Waiting for Biometric...");
}

// ─────────────────────────────────────────────────────
void loop() {
  unsigned long now = millis();

  // Polling for server-side manual open (Admin Dashboard)
  static unsigned long lastPoll = 0;
  if (now - lastPoll > 2000) { // Poll every 2s
     lastPoll = now;
     checkRemoteOpen();
  }

  switch (gateState) {

    // ── STEP 1: Wait for Fingerprint ─────────────────
    case IDLE: {
      uint8_t fpResult = fingerprintVerify();
      if (fpResult == 0) { // 0 = MATCH SUCCESS
        Serial.println("✅ Biometric Identity Verified");
        lcdMsg("Verified!", "Accessing Cloud...");
        beep(1000, 200); delay(100); beep(1000, 200);
        
        // Pass finger ID to server for logging
        notifyServerGateOpen(getFingerID()); 
        
        gateState = ACCESS_GRANTED;
        stateTimer = now;
      } else if (fpResult == 1) { // 1 = NO MATCH
        Serial.println("❌ Biometric Mismatch");
        lcdMsg("Identity Fail", "Access Denied");
        beep(200, 1000);
        setLEDs(false, true);
        delay(3000);
        setLEDs(false, false);
        lcdMsg("System Ready", "Scan Fingerprint");
      }
      break;
    }

    case ACCESS_GRANTED: {
       lcdMsg("Access Granted!", "Gate Opening...");
       setLEDs(true, false);
       gateOpen();
       
       delay(10000); // Wait 10 seconds for vehicle/person clearance
       
       gateClose();
       setLEDs(false, false);
       lcdMsg("Gate Closed", "Secured.");
       delay(3000);
       gateState = IDLE;
       lcdMsg("System Ready", "Scan Fingerprint");
       break;
    }

    default:
      gateState = IDLE;
      break;
  }

  delay(100);
}

// ─────────────────────────────────────────────────────
// HTTP: Check if Admin sent a direct 'Open Gate' command
void checkRemoteOpen() {
  if (WiFi.status() != WL_CONNECTED) return;
  HTTPClient http;
  http.begin(String(SERVER_BASE) + "/api/gate/status");
  int code = http.GET();
  if (code == 200) {
    String payload = http.getString();
    DynamicJsonDocument doc(256);
    deserializeJson(doc, payload);
    if (doc["gate"]["isOpen"].as<bool>() && gateState == IDLE) {
       Serial.println("🔓 Remote Open Command Received");
       gateState = ACCESS_GRANTED;
    }
  }
  http.end();
}

// HTTP: Notify server that gate was opened by local biometric success
void notifyServerGateOpen(int fingerId) {
  if (WiFi.status() != WL_CONNECTED) return;
  HTTPClient http;
  http.begin(String(SERVER_BASE) + "/api/gate/open");
  http.addHeader("Content-Type", "application/json");
  
  DynamicJsonDocument doc(128);
  doc["fingerprintId"] = fingerId;
  doc["message"] = "Biometric Access Granted (ESP32)";
  
  String body;
  serializeJson(doc, body);
  http.POST(body);
  http.end();
}

// ── Helpers ───────────────────────────────────────────
void lcdMsg(const String& line1, const String& line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1.substring(0, 16));
  if (line2.length() > 0) {
    lcd.setCursor(0, 1);
    lcd.print(line2.substring(0, 16));
  }
}

void setLEDs(bool green, bool red) {
  digitalWrite(LED_GREEN, green ? HIGH : LOW);
  digitalWrite(LED_RED,   red   ? HIGH : LOW);
}

void beep(int freq, int duration) {
  tone(BUZZER_PIN, freq, duration);
}
