#include <esp_now.h>
#include <WiFi.h>

// Telemetry payload structure matching transmitter
typedef struct struct_message {
    int baseYaw;
    int shoulder;
    int elbow;
    int wrist;
    int gripper;
} struct_message;

struct_message incomingReadings;

// COMPLIANT WITH ESP32 ARDUINO CORE v3.0+:
// Signature expects (const esp_now_recv_info *info, const uint8_t *incomingData, int len)
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  
  Serial.print("Data Received -> ");
  Serial.print("Yaw: "); Serial.print(incomingReadings.baseYaw);
  Serial.print(" | Shoulder: "); Serial.print(incomingReadings.shoulder);
  Serial.print(" | Elbow: "); Serial.print(incomingReadings.elbow);
  Serial.print(" | Wrist: "); Serial.print(incomingReadings.wrist);
  Serial.print(" | Gripper: "); Serial.println(incomingReadings.gripper);

  // Example: Direct actuation of servo objects
  // servoYaw.write(incomingReadings.baseYaw);
  // servoShoulder.write(incomingReadings.shoulder);
}
 
void setup() {
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register reception interrupt callback
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("ESP-NOW Receiver Initialized. Waiting for data...");
}
 
void loop() {
  // Free for background kinematics or safety watchdogs;
  // Packet arrival handled asynchronously by Wi-Fi MAC layer interrupts!
  delay(100);
}
