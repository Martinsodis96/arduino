const int motion = 8;
const int summer = 6;

void setup() {
  Serial.begin(9600);
  pinMode(motion, INPUT);
  pinMode(summer, OUTPUT);

}

void loop() {
  int motionValue = digitalRead(motion);
  makeSummerSound(motionValue);
  delay(200);
}


void makeSummerSound(int motionValue) {
  if (motionValue == HIGH) {
    tone(summer, 100);
  }
  else {
    noTone(summer);
  }
}
