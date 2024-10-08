#include "espnow.h"

// External variables from the main file
extern double Kp;
extern double Ki;
extern double Kd;
extern double setpoint;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info_t* esp_now_info, const uint8_t* incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  // Update PID parameters
  Kp = myData.Kp;
  Ki = myData.Ki;
  Kd = myData.Kd;

  // Print the received values
  Serial.print("Received new PID values - Kp: ");
  Serial.print(Kp);
  Serial.print(" Ki: ");
  Serial.print(Ki);
  Serial.print(" Kd: ");
  Serial.println(Kd);

  setpoint = Kd;
  Kd = 0;
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
