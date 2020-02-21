#include "DistanceCheck.h"
#include "MotorControl.h"
#include "FireSiren.h"

void setup() {
  usSensorSetup();
  DCMotorSetup();
  Serial.begin(9600);
  sirenSound();
}

void loop() {

//  debug_printDistances();
//  motorBottLeft.run(FORWARD);
//  motorBottRight.run(FORWARD);
//  motorFrontRight.run(FORWARD);
//  motorFrontLeft.run(FORWARD);
}
