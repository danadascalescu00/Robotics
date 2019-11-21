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
const int pinX = A1;
const int pinY = A0;

int switchValue = 0;
int xValue = 0;
int yValue = 0;

const int segSize = 8;

const int noOfDisplays = 4;
const int noOfDigits = 10;

int dpState = LOW;
int swState = LOW;

bool joyMovedOx = false;
bool joyMovedOy = false;
int minTreshold = 400;
int maxTreshold = 600;

int currentDisplay = 0;

bool swPressed = false;
bool swPressed2 = true;
 
int currentNumber[noOfDisplays] = {
  0, 0, 0, 0
};
int currentDpState[noOfDisplays] = {
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

void showDigit(int num) {
  for (int i = 0; i < noOfDisplays; i++) {
    digitalWrite(digits[i], HIGH);
  }
  digitalWrite(digits[num], LOW);
}

void displayNumber(byte digit, int decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }

  analogWrite(segments[segSize - 1], decimalPoint);
}

void setup() {
  for (int i = 0; i < segSize - 1; i++){
    pinMode(segments[i], OUTPUT);  
  }
  for (int i = 0; i < noOfDisplays; i++){
    pinMode(digits[i], OUTPUT);  
  }
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  Serial.begin(9600);
}


void loop() {

  swState = digitalRead(pinSW);

  if(swState == LOW && swPressed == false && swPressed2 == true ){
    swPressed = true;
    swPressed2 = false;
  }
  else if(swState == LOW && swPressed == true && swPressed2 == true){
    swPressed = false;
    swPressed2 = false;
  }
  if(swState == HIGH){
    swPressed2 = true;
  }

  xValue = analogRead(pinX);

  if (xValue < minTreshold && joyMovedOx == false && swPressed == false){
    if(currentDisplay < noOfDisplays){
      currentDisplay++;
      currentDpState[currentDisplay] = 600;
      currentDpState[currentDisplay - 1] = 0;
    }
    else{
      currentDisplay = 0;
      currentDpState[currentDisplay] = 600;
      currentDpState[noOfDisplays - 1] = 0;
    }
    joyMovedOx = true;
      
  }
  
  if(xValue >= maxTreshold && joyMovedOx == false && swPressed == false){
    if(currentDisplay > 0){
      currentDisplay--;
      currentDpState[currentDisplay] = 600;
      currentDpState[currentDisplay + 1] = 0;
    }
    else{
      currentDisplay = noOfDisplays - 1;
      currentDpState[currentDisplay] = 600;
      currentDpState[0] = 0;
    }
    joyMovedOx = true;
  }
  
  if(xValue >= minTreshold && xValue <= maxTreshold){
    joyMovedOx = false;
  }

  yValue = analogRead(pinY);
        
   if (yValue < minTreshold && joyMovedOy == false && swPressed == true){
    if(currentNumber[currentDisplay] > 0){
      currentNumber[currentDisplay]--;
    }
    else{
      currentNumber[currentDisplay] = 9;
    }
    joyMovedOy = true;
   }
   
   if(yValue >= maxTreshold && joyMovedOy == false && swPressed == true){
     if(currentNumber[currentDisplay] < 9){
       currentNumber[currentDisplay]++;
     }
     else{
       currentNumber[currentDisplay] = 0;
     }
     joyMovedOy = true;
   }
   
   if(yValue >= minTreshold && yValue <= maxTreshold){
     joyMovedOy = false;
   }

  for(int i = 0; i < noOfDisplays; i++){
    showDigit(i);
    displayNumber(currentNumber[i], currentDpState[i]);
    delay(5);
  }

}
