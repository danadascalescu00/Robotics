//declare the pins
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
const int pinOx = A0;
const int pinOy = A1;

int switchValue;
int xValue;
int yValue;

const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;

int dpState = LOW;
int swState = LOW;

int currentDisplay = 0;
int minThreashold = 400;
int maxThreashold = 600;

bool joyMovedOx = false;
bool joyMovedOy = false;

//by default the current state of the display is locked
bool switchPressedI = false;
bool switchPressedII = true;

int currentNumber[noOfDisplays] = {
  0, 0, 0, 0
};

int currentDPState[noOfDisplays] = {
  600, 0, 0, 0
};

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int digits[noOfDisplays] = {
  pinD1, pinD2, pinD3, pinD4
 };   
 
byte digitMatrix[noOfDigits][segSize - 1] = {
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

void setDigit(int digit) {
  for(int i = 0; i < noOfDigits; i++) {
    digitalWrite(digits[i], HIGH);
  }
  digitalWrite(digits[digit], LOW);    
}

void displayNumber(byte digit, int decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  analogWrite(segments[segSize - 1], decimalPoint);
}


void setup() {
  for(int i = 0; i < segSize - 1; i++) {
    pinMode(segments[i], OUTPUT);  
  }

  for(int i = 0; i < noOfDisplays; i++) {
    pinMode(digits[i], OUTPUT);
  }

  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinOx, INPUT);
  pinMode(pinOy, INPUT);
  Serial.begin(9600); 
}

void loop() {
  swState = digitalRead(pinSW);

  if(swState == LOW)
  {
    if(switchPressedI == false && switchPressedII == true) {
      switchPressedI = true;
      switchPressedII = false;
    }else if(switchPressedI == true && switchPressedII == true) {
      switchPressedI = false;
      switchPressedII = false;
    }
  }else{
    switchPressedII = true;
  }

  xValue = analogRead(pinOx);

  if(xValue <= minThreashold && joyMovedOx == false && switchPressedI == false){
    if(currentDisplay > 0) {
      currentDPState[currentDisplay] = 0;
      currentDisplay--;
      currentDPState[currentDisplay] = 600;
    }
    else{
      currentDisplay = noOfDisplays - 1;
      currentDPState[0] = 0;
      currentDPState[currentDisplay] = 600;
    }
    joyMovedOx =true; 
  }

  if(xValue >= maxThreashold && joyMovedOx == false && switchPressedI == false){
    if(currentDisplay < noOfDisplays) {
      currentDPState[currentDisplay] = 0;
      currentDisplay++;
      currentDPState[currentDisplay] = 600;
    }
    else{
      currentDisplay = 0;
      currentDPState[noOfDisplays - 1] = 0;
      currentDPState[currentDisplay] = 600;
    }
    joyMovedOx = true;
  }

  if(xValue > minThreashold && xValue < maxThreashold){
    joyMovedOx = false;
  }

  yValue = analogRead(pinOy);

  if (yValue <= minThreashold && joyMovedOy == false && switchPressedI == true) {
    if(currentNumber[currentDisplay] > 0) {
      currentNumber[currentDisplay]--;
    }else {
      currentNumber[currentDisplay] = 9;
    }
    joyMovedOy = true;
  }

  if (yValue >= maxThreashold && joyMovedOy == false && switchPressedI == true) {
    if(currentNumber[currentDisplay] < 9) {
      currentNumber[currentDisplay]++;
    }else {
      currentNumber[currentDisplay] = 0;
    }
    joyMovedOy = true;
  }

  if(yValue >= minThreashold && yValue <= maxThreashold){
     joyMovedOy = false;
  }

  for(int i = 0; i < noOfDisplays; i++){
    setDigit(i);
    displayNumber(currentNumber[i], currentDPState[i]);
    delay(5);
  }  
  delay(5);
}
