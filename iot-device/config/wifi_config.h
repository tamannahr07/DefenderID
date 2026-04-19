#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

// ── WiFi Settings ─────────────────────────────────────
// Replace these with your actual WiFi credentials
#define WIFI_SSID     "YourWiFiName"
#define WIFI_PASSWORD "YourWiFiPassword"

// ── Server Settings ───────────────────────────────────
// Replace SERVER_IP with your PC's local IP address.
// To find it: run  ipconfig  in Windows Command Prompt
// Look for "IPv4 Address" under your WiFi adapter, e.g. 192.168.1.100
#define SERVER_IP   "192.168.1.100"
#define SERVER_PORT 5000

// ── Derived server base URL ────────────────────────────
// E.g.  http://192.168.1.100:5000
#define SERVER_BASE  "http://" SERVER_IP ":" xstr(SERVER_PORT)
#define xstr(s) str(s)
#define str(s) #s

// ── Hardware Pin Assignments ──────────────────────────
// RFID RC522 (SPI)
#define RFID_SS_PIN   5    // SDA/SS   → ESP32 GPIO5
#define RFID_RST_PIN  22   // RST      → ESP32 GPIO22
// SPI CLK → GPIO18, MISO → GPIO19, MOSI → GPIO23

// Fingerprint Sensor (UART)
#define FP_RX_PIN     16   // Sensor TX → ESP32 GPIO16
#define FP_TX_PIN     17   // Sensor RX → ESP32 GPIO17

// Keypad
#define KEYPAD_ROWS   4
#define KEYPAD_COLS   4

// LCD I2C
#define LCD_ADDRESS   0x27
#define LCD_COLS      16
#define LCD_ROWS      2

// Servo
#define SERVO_PIN     13   // Signal → ESP32 GPIO13
#define SERVO_CLOSED  0    // Degrees
#define SERVO_OPEN    90   // Degrees

// Buzzer (optional)
#define BUZZER_PIN    12

// LED indicators (optional)
#define LED_GREEN     26
#define LED_RED       27

#endif // WIFI_CONFIG_H
