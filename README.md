# 📦 ESP32 Smart Agriculture Sensor Suite

This project integrates **multiple sensors** with an **ESP32** board to monitor key environmental parameters in agriculture, including:

* 📊 **Soil pH**
* 🌡️ **Temperature (°C / °F)**
* 💧 **Humidity (%)**
* 🌱 **Soil Moisture (%)**
* 💡 **Light Intensity (%)**
* 🧪 **NPK (Nitrogen, Phosphorus, Potassium in mg/kg)** via RS485

<br>

## 🚀 Features

* Uses **Modbus RS485** protocol for NPK sensor communication
* Outputs real-time readings with **proper units** and **classification**
* Runs on **ESP32** with accurate 12-bit ADC readings
* Modular and well-commented code for easy expansion

---

## 🔧 Hardware Used

| Component                     | Quantity |
| ----------------------------- | -------- |
| ESP32 Dev Module              | 1        |
| PH4502C pH Sensor             | 1        |
| DHT22 Temperature & Humidity  | 1        |
| Analog Soil Moisture Sensor   | 1        |
| LDR (Light Sensor) + Resistor | 1        |
| RS485-to-TTL Module           | 1        |
| NPK Soil Sensor (RS485)       | 1        |
| Jumper Wires                  | —        |
| Breadboard                    | —        |

---

## 🔌 Pin Connections

| Sensor          | ESP32 GPIO | Notes                       |
| --------------- | ---------- | --------------------------- |
| **pH Sensor**   | 35         | Analog pin (Po output only) |
| **Soil Sensor** | 34         | Analog                      |
| **LDR**         | 32         | Voltage divider input       |
| **DHT22**       | 4          | Digital data pin            |
| **RS485**       | RXD2 = 16  | Connect to RO pin           |
|                 | TXD2 = 17  | Connect to DI pin           |
|                 | DE/RE = 5  | Connected together          |
|                 | VCC = 5V   | Use VIN or external power   |

> 💡 **Important:** Power the NPK sensor and RS485 module with **5V** (e.g., from ESP32 VIN pin or external source).

---

## 📥 Code Overview

All sensors are read every 10 seconds and values are printed to the Serial Monitor in this format:

```
🔬 pH: 6.78 (unitless) | Voltage: 2.45 V | Type: Slightly Acidic
🌡 Temperature: 28.50 °C / 83.30 °F
💧 Humidity: 60.00 % RH
🌱 Soil Moisture: 45 %
💡 Light Level: 58 % brightness
🔄 Reading NPK values...
🌾 Nitrogen (N): 123.45 mg/kg
🌾 Phosphorus (P): 56.78 mg/kg
🌾 Potassium (K): 91.00 mg/kg
```

---

## 🧪 Libraries Required

Install these libraries via the Arduino IDE Library Manager:

* `DHT sensor library by Adafruit`
* `ModbusMaster by Doc Walker`

---

## ▶️ Getting Started

1. Open the `.ino` file in Arduino IDE
2. Select **ESP32 Dev Module** in Tools > Board
3. Select the correct **COM port**
4. Click **Upload**
5. Open the Serial Monitor at **115200 baud**

---

## 📌 Future Ideas

* Add OLED or LCD display
* Send data to ThingSpeak / Blynk / Firebase
* Log readings to SD card
* Set alarms for extreme values

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).
