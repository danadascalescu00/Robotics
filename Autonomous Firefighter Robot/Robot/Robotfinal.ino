#include "DistanceCheck.h"
#include "MotorControl.h"
#include "FireSiren.h"
#include "FlameDetecting.h"
#include "WaterControl.h"

boolean once = false;

void setup() {
  usSensorSetup();
  DCMotorSetup();
  sirenSetup();
  flameSensorSetup();
  waterSetup();  
}

void loop() {
  if (checkForFlame()) {
    playSiren();
    emergencyMovement();
  }
  else {
    resetSiren();
    normalMovement();
  }
}
