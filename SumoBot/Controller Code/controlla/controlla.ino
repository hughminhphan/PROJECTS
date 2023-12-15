#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);  // CE, CSN

const byte address[6] = "12345";
int potentiometerPins[] = { A0, A2, A1, A3 };
int buttonPins[] = { 4, 6, 3, 5, 9, 10 };
int values[8];
int lastvalues[8];
int speed[2];
int leftpotmin = 0;
int leftpotmax = 1023;
int leftpotmid = 400;
int rightpotmin = 0;
int rightpotmax = 1023;
int rightpotmid = 400;

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  Serial.begin(9600);
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
}

void loop() {
  values[0] = analogRead(potentiometerPins[0]);
  values[1] = analogRead(potentiometerPins[1]);
  values[2] = digitalRead(buttonPins[0]);
  values[3] = digitalRead(buttonPins[1]);
  values[4] = digitalRead(buttonPins[2]);
  values[5] = digitalRead(buttonPins[3]);
  values[6] = digitalRead(buttonPins[4]);
  values[7] = digitalRead(buttonPins[5]);
  if (values[0] >= leftpotmid) {
    values[0] = map(values[0], leftpotmid, leftpotmax, 50, 100);
  } else if (values[0] < leftpotmid) {
    values[0] = map(values[0], leftpotmin, leftpotmid, 0, 50);
  }
  if (values[1] >= rightpotmid) {
    values[1] = map(values[1], rightpotmid, rightpotmax, 50, 100);
  } else if (values[0] < rightpotmid) {
    values[1] = map(values[1], rightpotmin, rightpotmid, 0, 50);
  }
  if (values[0] > 90) {
    values[0] = 100;
  } else if (values[0] < 10) {
    values[0] = 0;
  } else if (values[0] < 60 && values[0] > 40) {
    values[0] = 50;
  }
  if (values[1] > 90) {
    values[1] = 100;
  } else if (values[1] < 10) {
    values[1] = 0;
  } else if (values[1] < 60 && values[1] > 40) {
    values[1] = 50;
  }

  if (values[2] == HIGH && values[5] == HIGH) {
    calibrateJoysticks();
  }
  speed[0] = values[0];
  speed[1] = values[1];

  radio.write(&speed, sizeof(speed));
  for (int i = 0; i < 8; i++) {
    lastvalues[i] = values[i];
  }
}
void calibrateJoysticks() {
  leftpotmid = analogRead(potentiometerPins[0]);
  rightpotmid = analogRead(potentiometerPins[1]);

  while (digitalRead(buttonPins[4]) == LOW) {
    if (digitalRead(buttonPins[3]) == HIGH) {
      leftpotmin = analogRead(potentiometerPins[0]);
    }
    if (digitalRead(buttonPins[2]) == HIGH) {
      leftpotmax = analogRead(potentiometerPins[0]);
    }
    if (digitalRead(buttonPins[1]) == HIGH) {
      rightpotmin = analogRead(potentiometerPins[1]);
    }
    if (digitalRead(buttonPins[0]) == HIGH) {
      rightpotmax = analogRead(potentiometerPins[1]);
    }
  }
}
