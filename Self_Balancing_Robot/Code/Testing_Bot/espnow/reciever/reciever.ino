#include <esp_now.h>
#include <WiFi.h>

// Structure to receive data
typedef struct struct_message {
  float Ki;
  float Kp;
  float Kd;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info_t *esp_now_info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Print the received values
  Serial.print("Ki: ");
  Serial.print(myData.Ki);
  Serial.print(" Kp: ");
  Serial.print(myData.Kp);
  Serial.print(" Kd: ");
  Serial.println(myData.Kd);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register for recv CB to get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  // Nothing to do here
    // Print the received values
  Serial.print("Ki: ");
  Serial.print(myData.Ki);
  Serial.print(" Kp: ");
  Serial.print(myData.Kp);
  Serial.print(" Kd: ");
  Serial.println(myData.Kd);
  delay(100);
}