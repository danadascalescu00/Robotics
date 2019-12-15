#include "pitches.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>

uint8_t ch = 0;
uint16_t currMsgBit = 0;

const char startMsg[] = "Press the red button to get started";

const int redPin   = A2;
const int greenPin = A3;
const int bluePin  = A5;

const int pinSW = 9; // digtal pin connected to switch output
const int pinX  = A0; // A0 - analog pin connected to X output
const int pinY  = A1; // A1 - analog pin connceted to Y output

const int buzzerPin = 6;

const int pushButton = 8;

//declare all the LCD pins
const int RS     = 12;
const int enable = 11;
const int D4     = 5;
const int D5     = 4; 
const int D6     = 3;
const int D7     = 2;

LiquidCrystal lcd(RS, enable, D4, D5, D6, D7);

unsigned int redValue   = 0;
unsigned int blueValue  = 0;
unsigned int greenValue = 0;

unsigned long previousMillisBlinking = 0;
unsigned long lastLevelMillis = 0;
unsigned long lastPhaseDuration = 0;
unsigned long lastShown = 0;
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50; // the debounce time; increase if the output flickers

const long interval = 250;
const long passingLevelInterval = 2500;
const long phaseInterval = 2000;

int switchValue, xValue, yValue;
int switchState = LOW;

boolean joyMovedOx = false;
boolean joyMovedOy = false;
boolean firstTime = false;
boolean phaseI = false;
boolean introductionDisplayed = false;
boolean cleared = false;
boolean clearedOnce = false;
boolean mainMenu = false;

int minThreshold = 400;
int maxThreshold = 600;

int buttonState = HIGH;  // the current reading from the input pin
int lastButtonState = HIGH; // the previous reading from the input pin

unsigned int startingLevel = 1;

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
    if(firstTime == false) {
      lastLevelMillis = newLevelMillis;
      firstTime = true;
    }else {
      switchState = HIGH;
      lastLevelMillis = 0;
      firstTime = false;
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

void display_introduction() {
  unsigned long phaseDuration = millis();
  if(phaseDuration - lastPhaseDuration >= phaseInterval) {
    if(firstTime == false) {
      firstTime = true;
      lastPhaseDuration = phaseDuration;
      lcd.setCursor(1,0);
      lcd.print("Space Invaders");
      lcd.setCursor(1,1);
      lcd.print("The Retro Game");
    }else {
      if(phaseI == false) {
        phaseI = true;
        lastPhaseDuration = phaseDuration;
      }else {
        lcd.clear();
        lcd.setCursor(4,0);
        lcd.print("Christmas ");
        lcd.setCursor(5,1);
        lcd.print("Edition");
        introductionDisplayed = true;
        sing();
        firstTime = false;
      }
    }
  }
}

void lcd_printMsg(char *str) {
  uint8_t ch = 0;
  while(ch < 16 && str[ch] != '\0') {
    lcd.print((char)str[ch++]);
  }
  while(ch++ < 16) {
    lcd.print(" ");
  }
}

void display_settings() {
  lcd.setCursor(0,0);
  lcd.print("Level: ");
  lcd.setCursor(7,0);
  lcd.print(startingLevel);
  lcd.setCursor(0,1);
  lcd.print("Player: ");
  lcd.setCursor(7,1);
}

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(pinSW, INPUT_PULLUP); //activate pull-up resistor on the pushbutton pin of joystick  
  pinMode(buzzerPin, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
  
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() {
  if(introductionDisplayed == false) {
    display_introduction();
  }
  else {
    int reading = digitalRead(pushButton);
    // check to see if the button was pressed, and we have waited long enough since
    // the last press to ignore any noise:
    
    if(reading != lastButtonState) {
      // reset the debouncing timer
      lastDebounceTime = millis();
    }

    if((millis() - lastDebounceTime) > debounceDelay) {
      if(reading != buttonState) {
        buttonState = reading;
      }
    }
    
    // save the reading:
    lastButtonState = reading;

    
    if(mainMenu == false) {
      if(cleared == false){
        lcd.clear();
        cleared = true;
      }
      lcd.setCursor(1,0);
      lcd.print("Space Invaders");
      lcd.setCursor(0,1);
      
      if(millis() > lastShown + 500) {
        lastShown = millis();
        if(startMsg[currMsgBit] == '\0') {
          currMsgBit = 0;        
        }else {
          lcd_printMsg(startMsg + currMsgBit);
          currMsgBit++;
        }
      }

      if(buttonState == LOW) {
        mainMenu = true;
        firstTime = false;
      }
    }else {
      // Main Menu:
      if(clearedOnce == false) {
        lcd.clear();
        clearedOnce = true;
        lcd.setCursor(1,0);
        lcd.print("Space Invaders");
        lcd.setCursor(0,1);
      }
      
      // Ramane de adaugat cele patru optiuni: Play, Settings, HighScore, Info
    }
  }
}
