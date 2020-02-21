#include "DistanceCheck.h"
#include "MotorControl.h"
#include "FireSiren.h"
#include "FlameDetecting.h"

void setup() {
  usSensorSetup();
  DCMotorSetup();
  sirenSetup();
  flameSensorSetup();
  Serial.begin(9600);
//  sirenSound();
}

int leftSensorValue, rightSensorValue;

void loop() {
    getSensorReadings(leftSensorValue, rightSensorValue);
    Serial.print(leftSensorValue); Serial.print(' '); Serial.println(rightSensorValue);
//  debug_printDistances();
//  motorBottLeft.run(FORWARD);
//  motorBottRight.run(FORWARD);
//  motorFrontRight.run(FORWARD);
//  motorFrontLeft.run(FORWARD);
}
