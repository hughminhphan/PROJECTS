// H.M.Phan - Robonyx - 17/9/24
// Using the ESPNOW protocol
// Transmits three values (Ki, Kp, Kd) to the first ESP32
// Kd is assigned from the analog read of pin 33 (ADC1)
// Ki and Kp are received as serial input from the serial monitor.
// Transmits the latest Ki, Kp, Kd values every 0.1 seconds

#include <esp_now.h>
#include <WiFi.h>

// Replace with the MAC Address of your receiver
uint8_t broadcastAddress[] = { 0x10, 0x06, 0x1C, 0x97, 0x87, 0x54 };

// Structure to send data
typedef struct struct_message {
  float Ki = 100.0;
  float Kp = 25.0;
  float Kd = 0.01;
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
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set pin 33 as input (ADC1)
  pinMode(33, INPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register for Send Callback
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Read analog value from pin 33 and assign to Kd
  int analogValue = analogRead(33);
  myData.Kd = analogValue * (100.0 / 4095.0);  // Scale to 0-100 range

  // Check for serial input to update Ki and Kp
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    sscanf(input.c_str(), "%f %f", &myData.Ki, &myData.Kp);

    Serial.print("Received values from Serial: Ki=");
    Serial.print(myData.Ki);
    Serial.print(" Kp=");
    Serial.println(myData.Kp);
  }

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }

  Serial.print("Current values: Ki=");
  Serial.print(myData.Ki);
  Serial.print(" Kp=");
  Serial.print(myData.Kp);
  Serial.print(" Kd=");
  Serial.println(myData.Kd);

  delay(100);  // Wait for 0.1 seconds before sending again
}
