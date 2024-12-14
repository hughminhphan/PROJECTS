#include "motor.h"
#include <AccelStepper.h>

// Define pin connections for Motor 1
const int dirPin1 = 15;
const int stepPin1 = 2;

// Define pin connections for Motor 2
const int dirPin2 = 0;
const int stepPin2 = 4;

// Global instances of AccelStepper
AccelStepper stepper1(AccelStepper::DRIVER, stepPin1, dirPin1);
AccelStepper stepper2(AccelStepper::DRIVER, stepPin2, dirPin2);

// Motor control variables

void motorSetup() {
    stepper1.setMaxSpeed(6000);
    stepper2.setMaxSpeed(6000);
}

void updateMotorControl(double pidOutput) {
    // Map PID output to steps per second (motor speed)
    const double MaxPIDOutput = 255.0;         // Maximum PID output
    const double MaxStepsPerSecond = 5500.0;   // Maximum motor speed
    const double MinStepsPerSecond = -5500.0;  // Minimum motor speed

    // Constrain PID output
    if (pidOutput > MaxPIDOutput)
        pidOutput = MaxPIDOutput;

    // Map PID output to motor speed
    double stepsPerSecond = mapDouble(pidOutput, -255, 255, MinStepsPerSecond, MaxStepsPerSecond);
    stepper1.setSpeed(stepsPerSecond + (turning - 500));
    stepper2.setSpeed(-stepsPerSecond + (turning - 500));
    stepper1.runSpeed();
    stepper2.runSpeed();
}

double mapDouble(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
