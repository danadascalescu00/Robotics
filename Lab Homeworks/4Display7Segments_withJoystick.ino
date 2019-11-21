const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = A2;
const int pinD1 = 7;
const int pinD2 = 9; 
const int pinD3 = 10;
const int pinD4 = 13;

const int pinSW = 4;
const int pinX = A0;
const int pinY = A1;

const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;

int switchValue;
int xValue;
int yValue;

int dpState = LOW;
int swState = LOW;

bool swPressedFirstTime = false;
bool swPressedSecondTime = true;
bool joyMovedOx = false;
bool joyMovedOy = false;

const int minThreashold = 400;
const int maxThreashold = 600;

int currentDisplay = 0; //current digit on which I display

int currentNumber[noOfDisplays] = {
  0, 0, 0, 0
};

int digits[noOfDisplays] = {
  pinD1, pinD2, pinD3, pinD4
};

int currentDpState[noOfDisplays] = {
  600, 0, 0, 0
};

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

const byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void showDigit(int number){
  for(int i = 0; i < noOfDisplays; i++){
    digitalWrite(digits[i], HIGH);
  }
  digitalWrite(digits[number], LOW);
}

void displayNumber(byte digit, int decimalPoint) {
  for(int i = 0; i < segSize - 1; i++){
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  analogWrite(segments[segSize - 1], decimalPoint);
}

void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {

  swState = digitalRead(pinSW);

  if(swState == LOW)
  {
    if(swPressedFirstTime == false && swPressedSecondTime == true){
      swPressedFirstTime = true;
      swPressedSecondTime = false;   
    }else if(swPressedFirstTime == true && swPressedSecondTime == true){
      swPressedFirstTime = false;
      swPressedSecondTime = false;    
    }
  }else{
    swPressedSecondTime = true;
  }

  xValue = analogRead(pinX);
  
  if(xValue <= minThreashold && joyMovedOx == false && swPressedFirstTime == false){
    if(currentDisplay > 0){
      currentDisplay--;
      //change state of DP point
      currentDpState[currentDisplay - 1] = 0;
      currentDpState[currentDisplay] = 600;
    }
    else{
      currentDpState[0] = 0;
      currentDisplay = noOfDisplays - 1;
      currentDpState[noOfDisplays - 1] = 600;
    }
    joyMovedOx = true;
  }

   if(xValue >= maxThreashold && joyMovedOx == false && swPressedFirstTime == false){
    if(currentDisplay < noOfDisplays){
      currentDisplay++;
      //change state of DP point
      currentDpState[currentDisplay - 1] = 0;
      currentDpState[currentDisplay] = 600;
    }
    else{
      currentDisplay = 0;
      currentDpState[currentDisplay] = 600;
      currentDpState[noOfDisplays - 1] = 0;
    }
    joyMovedOx = true;
  }

  if(xValue > minThreashold && xValue < maxThreashold){
    joyMovedOx = false;
  }

  yValue = analogRead(pinY);

  if(yValue <= minThreashold && joyMovedOy == false && swPressedFirstTime == true){
    if(currentNumber[currentDisplay] > 0){
      currentNumber[currentDisplay]--;
    }
    else{
      currentNumber[currentDisplay] = 9;
    }
    joyMovedOy = true;
  }

  if(yValue >= maxThreashold && joyMovedOy == false && swPressedFirstTime == true){
    if(currentNumber[currentDisplay] < 9){
      currentNumber[currentDisplay]++;
    }
    else{
      currentNumber[currentDisplay] = 0;
    }
    joyMovedOy = true;
   }

  if(yValue > minThreashold && yValue < maxThreashold){
    joyMovedOy = false;
  }

  for(int i = 0; i < noOfDisplays; i++){
    showDigit(i);
    displayNumber(currentNumber[i], currentDpState[i]);
    delay(25);
  }
}
