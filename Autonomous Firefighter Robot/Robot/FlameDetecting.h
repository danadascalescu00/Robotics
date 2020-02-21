const uint8_t flameSensorLeft = A9;
const uint8_t flameSensorRight = A8;
const int sensorMin = 0;
const int sensorMax = 1023;

void flameSensorSetup() {

}

void getSensorReadings(int &left, int &right) {
  int leftUnmapped = analogRead(flameSensorLeft);
  int rightUnmapped = analogRead(flameSensorRight);
  left = map(leftUnmapped, sensorMin, sensorMax, 0, 3);
  right = map(rightUnmapped, sensorMin, sensorMax, 0, 3);
}
