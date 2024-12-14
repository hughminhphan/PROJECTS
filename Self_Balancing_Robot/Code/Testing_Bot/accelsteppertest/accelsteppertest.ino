#include <AccelStepper.h>

// Define pin connections for Motor 1
const int dirPin1 = 15;
const int stepPin1 = 2;

// Define pin connections for Motor 2
const int dirPin2 = 15;
const int stepPin2 = 2;

// Create an instance of AccelStepper
AccelStepper stepper(AccelStepper::DRIVER, stepPin1, dirPin1);

void setup() {
  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(7000);    // Maximum steps per second
  // Acceleration is not used with runSpeed(), but can be set if needed
}

void loop() {
  // Set the speed for continuous rotation
  stepper.setSpeed(-640);  // Steps per second (use negative value for reverse direction)
  stepper.runSpeed();      // Continues moving at the set speed
}
