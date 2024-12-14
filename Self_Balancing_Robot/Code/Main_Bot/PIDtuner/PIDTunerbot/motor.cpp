#include "motor.h"

// Define pin connections for Motor 1
const int dirPin1 = 15;
const int stepPin1 = 2;

// Define pin connections for Motor 2
const int dirPin2 = 0;
const int stepPin2 = 4;

// Motor control variables
unsigned long lastStepTime = 0;
double stepDelayUs = 1000;  // Initial delay between steps in microseconds
int motorDir = HIGH;        // Initial motor direction

void motorSetup() {
  // Initialize the output pins for Motor 1
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin1, OUTPUT);

  // Initialize the output pins for Motor 2
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin2, OUTPUT);

  // Initialize motor direction
  digitalWrite(dirPin1, motorDir);
  digitalWrite(dirPin2, !motorDir);  // Inverse of motorDir
}

void generateStepPulses() {
  unsigned long now = micros();
  if (now - lastStepTime >= stepDelayUs) {
    lastStepTime = now;

    // Generate a step pulse
    digitalWrite(stepPin1, HIGH);
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(2);  // Pulse width
    digitalWrite(stepPin1, LOW);
    digitalWrite(stepPin2, LOW);
  }
}

void updateMotorControl(double pidOutput) {
  // Determine motor direction based on PID output
  if (pidOutput > 0) {
    motorDir = HIGH;  // Motor 1 forward, Motor 2 backward
  } else {
    motorDir = LOW;          // Motor 1 backward, Motor 2 forward
    pidOutput = -pidOutput;  // Use absolute value for speed
  }

  // Set motor directions (inverse of each other)
  digitalWrite(dirPin1, motorDir);
  digitalWrite(dirPin2, !motorDir);  // Inverse direction

  // Map PID output to steps per second (motor speed)
  const double MaxPIDOutput = 255.0;        // Maximum PID output
  const double MaxStepsPerSecond = 8000.0;  // Maximum motor speed
  const double MinStepsPerSecond = 0.0;     // Minimum motor speed

  // Constrain PID output
  if (pidOutput > MaxPIDOutput)
    pidOutput = MaxPIDOutput;

  // Map PID output to motor speed
  double stepsPerSecond = mapDouble(pidOutput, 0.0, MaxPIDOutput, MinStepsPerSecond, MaxStepsPerSecond);

  // Prevent division by zero
  if (stepsPerSecond < 1)
    stepsPerSecond = 1;

  // Calculate delay between steps in microseconds
  stepDelayUs = 1e6 / stepsPerSecond;
}

double mapDouble(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
