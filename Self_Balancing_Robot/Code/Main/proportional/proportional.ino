// Self-balancing robot code using BNO055 with PI control and microstepping (factor of 4)
// H.M.Phan - Robonyx - 15/9/24
// Include necessary libraries
// Include necessary libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

// Define pin connections for Motor 1
const int dirPin1 = 15;
const int stepPin1 = 2;

// Define pin connections for Motor 2
const int dirPin2 = 0;
const int stepPin2 = 4;

// Create the BNO055 object using the default I2C address (0x28)
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

// PID control variables
double setpoint = 0; // Desired angle (upright position)
double input = 0.0;    // Current angle from IMU
double output = 0.0;   // PID controller output

// PID tuning parameters (to be adjusted experimentally)
double Kp = 15;
double Ki = 0;
double Kd = 0;

// PID calculation variables
double previousError = 0.0;
double integral = 0.0;
unsigned long lastComputeTime = 0;

// Motor control variables
unsigned long lastStepTime = 0;
double stepDelayUs = 1000; // Initial delay between steps in microseconds
int motorDir = HIGH;       // Initial motor direction

// Map function for double values
double mapDouble(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait for Serial to initialize
  }

  // Initialize the BNO055 sensor
  if (!bno.begin()) {
    Serial.println("Error initializing BNO055! Check your connections.");
    while (1);
  }

  delay(1000); // Allow sensor initialization

  // Use external crystal oscillator
  bno.setExtCrystalUse(true);

  Serial.println("BNO055 sensor initialized successfully!");

  // Initialize the output pins for Motor 1
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin1, OUTPUT);

  // Initialize the output pins for Motor 2
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin2, OUTPUT);

  // Initialize motor direction
  digitalWrite(dirPin1, motorDir);
  digitalWrite(dirPin2, !motorDir); // Inverse of motorDir

  // Initialize last compute time
  lastComputeTime = millis();
}

void loop() {
  unsigned long now = micros();

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

    // Update motor control based on PID output
    updateMotorControl(output);

    // Debugging output
    Serial.print("Pitch: ");
    Serial.print(input);
    Serial.print("°\tPID Output: ");
    Serial.println(output);
  }

  // Generate step pulses based on desired delay
  if (now - lastStepTime >= stepDelayUs) {
    lastStepTime = now;

    // Generate a step pulse
    digitalWrite(stepPin1, HIGH);
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(2); // Pulse width
    digitalWrite(stepPin1, LOW);
    digitalWrite(stepPin2, LOW);
  }
}

void computePID() {
  unsigned long currentTime = millis();
  double dt = (currentTime - lastComputeTime) / 1000.0; // Convert to seconds
  lastComputeTime = currentTime;

  // Avoid division by zero
  if (dt <= 0) dt = 0.001;

  // Calculate error
  double error = setpoint - input;

  // Calculate integral
  integral += error * dt;

  // Calculate derivative
  double derivative = (error - previousError) / dt;

  // Compute PID output
  output = Kp * error + Ki * integral + Kd * derivative;

  // Save error for next loop
  previousError = error;

  // Constrain output
  if (output > 255) output = 255;
  if (output < -255) output = -255;
}

void updateMotorControl(double pidOutput) {
  // Determine motor direction based on PID output
  if (pidOutput > 0) {
    motorDir = HIGH; // Motor 1 forward, Motor 2 backward
  } else {
    motorDir = LOW;  // Motor 1 backward, Motor 2 forward
    pidOutput = -pidOutput; // Use absolute value for speed
  }

  // Set motor directions (inverse of each other)
  digitalWrite(dirPin1, motorDir);
  digitalWrite(dirPin2, !motorDir); // Inverse direction

  // Map PID output to steps per second (motor speed)
  const double MaxPIDOutput = 255.0;        // Maximum PID output
  const double MaxStepsPerSecond = 2000.0;  // Maximum motor speed
  const double MinStepsPerSecond = 0.0;     // Minimum motor speed

  // Constrain PID output
  if (pidOutput > MaxPIDOutput) pidOutput = MaxPIDOutput;

  // Map PID output to motor speed
  double stepsPerSecond = mapDouble(pidOutput, 0.0, MaxPIDOutput, MinStepsPerSecond, MaxStepsPerSecond);

  // Prevent division by zero
  if (stepsPerSecond < 1) stepsPerSecond = 1;

  // Calculate delay between steps in microseconds
  stepDelayUs = 1e6 / stepsPerSecond;
}
