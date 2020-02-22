#ifndef WATERCONTROL_INCLUDED
#define WATERCONTROL_INCLUDED

#include <Servo.h>

Servo verticalServo, horizontalServo;

const uint8_t usTrigPin = 38, usEchoPin = 36;

void waterSetup() {
  verticalServo.attach(9);
  verticalServo.write(0);
  horizontalServo.attach(10);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(usTrigPin, OUTPUT);
  pinMode(usEchoPin, INPUT);
  digitalWrite(usTrigPin, LOW);
}

int readWaterLevel() {
  digitalWrite(usTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(usTrigPin, LOW);
  return pulseIn(usEchoPin, HIGH) * 0.034 / 2;
}

bool spraying = false;

void sprayWater() {
  if (!spraying) {
    spraying = true;
    //..............
    spraying = false;
  }
}

#endif
