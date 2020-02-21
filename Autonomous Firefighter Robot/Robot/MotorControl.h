#include <AFMotor.h>

AF_DCMotor motorBottLeft(1, MOTOR12_2KHZ); // create motor #1, 64KHz pwm
AF_DCMotor motorBottRight(2, MOTOR12_2KHZ); // create motor #2, 64KHz
AF_DCMotor motorFrontRight(3, MOTOR12_2KHZ); // create motor #3, 64KHz
AF_DCMotor motorFrontLeft(4, MOTOR12_2KHZ); // create motor #4, 64KHz

const uint8_t runningSpeed = 200;

void DCMotorSetup() {
  motorBottLeft.setSpeed(runningSpeed);
  motorBottRight.setSpeed(runningSpeed);
  motorFrontRight.setSpeed(runningSpeed);
  motorFrontLeft.setSpeed(runningSpeed);
}

bool vehicleMoving = false;

void goForward() {
  if (!vehicleMoving) {
    vehicleMoving = true;
    motorBottLeft.run(FORWARD);
    motorBottRight.run(FORWARD);
    motorFrontRight.run(FORWARD);
    motorFrontLeft.run(FORWARD);
  }
}

void goBackwards() {
  if (!vehicleMoving) {
    vehicleMoving = true;
    motorBottLeft.run(BACKWARD);
    motorBottRight.run(BACKWARD);
    motorFrontRight.run(BACKWARD);
    motorFrontLeft.run(BACKWARD);
  }
}

void fullStop() {
  if (vehicleMoving) {
    vehicleMoving = false;
    motorBottLeft.run(RELEASE);
    motorBottRight.run(RELEASE);
    motorFrontRight.run(RELEASE);
    motorFrontLeft.run(RELEASE);
  }
}

const uint64_t fullturn = 12000;
const uint8_t turnAngle = 25;
bool isRotating = false;

void rotateVehicle(int angle) {
  if (!isRotating) {
    isRotating = true;
    fullStop();
    uint64_t turn = abs(angle) / 360.0 * fullturn;
    if (angle > 0) {
      motorBottRight.setSpeed(runningSpeed / 2);
      motorFrontRight.setSpeed(runningSpeed / 2);
      motorBottLeft.run(FORWARD);
      motorBottRight.run(BACKWARD);
      motorFrontRight.run(BACKWARD);
      motorFrontLeft.run(FORWARD);
      delay(turn);
      motorBottRight.setSpeed(runningSpeed);
      motorFrontRight.setSpeed(runningSpeed);
    } else {
      motorBottLeft.setSpeed(runningSpeed / 2);
      motorFrontLeft.setSpeed(runningSpeed / 2);
      motorBottLeft.run(BACKWARD);
      motorBottRight.run(FORWARD);
      motorFrontRight.run(FORWARD);
      motorFrontLeft.run(BACKWARD);
      delay(turn);
      motorBottLeft.setSpeed(runningSpeed);
      motorFrontLeft.setSpeed(runningSpeed);
    }
    fullStop();
    isRotating = false;
  }
}
