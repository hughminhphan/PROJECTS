// transmittercontroller.ino
#include <esp_now.h>
#include <WiFi.h>

// Structure to receive PID data
typedef struct pid_data_t {
  float p;
  float i;
  float d;
  float setpoint;
  float input;
  float output;
} pid_data_t;

// Create a pidData variable
pid_data_t pidData;

// Callback when data is received
void OnDataRecv(const esp_now_recv_info_t* esp_now_info, const uint8_t* incomingData, int len) {
  memcpy(&pidData, incomingData, sizeof(pidData));

  // Print the received data
  Serial.print("P: ");
  Serial.print(pidData.p);
  Serial.print(", I: ");
  Serial.print(pidData.i);
  Serial.print(", D: ");
  Serial.print(pidData.d);
  Serial.print(", Setpoint: ");
  Serial.print(pidData.setpoint);
  Serial.print(", Input: ");
  Serial.print(pidData.input);
  Serial.print(", Output: ");
  Serial.println(pidData.output);
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Print MAC address (optional, for debugging)
  Serial.print("Transmitter MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Transmitter Controller is ready to receive data");
}

void loop() {
  // Nothing to do here; data is received via the callback
}