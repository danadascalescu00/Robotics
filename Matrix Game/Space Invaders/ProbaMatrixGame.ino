#include "pitches.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>

const int redPin = A2;
const int greenPin = A3;
const int bluePin = A5;

const int pinSW = 9; // digtal pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connceted to Y output

const int buzzerPin = 6;

const int pinPushButton = 8;

//declare all the LCD pins
const int RS = 13;
const int enable = 7;
const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;

LiquidCrystal lcd(RS, enable, D4, D5, D6, D7);

unsigned int redValue = 0;
unsigned int blueValue = 0;
unsigned int greenValue = 0;

unsigned long previousMillisBlinking = 0;
unsigned long lastLevelMillis = 0;

const long interval = 250;
const long passingLevelInterval = 2500;

int switchValue, xValue, yValue;
int switchState = LOW;

boolean joyMovedOx = false;
boolean joyMovedOy = false;
boolean firstTime = true;

int minThreshold = 400;
int maxThreshold = 600;

int reading;
int buttonState = HIGH;

// "We wish you a Merry Christmas"
int wish_melody[] = {
  NOTE_B3, 
  NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,
  NOTE_D4, NOTE_G4, NOTE_E4,
  NOTE_F4
};

int wish_tempo[] = {
  4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 8, 8,
  4, 4, 4,
  2
};

void levelPassed() {
  unsigned long currentMillisBlinking = millis();
  redValue = blueValue = 0;
  
  if(currentMillisBlinking - previousMillisBlinking >= interval) {
    previousMillisBlinking = currentMillisBlinking;
    if(greenValue == 255) {
      greenValue = 0;
    }else {
      greenValue = 255;
    }
    SetColors(redValue, greenValue, blueValue);
  }
}

void newLevel() {
  levelPassed();
  unsigned long newLevelMillis = millis();
  if(newLevelMillis - lastLevelMillis >= passingLevelInterval) {
    if(firstTime == true) {
      lastLevelMillis = newLevelMillis;
      firstTime = false;
    }else {
      switchState = LOW;
      lastLevelMillis = 0;
      firstTime = true;
      SetColors(0,0,0);
    }
  }
}

void SetColors(unsigned int Red, unsigned int Green, unsigned int Blue) {
  analogWrite(redPin, Red);
  analogWrite(greenPin, Green);
  analogWrite(bluePin, Blue);  
}

void sing() {
  int size = sizeof(wish_melody) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {
    int noteDuration = 1000 / wish_tempo[thisNote];
     buzz(buzzerPin, wish_melody[thisNote], noteDuration);
     int pauseBetweenNotes = noteDuration * 1.30;
     delay(pauseBetweenNotes);
     buzz(buzzerPin, 0, noteDuration);
  }
}

void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000 / frequency / 2; 
  long numCycles = frequency * length / 1000; 
  for (long i = 0; i < numCycles; i++) {
    digitalWrite(targetPin, HIGH); 
    delayMicroseconds(delayValue); 
    digitalWrite(targetPin, LOW); 
    delayMicroseconds(delayValue);
  }
}

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(pinSW, INPUT_PULLUP); //activate pull-up resistor on the pushbutton pin of joystick  
  pinMode(buzzerPin, OUTPUT);
  pinMode(pinPushButton, INPUT_PULLUP);
  lcd.begin(16, 2);
  lcd.print("Welcome!");
}

void loop() {
  
}
