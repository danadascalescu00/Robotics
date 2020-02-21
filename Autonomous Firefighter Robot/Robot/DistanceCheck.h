// defines pins numbers
struct ultrasonic {
  uint8_t trigPin, echoPin;
} usSensor[3] = {{23, 22}, {39, 37}, {52, 53}};

#define LEFT_US 0
#define FRONT_US 1
#define RIGHT_US 2

int getDistance(int us) {
  digitalWrite(usSensor[us].trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(usSensor[us].trigPin, LOW);
  return pulseIn(usSensor[us].echoPin, HIGH) * 0.034 / 2;
}

void debug_printDistances() {
  int leftDistance = getDistance(LEFT_US);
  int frontDistance = getDistance(FRONT_US);
  int rightDistance = getDistance(RIGHT_US);
  Serial.print("Left distance: "); Serial.print(leftDistance); Serial.print("; ");
  Serial.print("Front distance: "); Serial.print(frontDistance); Serial.print("; ");
  Serial.print("Right distance: "); Serial.println(rightDistance);
}

void usSensorSetup() {
  for(int i = 0; i < 3; ++i) {
      pinMode(usSensor[i].trigPin, OUTPUT); // Sets the trigPin as an Output
      pinMode(usSensor[i].echoPin, INPUT); // Sets the echoPin as an Input
      digitalWrite(usSensor[i].trigPin, LOW);
  }
}
