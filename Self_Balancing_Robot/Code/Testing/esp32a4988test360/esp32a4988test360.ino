// Control a NEMA17 stepper motor with an A4988 driver using ESP32
// Spins the motor 360 degrees at twice the previous speed
// H.M.Phan - Robonyx - [Today's Date]

#include <Arduino.h>

// Define pin connections for Motor 1
const int dirPin1 = 15;
const int stepPin1 = 2;

// Number of steps per revolution for NEMA17 motor (usually 200 steps)
// Adjust if your motor has a different step count
const int stepsPerRevolution = 200;

// Microstepping setting (1/16 microstepping)
const int microstepping = 16;

// Calculate total steps for one full revolution
const int totalSteps = stepsPerRevolution * microstepping;

// Original delay between steps in microseconds
// const int stepDelay = 500;  // Original delay

// Updated delay to make motor spin twice as fast
const int stepDelay = 250;  // Reduced delay by half

void setup() {
  // Initialize the output pins for Motor 1
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin1, OUTPUT);

  // Set initial direction
  digitalWrite(dirPin1, HIGH);  // Set direction (HIGH or LOW)

  // Optional: Initialize serial communication for debugging
  Serial.begin(115200);
  Serial.println("Starting motor rotation at double speed...");
}

void loop() {
  // Rotate the motor 360 degrees
  rotateMotor360();

  // Optional: Add a delay or change direction
  delay(1000);  // Wait for 1 second before next rotation
}

void rotateMotor360() {
  // Loop through total steps to make one full revolution
  for (int step = 0; step < totalSteps; step++) {
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(stepDelay);
  }

  // Optional: Print a message after one full rotation
  Serial.println("Completed one full rotation at double speed.");
}
