#include <AFMotor.h>

AF_DCMotor motorBottLeft(1, MOTOR12_64KHZ), motorBottRight(2, MOTOR12_64KHZ), motorFrontRight(3, MOTOR12_64KHZ), motorFrontLeft(4, MOTOR12_64KHZ);

//variable used for ultrasonic
struct ultrasonicSensor {
  uint8_t trigPin, echoPin;
}sensor[3] = {{8, 9}, {0, 0}, {0, 0}};

int getDistance(int sensorNo) {
  digitalWrite(sensor[sensorNo].trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor[sensorNo].trigPin, LOW);
  return pulseIn(sensor[sensorNo].echoPin, HIGH) * 0.034 / 2;
}

// variable used for sound effect
const byte buzzerPin = 10;
const int soundDuration = 50, periodDuration = 40;
unsigned long lastPeriodStart;

// This function will be used only  one time in the setup, so the delay() won't interference with the functionality of the program  
void sirenSound() {
  for(int hz = 440; hz < 1000; hz++) {
    tone(buzzerPin, hz, soundDuration);
    delay(5);
  }
  for(int hz = 1000; hz> 440; hz--) {
    tone(buzzerPin, hz, soundDuration);
    delay(5);
  }
}

void effects() {
  for(int hz = 440; hz < 1000; hz++) {
    tone(buzzerPin, hz, soundDuration);
    lastPeriodStart = millis();
    while(millis() - lastPeriodStart > soundDuration and millis() - lastPeriodStart <= soundDuration + 5) {
      noTone(buzzerPin);
    }
  }
}

void setup() {
  for(int i = 0; i < 3; ++i) {
    //Serial.print(sensor[i].trigPin); Serial.print(' '); Serial.println(sensor[i].echoPin);
    pinMode(sensor[i].trigPin, OUTPUT);
    pinMode(sensor[i].echoPin, INPUT);
  }
  motorBottLeft.setSpeed(200);
  motorBottRight.setSpeed(200);
  motorFrontRight.setSpeed(200);
  motorFrontLeft.setSpeed(200);
  motorBottLeft.run(FORWARD);
  motorBottRight.run(FORWARD);
  motorFrontRight.run(FORWARD);
  motorFrontLeft.run(FORWARD);
}

void loop() {
  //Siren Sound and lights effecs:
  effects();
}
