#include "display.h"

// Create a new instance of the MD_MAX72XX class with hardware SPI
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Bitmaps for smiley and sad faces
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

void displaySetup() {
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 2); // Set brightness (0-15)
  displayBitmap(0, sad);
  displayBitmap(1, smiley);
}

void displayLoop() {
  // Display smiley face on Module 0
  displayBitmap(0, smiley);

  // Display sad face on Module 1
  displayBitmap(1, sad);

  delay(1000); // Display for 1 second

  // Swap images
  displayBitmap(0, sad);
  displayBitmap(1, smiley);

  delay(1000); // Display for 1 second
}

// Function to display a bitmap on a specific module
void displayBitmap(uint8_t device, const uint8_t *bitmap) {
  for (uint8_t row = 0; row < 8; row++) {
    mx.setRow(device, row, pgm_read_byte(&bitmap[row]));
  }
}
