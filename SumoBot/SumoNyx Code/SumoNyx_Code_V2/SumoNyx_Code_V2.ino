#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
RF24 radio(7, 8);  // CE, CSN

const byte address[6] = "12345";
int values[8];
int motorpins[] = { 10, 6, 9, 5 };
int lastvalues[8];

void setup() {
  pinMode(motorpins[0], OUTPUT);
  pinMode(motorpins[1], OUTPUT);
  pinMode(motorpins[2], OUTPUT);
  pinMode(motorpins[3], OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  Serial.println("Serial Initiallised and listening");
}



void loop() {
  if (radio.available()) {
    radio.read(&values, sizeof(values));
    if (values[0] > 55) {
      analogWrite(motorpins[2], map(values[0], 50, 100, 255, 0));
      digitalWrite(motorpins[0], HIGH);
    } else if (values[0] < 45) {
      analogWrite(motorpins[0], map(values[0], 50, 0, 255, 0));
      digitalWrite(motorpins[2], HIGH);
    } else {
      digitalWrite(motorpins[0], HIGH);
      digitalWrite(motorpins[2], HIGH);
    }
    if (values[1] > 55) {
      analogWrite(motorpins[1], map(values[1], 50, 100, 255, 0));
      digitalWrite(motorpins[3], HIGH);
    } else if (values[1] < 45) {
      analogWrite(motorpins[3], map(values[1], 50, 0, 255, 0));
      digitalWrite(motorpins[1], HIGH);
    } else {
      digitalWrite(motorpins[1], HIGH);
      digitalWrite(motorpins[3], HIGH);
    }
  }
}
