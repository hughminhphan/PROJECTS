// Testing the motor movement with the ESP32 controlling a standard nema17 motor using the a4988 motor driver
// H.M.Phan - Robonyx - 15/9/24
// High freqyency buzzing, rickity movement using the boot pin is kind of sus

#include <AccelStepper.h>

// Define motor control pins
// #define DIR_PIN  0   // Direction pin connected to IO0
// #define STEP_PIN 4   // Step pin connected to IO4
#define DIR_PIN  15   // Direction pin connected to IO0
#define STEP_PIN 2   // Step pin connected to IO4
// Create an instance of AccelStepper in driver mode
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  // Set the maximum speed and acceleration for the stepper motor
  stepper.setMaxSpeed(1000);      // Maximum speed in steps per second
  stepper.setAcceleration(500);   // Acceleration in steps per second squared
}

void loop() {
  // Move the motor forward 200 steps
  stepper.move(200);
  stepper.runToPosition();  // Blocks until the motion is complete
  delay(1000);              // Wait for 1 second

  // Move the motor backward 200 steps
  stepper.move(-200);
  stepper.runToPosition();  // Blocks until the motion is complete
  delay(1000);              // Wait for 1 second
}
