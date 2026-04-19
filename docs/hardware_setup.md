# Hardware Setup Guide — DefenderID Smart Gate

## Component List

| # | Component          | Model/Spec         | Qty |
|---|-------------------|-------------------|-----|
| 1 | Microcontroller   | ESP32 Dev Board   | 1   |
| 2 | RFID Reader       | RC522             | 1   |
| 3 | Fingerprint Sensor| AS608 or R307     | 1   |
| 4 | Keypad            | 4x4 Matrix        | 1   |
| 5 | LCD Display       | 16x2 I2C (0x27)   | 1   |
| 6 | Servo Motor       | SG90              | 1   |
| 7 | Active Buzzer     | 3.3V or 5V        | 1   |
| 8 | LED               | Green + Red       | 2   |
| 9 | Resistors         | 220Ω              | 2   |
| 10| Jumper Wires      | Male-Male/Female  | ~30 |
| 11| Breadboard        | 830 Point         | 1   |
| 12| Power Supply      | 5V/2A             | 1   |

---

## Wiring Diagram

### RC522 RFID Reader → ESP32
```
RC522 Pin   →   ESP32 GPIO
─────────────────────────────
SDA (SS)    →   GPIO 5
SCK         →   GPIO 18
MOSI        →   GPIO 23
MISO        →   GPIO 19
IRQ         →   (not connected)
GND         →   GND
RST         →   GPIO 22
3.3V        →   3.3V  ⚠️ NOT 5V!
```

### AS608 / R307 Fingerprint Sensor → ESP32
```
Sensor Pin  →   ESP32
─────────────────────
VCC (Red)   →   3.3V (or 5V via regulator)
GND (Black) →   GND
TX (Yellow) →   GPIO 16
RX (Green)  →   GPIO 17
```
> ⚠️ If your sensor is 5V, use a voltage divider or logic level shifter on TX/RX.

### 4x4 Keypad → ESP32
```
Keypad Pin  →   ESP32 GPIO
───────────────────────────
Row 1       →   GPIO 32
Row 2       →   GPIO 33
Row 3       →   GPIO 25
Row 4       →   GPIO 26
Col 1       →   GPIO 27
Col 2       →   GPIO 14
Col 3       →   GPIO 12
Col 4       →   GPIO 13
```

### LCD 16x2 I2C → ESP32
```
LCD Pin     →   ESP32
───────────────────────
VCC         →   5V
GND         →   GND
SDA         →   GPIO 21
SCL         →   GPIO 22
```
> Default I2C address is 0x27. If LCD doesn't work, scan for I2C address using i2c_scanner.

### SG90 Servo Motor → ESP32
```
Servo Wire  →   ESP32
───────────────────────
Brown (GND) →   GND
Red   (VCC) →   5V (external supply recommended)
Orange(SIG) →   GPIO 13
```
> ⚠️ SG90 can draw 250-500mA. If the ESP32 resets when servo moves, use a separate 5V power supply for the servo — connect grounds together.

### LED Indicators → ESP32
```
Green LED → 220Ω resistor → GPIO 26
Red LED   → 220Ω resistor → GPIO 27
Both (-) → GND
```

### Buzzer → ESP32
```
Buzzer (+) → GPIO 12
Buzzer (-) → GND
```

---

## Fingerprint Enrollment

Before the gate can verify fingerprints, you must enroll your fingerprint into the sensor.

### Steps:
1. Open `fingerprint_module.ino`
2. Set `#define ENROLL_MODE true`
3. Set `#define ENROLL_ID 1` (or another slot 1-127)
4. Upload to ESP32
5. Open Serial Monitor at 115200 baud
6. Follow the prompts:
   - "Place finger on sensor..."
   - "Remove finger..."
   - "Place same finger again..."
   - "✅ Fingerprint enrolled as ID #1"
7. Set `ENROLL_MODE` back to `false`
8. Upload again

Multiple users can be enrolled in different IDs (1, 2, 3...).

---

## RFID Card UID

To find your physical RFID card's UID:
1. Flash the ESP32 with the code
2. Open Arduino Serial Monitor (115200 baud)
3. Tap the card on the RC522 reader
4. The UID will print, e.g. `A1B2C3D4`
5. Enter this UID on the Activation Page to link it to your digital card

---

## Troubleshooting

| Problem | Solution |
|---------|---------|
| RC522 "not found" | Check SPI wiring; ensure 3.3V (not 5V) |
| Fingerprint "not found" | Check UART wiring; verify baud rate 57600 |
| LCD blank | Check I2C address (run i2c_scanner); check contrast pot |
| Servo doesn't move | Power from external 5V supply |
| WiFi not connecting | Check SSID/password in wifi_config.h |
| Server not reachable | Verify SERVER_IP is your PC's actual LAN IP |
| OTP verify fails | Ensure ESP32 and PC are on same WiFi network |
