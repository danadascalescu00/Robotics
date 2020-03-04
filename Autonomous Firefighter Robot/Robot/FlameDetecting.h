#ifndef FLAMEDETECTING_INCLUDED
#define FLAMEDETECTING_INCLUDED

const uint8_t flameSensorLeft = A9;
const uint8_t flameSensorRight = A8;
const uint16_t sensorMin = 0;
const uint16_t sensorMax = 1023;

void getSensorReadings(int &left, int &right) {
  int leftUnmapped = analogRead(flameSensorLeft);
  int rightUnmapped = analogRead(flameSensorRight);
  left = map(leftUnmapped, sensorMin, sensorMax, 0, 3);
  right = map(rightUnmapped, sensorMin, sensorMax, 0, 3);
}

bool checkForFlame() {
  int leftReading, rightReading;
  getSensorReadings(leftReading, rightReading);
  if (leftReading < 2 || rightReading < 2) {
    return true;
  }
  return false;
}

#endif
