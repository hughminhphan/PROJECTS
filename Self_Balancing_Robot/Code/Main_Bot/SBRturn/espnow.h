#ifndef ESPNOW_H
#define ESPNOW_H

#include <esp_now.h>
#include <WiFi.h>

// Structure to receive data
typedef struct struct_message {
  float setpoint;
  float turning;
  float tilt;
  int face;

} struct_message;

// External variables
extern struct_message myData;

// Function prototypes
void espnowSetup();
void OnDataRecv(const esp_now_recv_info_t *esp_now_info, const uint8_t *incomingData, int len);

#endif  // ESPNOW_H
