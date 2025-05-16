int in4Pin = 12;
int in3Pin = 14;
int enaPinB = 13;
int in1Pin = 26;
int in2Pin = 27;
int enaPinA = 25;

void setup() {
  Serial.begin(115200);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enaPinB, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enaPinA, OUTPUT);
}

void loop() {
  // Marche avant
  Serial.printf("Marche avant\n");
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
  analogWrite(enaPinB, 100);
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  analogWrite(enaPinA, 100);
  delay(2000);

  // Arrêt
  Serial.printf("Arrêt\n");
  digitalWrite(enaPinB, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
  digitalWrite(enaPinA, LOW);
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  
  delay(5000);

}