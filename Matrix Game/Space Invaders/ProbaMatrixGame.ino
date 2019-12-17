#include "pitches.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>

#define ARROW         byte(0)
#define REVERSE_ARROW byte(1)

uint8_t ch = 0;
uint16_t currMsgBit = 0;

const char startMsg[]   = "Press the red button to get started";
const char endMsg[]     = "Press the red button to exit";
const char githubLink[] = "https://github.com/danadascalescu00/Robotics/tree/master/Matrix%20Game";

char Name[8];

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

int xValue, yValue;
int switchState = LOW;

boolean joyMovedOx = false;
boolean joyMovedOy = false;
boolean firstTime = false;
boolean phaseI = false;
boolean introductionDisplayed = false;
boolean cleared = false;
boolean clearedOnce = false;
boolean mainMenu = false;
boolean pressToStart = false;
boolean pressedToExit = false;
boolean setState = false;
boolean buttonPressed = true;
boolean switchToLetter = true;
boolean highScoreFirstLine = false;


int minThreshold = 350;
int maxThreshold = 750;

int buttonState = HIGH;  // the current reading from the input pin
int lastButtonState = HIGH; // the previous reading from the input pin

unsigned int level = 1;
unsigned int startingLevel = 1;
unsigned int option = 1;
unsigned int displacement = 1;
unsigned int score = 0;
unsigned int Highscore = 0;

int letter = 0;
int pos = 0;

// the currents address in the EEPROM for saving top three players highscore 
const int highscoreAddrI = 0, highscoreAddrII = 32, highscoreAddrIII = 64;

byte downArrow[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};

byte upArrow[] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};

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

struct highscore {
  unsigned int scoreH = 0;
  char playerName[8] = "Unknown";
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

void red_button_pressed() {
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
  if(buttonState == LOW) {
    lcd.clear();
    mainMenu = true;
    pressToStart = true;
    firstTime = false;
    displacement = 1;
    currMsgBit = 0;
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

void display_menu(unsigned int option) {
  // set the cursor:
  switch(option){
    case 1: {
      lcd.setCursor(1,0);
      lcd.print("Space Invaders");
      lcd.setCursor(0,1);
      lcd.print(">Play Settings");

      // scroll arrow
      lcd.setCursor(15,1);
      lcd.write(REVERSE_ARROW);
      break;
    }
    case 2: {
      lcd.setCursor(1,0);
      lcd.print("Space Invaders");
      lcd.setCursor(0,1);
      lcd.print("Play >Settings");

      // scroll arrow
      lcd.setCursor(15,1);
      lcd.write(REVERSE_ARROW);
      break;
    }
    case 3: {
      lcd.setCursor(0,0);
      lcd.print(">Highscore");
      lcd.setCursor(0,1);
      lcd.print("Info");

      // scroll arrow
      lcd.setCursor(15,1);
      lcd.write(ARROW);
      break;
    }
    case 4: {
      lcd.setCursor(0,0);
      lcd.print("Highscore");
      lcd.setCursor(0,1);
      lcd.print(">Info");

      // scroll arrow
      lcd.setCursor(15,1);
      lcd.write(ARROW);
      break;
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

void option_choosed(unsigned int option) {
  switch(option) {
    case 1:{
      break;
    }
    case 2: {
      display_settings();
      break;
    }
    case 3: {
      firstTime = false;
      display_highscore();
      break;
    }
    case 4: {
      display_info();
      break;
    }
  }
}

void display_settings() {
  if(buttonPressed == true) {
    yValue = analogRead(pinY);
    if(yValue < minThreshold) {
      if(joyMovedOy == false) {
        displacement -= 1;
        joyMovedOy = true;
      }
      if(displacement < 1) {
        displacement = 1;
      }
    }else {
      if(yValue > maxThreshold) {
        if(joyMovedOy == false) {
          displacement += 1;
          joyMovedOy = true;
        }
        if(displacement > 3) {
          displacement = 3;
        }
      }else{
        joyMovedOy = false;
      }
    }

    if(joyMovedOy == true) {
      lcd.clear();
    }
  }

  switch(displacement) {
    case 1: {
      lcd.setCursor(0,0);
      lcd.print(">Start level: ");
      lcd.setCursor(13,0);
      lcd.print(startingLevel);
      lcd.setCursor(0,1);
      lcd.print(" Player: ");
      lcd.setCursor(9,1);
      lcd.print(Name);

      // scroll arrow:
      lcd.setCursor(15,0);
      lcd.write(REVERSE_ARROW);

      int switchValue = digitalRead(pinSW);
      if(switchValue == LOW) {
        setState = !setState;
      }

      if(setState == true) {
        // Set the starting level:
        buttonPressed = false;
        xValue = analogRead(pinX);
        if(xValue < minThreshold) {
          if(joyMovedOx == false) {
            startingLevel -= 1;
            joyMovedOx = true;
          }
          if(startingLevel < 1) {
            startingLevel = 1;
          }
        }else {
          if(xValue > maxThreshold) {
            if(joyMovedOx == false) {
              startingLevel += 1;
              joyMovedOx = true;
            }
            if(startingLevel > 5) {
              startingLevel = 5;
            }
          }else{
            joyMovedOx = false;
          }
        }
        
        lcd.setCursor(13,0);
        lcd.print(startingLevel);
  
        int switchValue = digitalRead(pinSW);
        if(switchValue == LOW) {
          buttonPressed = true;
        }
      }
    break;
   }
   case 2: {
    lcd.setCursor(0,0);
    lcd.print("Start level: ");
    lcd.setCursor(13,0);
    lcd.print(startingLevel);
    lcd.setCursor(0,1);
    lcd.print(">Player: ");
    lcd.setCursor(9,1);
    if(firstTime == false) {
      lcd.print("_______");
      firstTime = true;
    }else{
      lcd.print(Name);
    }
    // scroll arrow:
    lcd.setCursor(15,0);
    lcd.write(REVERSE_ARROW);

    int switchValue = digitalRead(pinSW);
    if(switchValue == LOW) {
      setState = !setState;
      Serial.println("here");
    }
    
    if(setState == true) {
      // Set player's name:
      buttonPressed = false;
      
      int switchVal = digitalRead(pinSW);
      if(switchVal == LOW and buttonPressed == true) {
        buttonPressed = true;
        setState = false;
      }else{
        if(switchVal == LOW and buttonPressed == false) {
          switchToLetter = true;
          buttonPressed = true;
          setState = true;
          }
      }
      
      xValue = analogRead(pinX);
      if(xValue < minThreshold) {
        if(joyMovedOx == false) {
          pos -= 1;
          joyMovedOx = true;
        }
        if(pos < 0) {
          pos = 0;
        }
      }else{
        if(xValue > maxThreshold) {
          if(joyMovedOx == false) {
            pos += 1;
            joyMovedOx = true; 
          }
          if(pos > 7) {
            pos = 7;
          }
        }else{
          joyMovedOx = false;
        }
       }

        yValue = analogRead(pinY);
        if(yValue < minThreshold) {
          if(joyMovedOy == false) {
            letter -= 1;
            joyMovedOy = true;
          }
          if(letter < 0) {
            letter = 0;
          }
        }else{
          if(yValue > maxThreshold) {
            if(joyMovedOy == false) {
              letter += 1;
              joyMovedOy = true; 
            }
            if(letter > 26) {
              letter = 26;
            }
          }else{
            joyMovedOy = false;
          }
        }
        if(joyMovedOy == true) {
          char let = 'A' + letter;
          lcd.print(let);
          Name[pos] = let;
          //switchToLetter = false;
        }
      

      if(switchToLetter == true) {
        //letter = 0;
        lcd.setCursor(9+pos,1);
        //lcd.print("_");
      }  
    }

    break;
   }
   case 3: {
      lcd.setCursor(0,1);
      lcd.print("               ");
      lcd.setCursor(0,0);
      if(millis() > lastShown + 500) {
        lastShown = millis();
        if(endMsg[currMsgBit] == '\0') {
          currMsgBit = 0;        
        }else {
          lcd_printMsg(endMsg + currMsgBit);
          currMsgBit++;
        }
      }

      // scroll arrow:
      lcd.setCursor(15,1);
      lcd.write(ARROW);
      
      red_button_pressed();
      
      break;
    }  
  }
}

void display_highscore() {
  yValue = analogRead(pinY);
  if(yValue < minThreshold) {
    if(joyMovedOy == false) {
      displacement -= 1;
      joyMovedOy = true;
    }
    if(displacement < 1) {
      displacement = 1;
    }
  }else {
    if(yValue > maxThreshold) {
      if(joyMovedOy == false) {
        displacement += 1;
        joyMovedOy = true;
      }
      if(displacement > 3) {
        displacement = 3;
      }
    }else{
      joyMovedOy = false;
    }
  }

  if(joyMovedOy == true) {
    lcd.clear();
  }

  switch(displacement) {
    case 1: {
      viewTopPlayersList(displacement);
      break;
    }
    case 2: {
      viewTopPlayersList(displacement);
      break;
    }
    case 3: {
      viewTopPlayersList(displacement);
      break;
    }
  }
}

void display_info() {
  yValue = analogRead(pinY);
  if(yValue < minThreshold) {
    if(joyMovedOy == false) {
      displacement -= 1;
      joyMovedOy = true;
    }
    if(displacement < 1) {
      displacement = 1;
    }
  }else {
    if(yValue > maxThreshold) {
      if(joyMovedOy == false) {
        displacement += 1;
        joyMovedOy = true;
      }
      if(displacement > 4) {
        displacement = 4;
      }
    }else{
      joyMovedOy = false;
    }
  }

  if(joyMovedOy == true) {
    lcd.clear();
  }

  switch(displacement) {
    case 1: {
      lcd.setCursor(1,0);
      lcd.print("Space Invaders");
      lcd.setCursor(0,1);
      lcd.print("@UnibucRobotics");

      // scroll arrow:
      lcd.setCursor(15,0);
      lcd.write(REVERSE_ARROW);
      break;
    }
    case 2: {
      lcd.setCursor(1,0);
      lcd.print("GitHub Link:");
      lcd.setCursor(0,1);

      if(millis() > lastShown + 700) {
        lastShown = millis();
        if(githubLink[currMsgBit] == '\0') {
          currMsgBit = 0;        
        }else {
          lcd_printMsg(githubLink + currMsgBit);
          currMsgBit++;
        }
      }

      // scroll arrow:
      lcd.setCursor(14,0);
      lcd.write(ARROW);
      lcd.setCursor(15,0);
      lcd.write(REVERSE_ARROW);
      
      break;
    }
    case 3: {
      lcd.setCursor(1,0);
      lcd.print("Creater:");
      lcd.setCursor(0,1);
      lcd.print("Dana Dascalescu");
      
      // scroll arrow:
      lcd.setCursor(14,0);
      lcd.write(ARROW);
      lcd.setCursor(15,0);
      lcd.write(REVERSE_ARROW);
      
      break;
    }
    case 4: {
      lcd.setCursor(0,0);
      if(millis() > lastShown + 500) {
        lastShown = millis();
        if(endMsg[currMsgBit] == '\0') {
          currMsgBit = 0;        
        }else {
          lcd_printMsg(endMsg + currMsgBit);
          currMsgBit++;
        }
      }

      // scroll arrow:
      lcd.setCursor(15,1);
      lcd.write(ARROW);

      red_button_pressed();
      
      break;
    }
  }
}

void updateTopPlayersList() {
  highscore highI, highII, highIII;
  EEPROM.get(highscoreAddrI, highI);
  EEPROM.get(highscoreAddrII, highII);
  EEPROM.get(highscoreAddrIII, highIII);

  if(score > highI.scoreH) {
    highscore newHigh;
    strcpy(newHigh.playerName, Name);
    newHigh.scoreH = score;

    EEPROM.put(highscoreAddrIII, highII);
    EEPROM.put(highscoreAddrII, highI);
    EEPROM.put(highscoreAddrI, newHigh);
  }
}

void viewTopPlayersList(int count) {
  highscore highI, highII, highIII;
  EEPROM.get(highscoreAddrI, highI);
  EEPROM.get(highscoreAddrII, highII);
  EEPROM.get(highscoreAddrIII, highIII);

  switch(count) {
    case 1: {
      while(1) {
        yValue = analogRead(pinY);
        if(yValue > maxThreshold)
          break;
        
        lcd.setCursor(3,0);
        lcd.print("HIGHSCORE");
        lcd.setCursor(0,1);
        lcd.print("1.");
        lcd.setCursor(3,1);
        lcd.print(highI.playerName);
        lcd.setCursor(12,1);
        lcd.print(highI.scoreH);
      }
      break;
    }
    case 2: {
      while(1) {
        yValue = analogRead(pinY);
        if(yValue > maxThreshold)
          break;

        lcd.setCursor(0,0);
        lcd.print("2.");
        lcd.setCursor(3,0);
        lcd.print(highII.playerName);
        lcd.setCursor(12,0);
        lcd.print(highII.scoreH);

        lcd.setCursor(0,1);
        lcd.print("3.");
        lcd.setCursor(3,1);
        lcd.print(highIII.playerName);
        lcd.setCursor(12,1);
        lcd.print(highIII.scoreH);
      }
      break;
    }
    case 3: {
      while(1) {
        yValue = analogRead(pinY);
        if(yValue > maxThreshold)
          break;
      
        lcd.setCursor(0,0);
        if(millis() > lastShown + 500) {
          lastShown = millis();
          if(endMsg[currMsgBit] == '\0') {
            currMsgBit = 0;        
          }else {
            lcd_printMsg(endMsg + currMsgBit);
            currMsgBit++;
          }
        }
      
        red_button_pressed();
      }
      break;
    }
  }
}

void setup() {
  strcpy(Name, "Unknown");
  Name[8] = '\0';
   
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(pinSW, INPUT_PULLUP); //activate pull-up resistor on the pushbutton pin of joystick  
  pinMode(buzzerPin, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
  
  lcd.begin(16, 2);
  lcd.createChar(ARROW, upArrow);
  lcd.home();
  lcd.createChar(REVERSE_ARROW, downArrow);
  lcd.home();
  
  //Serial.begin(9600);
}

void loop() {
//  highscore highI;
//  EEPROM.put(highscoreAddrI, highI);
//  EEPROM.put(highscoreAddrII, highI);
//  EEPROM.put(highscoreAddrIII, highI);
  updateTopPlayersList(); // MOVE TO GAME MODE
  
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
 
    if(mainMenu == false && pressToStart == false) {
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
        pressToStart = true;
        firstTime = false;
      }
    }else {
      // Main Menu:
      if(mainMenu == true) {
        if(clearedOnce == false) {
          lcd.clear();
          clearedOnce = true;
          lcd.setCursor(1,0);
          lcd.print("Space Invaders");
          lcd.setCursor(0,1);
        }
      
        display_menu(option);

        yValue = analogRead(pinY);

        if(yValue < minThreshold) {
          if(joyMovedOy == false) {
            option -= 1;
            joyMovedOy = true;
          }
          if(option < 1) {
            option = 1;
          }
        }else {
          if(yValue > maxThreshold) {
            if(joyMovedOy == false) {
              option += 1;
              joyMovedOy = true;
            }
            if(option > 4) {
              option = 4;
            }
          }else {
            joyMovedOy = false;
          }
        }

        if(joyMovedOy == true) {
          lcd.clear();
        }

        int switchValue = digitalRead(pinSW);
        if(switchValue == LOW) {
          mainMenu = false;
          lcd.clear();
          joyMovedOx = false;
          joyMovedOy = false;
        }
      }else{
        //switch to choose: Play, Settings, Highscore or Info
        option_choosed(option);
      }
    }
  }
}
