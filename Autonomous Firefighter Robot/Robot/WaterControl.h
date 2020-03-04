#ifndef WATERCONTROL_INCLUDED
#define WATERCONTROL_INCLUDED

#include<Servo.h>

Servo verticalServo, horizontalServo;

const uint8_t usTrigPin = 38, usEchoPin = 36, pumpPin = 34, relayPin = 40;

// declaring pins for the pump moving control mechanism
const uint8_t verticalServoPin = 44, horizontalServoPin = 46;
const uint16_t pumpMovingDuration = 1000;
const uint64_t  waterFlowDuration = 60000;
unsigned long lastWaterPumpStart, lastWaterPumpMove;


void waterSetup() {
  verticalServo.attach(verticalServoPin);
  horizontalServo.attach(horizontalServoPin);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(usTrigPin, OUTPUT);
  pinMode(usEchoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(usTrigPin, LOW);
  horizontalServo.write(90);
}

int readWaterLevel() {
  digitalWrite(usTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(usTrigPin, LOW);
  return pulseIn(usEchoPin, HIGH) * 0.034 / 2;
}

void moveWaterPump() {
  //left--right move of the water pump
  horizontalServo.write(0);
  horizontalServo.write(60);
  horizontalServo.write(120);
  //up--dowm move of the water pump
  verticalServo.write(0);
  lastWaterPumpMove = millis();
  if(millis() - lastWaterPumpStart >= pumpMovingDuration) {
    lastWaterPumpMove = millis();
    verticalServo.write(90);
  }
  verticalServo.write(45);
}

bool spraying = false;

void sprayWater() {
  if (spraying == false) {
    spraying = true;
    moveWaterPump();
    lastWaterPumpStart = millis();
    if(millis() - lastWaterPumpStart >= waterFlowDuration) {
      spraying = false;
      digitalWrite(relayPin, LOW);
    }
  }
}

#endif
