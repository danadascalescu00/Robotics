#ifndef WATERCONTROL_INCLUDED
#define WATERCONTROL_INCLUDED

const uint8_t usTrigPin = 38, usEchoPin = 36;

void waterSetup() {
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

#endif
