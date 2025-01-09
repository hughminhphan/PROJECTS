#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
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

// External declaration of the MD_MAX72XX object
extern MD_MAX72XX mx;

extern double output;
extern int face;


// Function prototypes
void displaySetup();
void displayLoop();
void displayBitmap(uint8_t device, const uint8_t *bitmap);
void displayFlicker();

#endif  // DISPLAY_H
