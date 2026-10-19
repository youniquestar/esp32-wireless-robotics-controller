# 📡 Ultra-Low-Latency ESP-NOW Wireless Telemetry & Robotic Arm Controller

[![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-E65100.svg?logo=espressif&logoColor=white)](https://www.espressif.com/en/products/socs/esp32)
[![Protocol: ESP--NOW](https://img.shields.io/badge/Protocol-ESP--NOW%20(2.4GHz)-blue.svg)]()
[![Core Compliance: v3.0+](https://img.shields.io/badge/ESP32%20Core-v3.0%2B%20Compliant-brightgreen.svg)]()
[![Latency: Sub--5ms](https://img.shields.io/badge/Latency-%3C5ms%20Real--Time-green.svg)]()
[![Language: C++](https://img.shields.io/badge/Language-C%2B%2B-00599C.svg?logo=c%2B%2B&logoColor=white)](https://en.wikipedia.org/wiki/C%2B%2B)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Status: Tested](https://img.shields.io/badge/Hardware%20Status-Tested%20%26%20Verified-brightgreen.svg)]()

A peer-to-peer, ultra-low-latency wireless telemetry and motion controller designed for multi-degree-of-freedom (5-DOF) robotic arms, manipulators, and teleoperation rigs. Uses Espressif's proprietary **ESP-NOW** connectionless protocol operating directly on raw 2.4GHz IEEE 802.11 action frames, bypassing the TCP/IP stack to achieve sub-5 millisecond roundtrip delivery latency.

Fully upgraded and compliant with the breaking API changes introduced in **ESP32 Arduino Core v3.0+** (`const wifi_tx_info_t *txInfo` and `const esp_now_recv_info *info`).

---

## 📋 Included Nodes

| Directory | Role | Description |
| :--- | :--- | :--- |
| [`esp32_now_transmitter/`](esp32_now_transmitter/) | Controller Node | Packages 5-DOF joint angles into packed struct, measures transmission latency, and prints delivery acknowledgments |
| [`esp32_now_receiver/`](esp32_now_receiver/) | Robotic Arm Actuator Node | Asynchronously catches packets via Wi-Fi MAC layer interrupts, unpacks coordinates, and drives servos |

---

## 🛠️ Hardware Requirements & Bill of Materials (BOM)

| Component | Specification | Quantity |
| :--- | :--- | :--- |
| **Transmitter Board** | ESP32-WROOM-32, ESP32-S3, or ESP32-C3 DevKit | 1 |
| **Receiver Board** | ESP32-WROOM-32, ESP32-S3, or ESP32-C3 DevKit | 1 |
| **Robotic Arm / Servos** | 5x MG996R or SG90 Servos (Base Yaw, Shoulder, Elbow, Wrist, Gripper) | 1 set |
| **Power Supply** | 5V 3A External Power Supply (for high-torque servos) | 1 |

---

## 📦 Data Payload Structure

Both transmitter and receiver share the binary packet struct:

```cpp
typedef struct struct_message {
    int baseYaw;   // Joint 1: 0° - 180°
    int shoulder;  // Joint 2: 0° - 180°
    int elbow;     // Joint 3: 0° - 180°
    int wrist;     // Joint 4: 0° - 180°
    int gripper;   // End-Effector: 0° (Open) - 180° (Closed)
} struct_message;
```

---

## ⚡ ESP32 Core v3.0+ Callback Compliance

Previous versions of ESP-NOW examples break on newer ESP32 core versions. This implementation implements the modern ESP-IDF / Arduino Core v3.0+ signatures:

```cpp
// Transmitter Callback Signature
void OnDataSent(const wifi_tx_info_t *txInfo, esp_now_send_status_t status);

// Receiver Callback Signature
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len);
```

---

## 🚀 Setup & Pairing Guide

### Step 1: Find Receiver MAC Address
On the receiver ESP32, upload a sketch containing:
```cpp
#include <WiFi.h>
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
}
void loop() {}
```
Note the 6-byte hexadecimal address (e.g., `B4:BF:E9:33:9B:F0`).

### Step 2: Configure Transmitter
In [`esp32_now_transmitter/esp32_now_transmitter.ino`](esp32_now_transmitter/esp32_now_transmitter.ino), set:
```cpp
uint8_t broadcastAddress[] = {0xB4, 0xBF, 0xE9, 0x33, 0x9B, 0xF0};
```

### Step 3: Flash and Run
Upload the transmitter and receiver sketches. Launch Serial Monitor on both boards at **115200 baud** to observe real-time telemetry streaming and roundtrip latency (< 5ms).

---

## 📜 License

Distributed under the [MIT License](LICENSE).
