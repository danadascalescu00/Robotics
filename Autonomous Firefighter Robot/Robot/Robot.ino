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
}

int leftSensorValue, rightSensorValue;

void loop() {
  if (getDistance(FRONT_US) < frontMinimum) {
    if (getDistance(LEFT_US) < leftMinimum) {
      if (getDistance(RIGHT_US) < rightMinimum) {
        goBackwards();
      } else {
        rotateVehicle(turnAngle);
        goForward();
      }
    } else {
      rotateVehicle(-turnAngle);
      goForward();
    }
  } else {
    goForward();
  }
}
