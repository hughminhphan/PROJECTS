#include <Arduino.h>
#include <AccelStepper.h>
#include "motor.h"
#include "espnow.h"
#include "display.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

// Create the BNO055 object using the default I2C address (0x28)
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

// PID control variables
double turning = 0;
double setpoint = 3;  // Desired angle (upright position)
double input = 0.0;   // Current angle from IMU
double output = 0.0;  // PID controller output

// PID tuning parameters (to be adjusted via ESP-NOW)
double Kp = 20.0;
double Ki = 200.0;
double Kd = 0.0;

// PID calculation variables
double previousError = 0.0;
double integral = 0.0;
unsigned long lastComputeTime = 0;

// Global display varibles
int face = 0;  //face signal from the transmitter

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    delay(10);  // Wait for Serial to initialize
  }

  // Initialize the BNO055 sensor
  if (!bno.begin()) {
    Serial.println("Error initializing BNO055! Check your connections.");
    while (1)
      ;
  }

  delay(2000);  // Allow sensor initialization

  // Use external crystal oscillator
  // bno.setExtCrystalUse(true);

  Serial.println("BNO055 sensor initialized successfully!");

  // Initialize motor
  motorSetup();

  // Initialize last compute time
  lastComputeTime = millis();

  // Initialize ESP-NOW
  espnowSetup();

  // Initialize Display
  displaySetup();
}

void loop() {
  // Read sensor data every 10ms
  static unsigned long lastSensorReadTime = 0;
  if (millis() - lastSensorReadTime >= 10) {
    lastSensorReadTime = millis();

    // Get orientation data from IMU
    sensors_event_t orientationData;
    bno.getEvent(&orientationData);

    // Extract the pitch angle (assuming orientation.z is pitch)
    input = orientationData.orientation.z;

    // Compute PID output
    computePID();



    // Debugging output
    // Serial.print("Setpoint:");
    // Serial.print(setpoint);
    // Serial.print(",");
    // Serial.print("turning:");
    // Serial.print(turning);
    // Serial.print(",");
    // Serial.print("PID Output:");
    // Serial.println(output);
    displayLoop();
  }
    // Update motor control based on PID output
    updateMotorControl(output);
}

void computePID() {
  unsigned long currentTime = millis();
  double dt = (currentTime - lastComputeTime) / 1000.0;  // Convert to seconds
  lastComputeTime = currentTime;

  // Avoid division by zero
  if (dt <= 0)
    dt = 0.001;

  // Calculate error
  double error = setpoint - input;

  // Calculate integral
  //Apply integral windup protection
  integral += error * dt;
  if (integral > 3) {
    integral = 3;
  }
  if (integral < -3) {
    integral = -3;
  }

  // Calculate derivative
  double derivative = (error - previousError) / dt;

  // Compute PID output
  output = Kp * error + Ki * integral + Kd * derivative;

  // Save error for next loop
  previousError = error;

  // Constrain output
  if (output > 255) {
    output = 255;
  }
  if (output < -255) {
    output = -255;
  }
  // Serial.print("P:");
  // Serial.print(Kp * error);
  // Serial.print(",");
  // Serial.print("I:");
  // Serial.print(Ki * integral);
  // Serial.print(",");
  // Serial.print("D:");
  // Serial.print(Kd * derivative);
  // Serial.print(",");
}
