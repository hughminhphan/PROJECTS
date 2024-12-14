#include "display.h"

// Create a new instance of the MD_MAX72XX class with hardware SPI
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Bitmaps for neutral and wink faces
const uint8_t PROGMEM neutral[] = {
  B00111100,
  B01111110,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100
};

const uint8_t PROGMEM wink[] = {
  B00001100,
  B00011100,
  B00111000,
  B00110000,
  B00110000,
  B00111000,
  B00011100,
  B00001100
};

const uint8_t PROGMEM struggleleft[] = {
  B00011000,
  B00011000,
  B00111100,
  B00111100,
  B01100110,
  B01100110,
  B11000011,
  B11000011
};
const uint8_t PROGMEM struggleright[] = {
  B11000011,
  B11000011,
  B01100110,
  B01100110,
  B00111100,
  B00111100,
  B00011000,
  B00011000
};
const uint8_t PROGMEM heart[] = {
  B01110000,
  B11111000,
  B11111100,
  B01111110,
  B01111110,
  B11111100,
  B11111000,
  B01110000
};

void displaySetup() {
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 2);  // Set brightness (0-15)
  displayBitmap(0, wink);
  displayBitmap(1, neutral);
}

unsigned long delayStartTime = 0;
bool delayActive = false;

void displayLoop() {
  unsigned long currentTime = millis();  // Get the current time in milliseconds

  if (!delayActive) {
    // If delay is not active, start it
    delayStartTime = currentTime;
    delayActive = true;
    return;  // Exit the function to make the delay non-blocking
  }

  // Check if 1 second has passed since the delay started
  if (currentTime - delayStartTime >= 1000) {
    // Delay has finished, execute the display code
    if (output > 70 || output < -70) {
      displayBitmap(0, struggleleft);
      displayBitmap(1, struggleright);
    } else if (face == 1) {
      displayBitmap(0, heart);
      displayBitmap(1, heart);
    } else {
      displayBitmap(0, neutral);
      displayBitmap(1, wink);
    }
    delayActive = false;  // Reset the delay state
  }
  // If the delay is still active and 1 second hasn't passed, do nothing
}



// Function to display a bitmap on a specific module
void displayBitmap(uint8_t device, const uint8_t *bitmap) {
  for (uint8_t row = 0; row < 8; row++) {
    mx.setRow(device, row, pgm_read_byte(&bitmap[row]));
  }
}
