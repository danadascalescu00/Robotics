#include "DistanceCheck.h"
#include "MotorControl.h"
#include "FireSiren.h"
#include "FlameDetecting.h"
#include "WaterControl.h"

void setup() {
  usSensorSetup();
  DCMotorSetup();
  sirenSetup();
  flameSensorSetup();
  waterSetup();
  Serial.begin(9600);
}

void loop() {
//  Serial.println(readWaterLevel());
//  debug_printDistances();
//  if (checkForFlame()) {
//    playSiren();
//    emergencyMovement();
//  }
//  else {
//    resetSiren();
//    normalMovement();
//  }
  verticalServo.write(134);
}
