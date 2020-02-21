#include <AFMotor.h>

AF_DCMotor motorBottLeft(1, MOTOR12_64KHZ); // create motor #1, 64KHz pwm
AF_DCMotor motorBottRight(2, MOTOR12_64KHZ); // create motor #2, 64KHz
AF_DCMotor motorFrontRight(3, MOTOR12_64KHZ); // create motor #3, 64KHz
AF_DCMotor motorFrontLeft(4, MOTOR12_64KHZ); // create motor #4, 64KHz

void DCMotorSetup() {
  motorBottLeft.setSpeed(200);
  motorBottRight.setSpeed(200);
  motorFrontRight.setSpeed(200);
  motorFrontLeft.setSpeed(200);
}

uint64_t fullturn = 10000;

void rotateBody(int angle) {
  int leftWay, rightWay;
  if (angle > 0) {
    leftWay = FORWARD;
    rightWay = BACKWARD;
    motorBottLeft.setSpeed(200);
    motorFrontLeft.setSpeed(200);
    motorBottRight.setSpeed(100);
    motorFrontRight.setSpeed(100);
  } else {
    leftWay = BACKWARD;
    rightWay = FORWARD;
    motorBottLeft.setSpeed(100);
    motorFrontLeft.setSpeed(100);
    motorBottRight.setSpeed(200);
    motorFrontRight.setSpeed(200);
  }
  uint64_t turn = abs(angle) / 360.0 * fullturn;
  uint64_t currentMillis = millis();
  motorBottLeft.run(leftWay);
  motorBottRight.run(rightWay);
  motorFrontRight.run(rightWay);
  motorFrontLeft.run(leftWay);
  delay(turn);
  motorBottLeft.run(RELEASE);
  motorBottRight.run(RELEASE);
  motorFrontRight.run(RELEASE);
  motorFrontLeft.run(RELEASE);
}
