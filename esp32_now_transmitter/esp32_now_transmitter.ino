#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER ESP32 MAC ADDRESS
// Example: uint8_t broadcastAddress[] = {0xB4, 0xBF, 0xE9, 0x33, 0x9B, 0xF0};
uint8_t broadcastAddress[] = {0xb4, 0xbf, 0xe9, 0x33, 0x9b, 0xf0};

// Telemetry payload structure for 5-DOF Robotic Arm / Joint Controller
typedef struct struct_message {
    int baseYaw;
    int shoulder;
    int elbow;
    int wrist;
    int gripper;
} struct_message;

struct_message gestureData;
unsigned long startTime;
unsigned long latency;

// COMPLIANT WITH ESP32 ARDUINO CORE v3.0+:
// Signature expects (const wifi_tx_info_t *txInfo, esp_now_send_status_t status)
void OnDataSent(const wifi_tx_info_t *txInfo, esp_now_send_status_t status) {
  latency = millis() - startTime;
  Serial.print("Delivery Status: ");
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.print("Success! -> Latency: ");
    Serial.print(latency);
    Serial.println(" ms");
  } else {
    Serial.println("Fail (Packet Dropped)");
  }
}
 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register send callback for packet verification
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer receiver
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer receiver");
    return;
  }
  Serial.println("ESP-NOW Transmitter Initialized.");
}
 
void loop() {
  // Read sensor potentiometers, IMU gestures, or joysticks
  gestureData.baseYaw   = random(10, 170); 
  gestureData.shoulder  = random(0, 180);
  gestureData.elbow     = random(0, 180);
  gestureData.wrist     = random(0, 180);
  gestureData.gripper   = random(0, 180);

  startTime = millis(); 
  
  // Transmit telemetry payload via ESP-NOW 2.4GHz Wi-Fi frame
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &gestureData, sizeof(gestureData));
   
  delay(100); 
}
