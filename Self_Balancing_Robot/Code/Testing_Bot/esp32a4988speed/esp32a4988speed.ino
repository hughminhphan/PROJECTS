// Modified code to drive a stepper motor in one direction constantly at a quick pace
// H.M.Phan - Robonyx - Modified

// Define pin connections for Motor 1
const int dirPin1 = 15;
const int stepPin1 = 2;

void setup() {
  // Initialize the output pins for Motor 1
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin1, OUTPUT);

  // Set direction for Motor 1
  digitalWrite(dirPin1, HIGH); // Change to LOW to reverse direction
}

void loop() {
  // Generate continuous steps at a quick pace
  digitalWrite(stepPin1, HIGH);
  delayMicroseconds(200); // Adjust this value to control speed (smaller = faster)
  digitalWrite(stepPin1, LOW);
  delayMicroseconds(200);
}
