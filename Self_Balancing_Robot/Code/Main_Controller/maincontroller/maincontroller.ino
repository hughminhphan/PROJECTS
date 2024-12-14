#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = { 0x10, 0x06, 0x1c, 0x97, 0x87, 0x54 };

// Define switch pin connections
const int kb1 = 23;
const int kb2 = 22;
const int kb3 = 21;
const int kb4 = 19;

const int p1h = 14;
const int p1v = 12;
const int p2h = 26;
const int p2v = 27;

// Structure to hold the data
typedef struct struct_message {
  bool kb1State;
  bool kb2State;
  bool kb3State;
  bool kb4State;
  int p1hValue;
  int p1vValue;
  int p2hValue;
  int p2vValue;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

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
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
      return;
    }

    pinMode(kb1, INPUT);
    pinMode(kb2, INPUT);
    pinMode(kb3, INPUT);
    pinMode(kb4, INPUT);
    pinMode(p1h, INPUT);
    pinMode(p1v, INPUT);
    pinMode(p2h, INPUT);
    pinMode(p2v, INPUT);
  }

  void loop() {
    // Update sensor states and joystick values
    myData.kb1State = digitalRead(kb1);
    myData.kb2State = digitalRead(kb2);
    myData.kb3State = digitalRead(kb3);
    myData.kb4State = digitalRead(kb4);
    myData.p1hValue = analogRead(p1h);
    myData.p1vValue = analogRead(p1v);
    myData.p2hValue = analogRead(p2h);
    myData.p2vValue = analogRead(p2v);

    // Send data via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    } else {
      Serial.println("Error sending the data");
    }

    delay(100);  // Adjust the delay as needed
  }

  void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Data Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
  }
