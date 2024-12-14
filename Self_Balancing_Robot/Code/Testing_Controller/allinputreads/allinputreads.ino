// Just testing basic button reads + the joysticks
// H.M.Phan - Robonyx - 2/11/24

// Define switch pin connections
const int kb1 = 23;
const int kb2 = 22;
const int kb3 = 21;
const int kb4 = 19;

const int p1h = 14;
const int p1v = 12;
const int p2h = 26;
const int p2v = 27;


void setup() {
  pinMode(kb1, INPUT);
  pinMode(kb2, INPUT);
  pinMode(kb3, INPUT);
  pinMode(kb4, INPUT);
  pinMode(p1h, INPUT);
  pinMode(p1v, INPUT);
  pinMode(p2h, INPUT);
  pinMode(p2v, INPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(kb1) == HIGH) {
    Serial.print("KB1: HIGH, ");
  } else {
    Serial.print("KB1: LOW, ");
  }

  if (digitalRead(kb2) == HIGH) {
    Serial.print("KB2: HIGH, ");
  } else {
    Serial.print("KB2: LOW, ");
  }

  if (digitalRead(kb3) == HIGH) {
    Serial.print("KB3: HIGH, ");
  } else {
    Serial.print("KB3: LOW, ");
  }

  if (digitalRead(kb4) == HIGH) {
    Serial.print("KB4: HIGH, ");
  } else {
    Serial.print("KB4: LOW, ");
  }
  delay(10);
  Serial.print(analogRead(p1h));
  Serial.print(", ");
  Serial.print(analogRead(p1v));
  Serial.print(", ");
  Serial.print(analogRead(p2h));
  Serial.print(", ");
  Serial.println(analogRead(p2v));
}
