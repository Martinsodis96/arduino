const int potentioMeterPin = A0;
const int d2ButtonPin = 2;
const int d3ButtonPin = 3;
const int redLedPin = 12;
const int blueLedPin = 13;
const int buzzerPin = 5;

const int usTrigPin = 7;
const int usEchoPin = 8;

void setup() {
  pinMode(usTrigPin, OUTPUT);
  pinMode(usEchoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int potentiometerValue = analogRead(potentioMeterPin);
  int ulraSensorValue = getValueFromUltraSensor();
  Serial.println(ulraSensorValue);
  toggleBlueLedButton();
  makeBuzzerSound(ulraSensorValue);

  delay(10);
}

void toggleBlueLedButton(){
  int d2ButtonValue = digitalRead(d2ButtonPin);
  digitalWrite(blueLedPin, d2ButtonValue == 0 ? 1 : 0);
}

void makeBuzzerSound(int frequency){
  tone(buzzerPin, frequency, 1);
}

int getValueFromUltraSensor(){
  digitalWrite(usTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(usTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(usTrigPin, LOW);
  
  return 7000 - pulseIn(usEchoPin, HIGH);
}
