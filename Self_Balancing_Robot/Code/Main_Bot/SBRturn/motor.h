#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <AccelStepper.h>

// Define pin connections for Motor 1
extern const int dirPin1;
extern const int stepPin1;

// Define pin connections for Motor 2
extern const int dirPin2;
extern const int stepPin2;

// Global instances of AccelStepper
extern AccelStepper stepper1;
extern AccelStepper stepper2;

// Motor control variables
extern unsigned long lastStepTime;
extern double stepDelayUs;
extern int motorDir;
extern double turning;

// Function prototypes
void motorSetup();
void updateMotorControl(double pidOutput);
double mapDouble(double x, double in_min, double in_max, double out_min, double out_max);

#endif  // MOTOR_H
