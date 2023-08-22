#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "12345";
int potentiometerPins[] = {A4, A6};
int buttonPins[] = {A3,A2,A1,A0,2,3};
int Values[8];

void setup() {
  pinMode(A3, INPUT);
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(A0,INPUT);
  pinMode(A4, INPUT);
  pinMode(A6, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  Serial.begin(9600);
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
}

void loop() {
  Values[0]=analogRead(potentiometerPins[0]);
  Values[1]=analogRead(potentiometerPins[1]);
  Values[2]=digitalRead(buttonPins[0]);
  Values[3]=digitalRead(buttonPins[1]);
  Values[4]=digitalRead(buttonPins[2]);
  Values[5]=digitalRead(buttonPins[3]);
  Values[6]=digitalRead(buttonPins[4]);
  Values[7]=digitalRead(buttonPins[5]);

  radio.write(&Values, sizeof(Values));
}