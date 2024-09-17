// H.M.Phan - Robonyx - 17/9/24
// Using the ESPNOW protocol
// transmits three values (Ki, Kp, Kd) to the first esp32
// recieves these three values as a serial input from typing itno the serial monitor.
// transmits the latest kp ki kd values every 0.1 seconds

#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0x10, 0x06, 0x1c, 0x97, 0x90, 0x5c};

// Structure to send data
typedef struct struct_message {
  float Ki = 10.0;
  float Kp = 100;
  float Kd = 0.05;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Peer info
esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    sscanf(input.c_str(), "%f %f %f", &myData.Ki, &myData.Kp, &myData.Kd);
    
    Serial.print("Received values: Ki=");
    Serial.print(myData.Ki);
    Serial.print(" Kp=");
    Serial.print(myData.Kp);
    Serial.print(" Kd=");
    Serial.println(myData.Kd);
  }
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  
  delay(100); // Wait for 0.1 seconds before sending again
}