// Testing ESP32 with 2 x max7219 8x8 dot matrices they need to be daisy chained sharing CS and dout to DIN
// H.M.Phan - Robonyx - 16/9/24
// this is the part of the robot that gives it its soul looollll
#include <MD_MAX72xx.h>
#include <SPI.h>

// Define the number of devices (8x8 LED matrices) cascaded
#define MAX_DEVICES 2

// Define the type of hardware. Use FC16_HW for generic MAX7219 modules
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

// Define the ESP32 pins for SPI communication
#define DATA_PIN 23  // DIN pin connected to IO23
#define CS_PIN   17  // CS pins of both modules connected to IO17
#define CLK_PIN  18  // CLK pin connected to IO18
opiu
// Create a new instance of the MD_MAX72XX class with hardware SPI
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const uint8_t PROGMEM smiley[] = {
  B00111100,
  B01111110,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100
};

const uint8_t PROGMEM sad[] = {
  B00001100,
  B00011100,
  B00111000,
  B00110000,
  B00110000,
  B00111000,
  B00011100,
  B00001100
};

void setup() {
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 5); // Set brightness (0-15)
}

void loop() {
  // Display smiley face on Module 0
  displayBitmap(0, smiley);
  
  // Display sad face on Module 1
  displayBitmap(1, sad);
  
  delay(1000); // Display for 1 second

  // Swap images
  displayBitmap(0, sad);
  displayBitmap(1, smiley);
  
  delay(1000); // Display for 1 second

  // Loop repeats
}

// Function to display a bitmap on a specific module
void displayBitmap(uint8_t device, const uint8_t *bitmap) {
  for (uint8_t row = 0; row < 8; row++) {
    mx.setRow(device, row, pgm_read_byte(&bitmap[row]));
  }
}
