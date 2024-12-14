#include "espnow.h"

// External variables from the main file
extern double Kp;
extern double Ki;
extern double Kd;
extern double setpoint;
extern double turning;
extern int face;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info_t* esp_now_info, const uint8_t* incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  // Update PID parameters
  setpoint = myData.setpoint;
  setpoint = setpoint + (myData.tilt/100 * 5);
  turning = myData.turning;
  face = myData.face;
  // Print the received values
  // Serial.print("Received new PID values - Kp: ");
  // Serial.print(Kp);
  // Serial.print(" Ki: ");
  // Serial.print(Ki);
  // Serial.print(" Kd: ");
  Serial.println(face);
}

void espnowSetup() {
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register for receive callback to get packet info
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("ESP-NOW initialized and ready to receive PID values");
}
