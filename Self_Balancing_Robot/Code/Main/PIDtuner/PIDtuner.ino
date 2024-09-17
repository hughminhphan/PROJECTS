// Testing the motor movement with the ESP32 controlling a standard nema17 motor using the a4988 motor driver
// H.M.Phan - Robonyx - 15/9/24
// Modified for 1/16 microstepping

// Define pin connections for Motor 1
const int dirPin1 = 15;
const int stepPin1 = 2;

// Define pin connections for Motor 2
const int dirPin2 = 0;
const int stepPin2 = 4;

// Define ramping parameters
const int rampSteps = 4000;          // Increased number of steps to ramp up/down (4x for 1/16 microstepping)
const int maxSpeedDelay = 50;        // Reduced minimum delay for higher max speed (1/4 of original for 1/16 microstepping)
const int minSpeedDelay = 250;       // Reduced maximum delay for higher min speed (1/4 of original for 1/16 microstepping)

void setup() {
  // Initialize the output pins for Motor 1
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin1, OUTPUT);

  // Initialize the output pins for Motor 2
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin2, OUTPUT);

  // Set initial direction for Motor 1
  digitalWrite(dirPin1, HIGH);

  // Set initial direction for Motor 2 (reverse of Motor 1)
  digitalWrite(dirPin2, LOW);
}

void loop() {
  // Move motors: Motor 1 forward, Motor 2 backward
  moveMotors(HIGH, LOW);
  delay(500); // Pause before changing direction

  // Move motors: Motor 1 backward, Motor 2 forward
  moveMotors(LOW, HIGH);
  delay(500); // Pause before next cycle
}

void moveMotors(int dir1, int dir2) {
  // Set directions for both motors
  digitalWrite(dirPin1, dir1);
  digitalWrite(dirPin2, dir2);

  // Ramp up speed
  for (int i = 0; i < rampSteps; i++) {
    int currentDelay = map(i, 0, rampSteps, minSpeedDelay, maxSpeedDelay);
    stepMotors(currentDelay);
  }

  // Run at maximum speed
  for (int i = 0; i < 4000; i++) {  // Increased to 4000 steps for 1/16 microstepping
    stepMotors(maxSpeedDelay);
  }

  // Ramp down speed
  for (int i = rampSteps; i > 0; i--) {
    int currentDelay = map(i, 0, rampSteps, minSpeedDelay, maxSpeedDelay);
    stepMotors(currentDelay);
  }
}

void stepMotors(int delayUs) {
  // Generate step pulses for both motors
  digitalWrite(stepPin1, HIGH);
  digitalWrite(stepPin2, HIGH);
  delayMicroseconds(delayUs);
  digitalWrite(stepPin1, LOW);
  digitalWrite(stepPin2, LOW);
  delayMicroseconds(delayUs);
}