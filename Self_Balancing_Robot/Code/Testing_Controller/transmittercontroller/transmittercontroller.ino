// H.M.Phan - Robonyx - 17/9/24
// Using the ESPNOW protocol
// Transmits three values (Ki, turn, tilt) to the first ESP32
// tilt is assigned from the analog read of pin 33 (ADC1)
// Ki and turn are received as serial input from the serial monitor.
// Transmits the latest Ki, turn, tilt values every 0.1 seconds

#include <esp_now.h>
#include <WiFi.h>

// Replace with the MAC Address of your receiver
uint8_t broadcastAddress[] = { 0x10, 0x06, 0x1C, 0x97, 0x87, 0x54 };

// Structure to send data
typedef struct struct_message {
  float setpoint = 0.5;
  float turning = 10.0;
  float tilt = 0.00;
  int face = 0;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Peer info
esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

unsigned long lastActiveTime = 0;  // Store the last time the button was pressed

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set pin 33 and 32 as input (ADC1)
  pinMode(33, INPUT);
  pinMode(32, INPUT);

  // Set pins 23, 22, 21, 19 as INPUT (KB1, KB2, KB3, KB4 respectively)
  pinMode(23, INPUT);
  pinMode(22, INPUT);
  pinMode(21, INPUT);
  pinMode(19, INPUT);

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
  // Read analog value from pin 33 and assign to tilt
  int analogValue1 = analogRead(33);
  int analogValue2 = analogRead(32);

  // Adjust setpoint based on digital inputs
  myData.setpoint += 0.1 * digitalRead(22);
  myData.setpoint -= 0.1 * digitalRead(19);

  myData.tilt = analogValue1 * (100.0 / 4095.0);  // Scale to 0-100 range

  // Face control logic
  if (digitalRead(23) == HIGH && digitalRead(21) == LOW) {
    myData.face = 1;
    lastActiveTime = millis();  // Update last active time
  }

  // Check if 1 second has passed since the last button press
  if (myData.face == 1 && (millis() - lastActiveTime >= 1000)) {
    myData.face = 0;
  }

  // Deadzone for analogValue2
  if (analogValue2 < 2500 && analogValue2 > 1500) {
    analogValue2 = 2048;
  }

  myData.turning = analogValue2 * (100.0 / 409.5);

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  // Debugging output
  Serial.print("Current values: Ki=");
  Serial.print(myData.setpoint);
  Serial.print(" turn=");
  Serial.print(myData.turning);
  Serial.print(" tilt=");
  Serial.print(myData.tilt);
  Serial.print(" face=");
  Serial.println(myData.face);

  delay(100);  // Wait for 0.1 seconds before sending again
}
