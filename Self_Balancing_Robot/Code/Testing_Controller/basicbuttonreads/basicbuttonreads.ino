// Just testing basic button reads
// H.M.Phan - Robonyx - 26/10/24

// Define switch pin connectionsd
const int kb1 = 23;
const int kb2 = 22;
const int kb3 = 21;
const int kb4 = 19;

void setup() {
  pinMode(kb1, INPUT);
  pinMode(kb2, INPUT);
  pinMode(kb3, INPUT);
  pinMode(kb4, INPUT);
  Serial.begin(9600);
}

void loop() {
  if(digitalRead(kb1)==HIGH){
    Serial.print("KB1: HIGH, ");
  }
  else{
    Serial.print("KB1: LOW, ");
  }

  if(digitalRead(kb2)==HIGH){
    Serial.print("KB2: HIGH, ");
  }
  else{
    Serial.print("KB2: LOW, ");
  }

  if(digitalRead(kb3)==HIGH){
    Serial.print("KB3: HIGH, ");
  }
  else{
    Serial.print("KB3: LOW; ");
  }

  if(digitalRead(kb4)==HIGH){
    Serial.println("KB4: HIGH");
  }
  else{
    Serial.println("KB4: LOW");
  }
  delay(10);
}
