#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

// Create the BNO055 object using the default I2C address (0x28 or 0x29)
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void setup() {
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
}

void loop() {
  sensors_event_t orientationData;
  bno.getEvent(&orientationData);

  // Print Euler angles: Heading (X), Roll (Y), Pitch (Z)
  Serial.print("Heading: ");
  Serial.print(orientationData.orientation.x);
  Serial.print("°\tRoll: ");
  Serial.print(orientationData.orientation.y);
  Serial.print("°\tPitch: ");
  Serial.print(orientationData.orientation.z);
  Serial.println("°");

  delay(100); // Delay between readings
}
