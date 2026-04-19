# 🛡️ DefenderID - Military Identity Security System

**DefenderID** is a high-security, local-first identity verification and gate access system designed for military and defense environments. It combines web-based personnel management with physical IoT-based gate control, focusing on biometric security.

---

## 🚀 Key Features

*   **🔒 Strict Access Control**: Restricted to specific authorized Gmail accounts and local admin/security roles.
*   **🖐 Biometric-First Flow**: Personnel must pass through a 4-step enrollment:
    1.  **Authenticated Login** (Gmail Whitelist)
    2.  **Military Identity Verification** (Photo & Credentials)
    3.  **Biometric Enrollment** (Fingerprint Scan)
    4.  **Identity Card Generation** (Downloadable PDF/PNG)
*   **🔍 Security Detail Lookup**: Integrated Admin tool to instantly verify personnel details by searching their Unique Card ID (e.g. `51F2D26`).
*   **🏢 Specialized Military Data**: Standardized branches (Defense Intelligence, Military Operations, etc.) and ranks (Captain, Major, General, etc.).
*   **🎮 IoT Gate Integration**: Real-time ESP32 gate control with fingerprint verification and remote administrative override.
*   **📜 Real-Time Access Logs**: Detailed audit trail of all entry/exit events with personnel identification.

---

## 🛠️ Technology Stack

*   **Frontend**: React.js (Vite), Tailwind CSS, Lucide Icons, Framer Motion.
*   **Backend**: Node.js, Express, MongoDB, JWT Security.
*   **IoT**: ESP32, AS608/R307 Fingerprint Sensor, MG996R Servo, I2C LCD.

---

## 👤 Authorized Personnel & Ranks

The system is currently configured for the **Defense Security Division**. Access is strictly limited to the following personnel:

| Identity | Gmail Address | Service Rank |
| :--- | :--- | :--- |
| **Tamanna Saini** | `tamannasaini860@gmail.com` | Soldier |
| **Sonakshi Dhiman** | `sonakshidhiman12@gmail.com` | Captain |
| **System Admin** | `admin@gate.local` | Commander |
| **Security Officer** | `security@gate.local` | Gate Control Officer |

### 🏢 Authorized Branches:
`Defense Security`, `Military Operations`, `Army Security Division`, `Access Control Unit`, `Defense Intelligence`, `Security Operations`.

---

## ⚙️ Installation & Setup

### 1. Prerequisites
*   **Node.js** (v18 or higher)
*   **MongoDB** (Local instance on port 27017)
*   **Arduino IDE** (For ESP32 firmware)

### 2. Environment Configuration
Ensure `backend/.env` is configured:
```env
PORT=5000
MONGO_URI=mongodb://localhost:27017/smartgate
JWT_SECRET=your_military_grade_secret
NODE_ENV=development
```

### 3. Quick Start
Run the following in separate terminals:
```bash
# Start Backend
cd backend
npm run dev

# Start Frontend
cd frontend
npm run dev
```

### 4. Database Seeding
To populate the system with the authorized personnel and initial logs:
```bash
cd backend
node seedData.js
```

---

## 🔌 Hardware Configuration (ESP32)

The system uses an **ESP32** for biometric gate triggering. 

1.  Open `iot-device/esp32/main.ino`.
2.  Update `iot-device/config/wifi_config.h` with your network details and Backend IP.
3.  The gate remains in **IDLE** state until a registered fingerprint is scanned.
4.  Once verified locally, the ESP32 notifies the Backend and triggers the Servo.

---

## 📂 Project Architecture

```text
├── backend/            # Express API, MongoDB Models, Controllers
├── frontend/           # React App (Modern UI + Framer Motion)
├── iot-device/         # ESP32 Firmware (Biometric-First States)
├── scripts/            # Automation & Maintenance scripts
└── docs/               # Technical Documentation
```

---

## ⚖️ License & usage
**Internal Use Only** - Prepared for Defense Strategic Operations.