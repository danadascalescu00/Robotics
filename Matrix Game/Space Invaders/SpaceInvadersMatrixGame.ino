#include "pitches.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include "LedControl.h"

#define ARROW                        byte(0)
#define REVERSE_ARROW                byte(1)
#define NOTES                        3
#define MATRIX_DIMENSION             8
#define LIVES                        3
#define RACKET_OUT_OF_RANGE         -5
#define ENEMY_DESTROYED             -7

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

// It is used as a template
const byte matrix[] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};

uint16_t currMsgBit = 0;

const char startMsg[]    = " Press the red button to get started";
const char endMsg[]      = " Press the red button to exit";
const char skipMsg[]     = " You can skip the story anytime by pressing the red button";
const char gitHubLink[]  = " https://github.com/danadascalescu00/Robotics/tree/master/Matrix%20Game";
const char storyMsgI[]   = " SPACE INVADERS The Retro Game: Special Christmas Edition"; 
const char storyMsgII[]  = " Do you think you can conqueur the galaxy?";
const char storyMsgIII[] = " Everything will get harder as you play!";
const char storyMsgIV[]  = " Every level passed without losing life will get you a special power and bonus score";
const char legendMsg[]   = " LEGEND: SP = Special Power. You can activate it by pressing the red button ";

char Name[8];

const int redPin   = A2;
const int greenPin = A3;
const int bluePin  = A5;

const int pinSW = 9; // digtal pin connected to switch output
const int pinX  = A0; // A0 - analog pin connected to X output
const int pinY  = A1; // A1 - analog pin connceted to Y output

const int buzzerPin = 6;

const int pushButton = 8;

const int RS     = 12;
const int enable = 11;
const int D4     = 5;
const int D5     = 4; 
const int D6     = 3;
const int D7     = 2;
LiquidCrystal lcd(RS, enable, D4, D5, D6, D7);

LedControl lc = LedControl(13, 7, 10, 1); //DIN, CLK, LOAD, No. DRIVER

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
const long passingLevelInterval = 5000;
const long phaseInterval = 2000;

int xValue, yValue, buttonValue;
int switchState = LOW;

boolean joyMovedOx = false, joyMovedOy = false;
boolean firstTime = false, firstTimeRGB = false;
boolean firstLevel = false, clearedDisplayNewLevel = false;
boolean phaseI = false;
boolean introductionDisplayed = false;
boolean cleared = false, clearedNewLevel = false;
boolean clearedOnce = false;
boolean mainMenu = false;
boolean pressToStart = false;
boolean pressedToExit = false;
boolean setState = false, setLetter = true;
boolean buttonPressed = true;
boolean highScoreFirstLine = false;
boolean storyDisplayed = false;
boolean pressedOnce = false;
boolean switchToLetter = true;

int minThreshold = 300, maxThreshold = 800;

int buttonState = HIGH;  // the current reading from the input pin
int lastButtonState = HIGH; // the previous reading from the input pin
unsigned int option = 1;
unsigned int displacement = 1;
unsigned int score = 0, Highscore = 0;

int letter = 0, pos = 0;

// variables used for game:
boolean playerWon = false, gameOver = false, newLevelBegin = false, noDamageTakenCurrentLevel;
boolean displayed = false, updatedTopPlayersList = false, firstStarship = true;
int playerPos = 4, noOfEnemies = 12, currentLevel;
unsigned int level = 1, startingLevel = 1, lives = LIVES, specialPower = 0, enemyCounter = 0;
const int movementDelay = 100, firedDelay = 500, racketDelay = 15, enemyRacketDelay = 21;
const int noOfRackets = 5, noOfLevels = 5;
const int enemyCreateDelay = 5000, enemyFiredDelay = 1900, bigbossMovementDelay = 120, bigbossFiredDelay = 3000;
int enemyMovementDelay; 
unsigned long movementTime, firedTime, enemyCreateTime, enemyFiredTime, bigbossFiredTime;

struct Racket {
  int posX = RACKET_OUT_OF_RANGE, posY;
  unsigned long moveDelay;
}playerRackets[noOfRackets], enemyRackets[noOfRackets], bigbossRackets[noOfRackets];

struct Enemie {
  int posX, posY;
  boolean created, dead = false, firstTimeShoot = false;
  unsigned long createdTime, movementTime, firedTime;
};

struct Bigboss {
  int lives = 2 * LIVES;
  int posX, posY = 0;
  boolean created = false, dead = false, firstTimeShoot = false;
  unsigned long movementTime, firedTime;
}bigBoss;

struct SpecialRacket {
  int posX = RACKET_OUT_OF_RANGE, posY;
  unsigned long moveDelay;
}playerSpecialRackets[noOfRackets];

// Numbers of enemies generated each level until the big boss will appear
int enemiesGenerated[noOfLevels] = {3, 4, 5, 5, 0};

Enemie *enemies;

/* SOUNDS */
int boom = NOTE_C4;
const int boomNoteDuration = 350;
boolean loseSound = true, winSound = true;
int loseNoteDuration[] = {2, 4, 4};
int loseNotes[] = {NOTE_F4, NOTE_A4, NOTE_C5};
unsigned int buzzerState = HIGH;

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

// the currents address in the EEPROM for saving top three players highscore 
const int highscoreAddrI = 0, highscoreAddrII = 32, highscoreAddrIII = 64;

struct highscore {
  unsigned int scoreH = 0;
  char playerName[8] = "Unknown";
};

void setColors(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
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

void redButtonPressed() {
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
    restartGame();
    mainMenu = true;
    pressToStart = true;
    firstTime = false;
    displacement = 1;
    currMsgBit = 0;
    loseSound = true;
    storyDisplayed = false;
    updatedTopPlayersList = false;
  }
}

void buttonPressedToExit() {
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
    pressToStart = true;
    firstTime = false;
    displacement = 1;
    currMsgBit = 0;
    storyDisplayed = true;
  }
}

void lcdPrint(char *str) {
  uint8_t ch = 0;
  while(ch < 16 && str[ch] != '\0') {
    lcd.print((char)str[ch++]);
  }
  while(ch++ < 16) {
    lcd.print(" ");
  }
}

void lcdPrintMessage(char* message) {
  if(millis() > lastShown + 500) {
    lastShown = millis();
    if(message[currMsgBit] == '\0') {
      currMsgBit = 0;        
    }else {
      lcdPrint(message + currMsgBit);
      currMsgBit++;
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
  }else{
    if(score > highII.scoreH) {
      highscore newHigh;
      strcpy(newHigh.playerName, Name);
      newHigh.scoreH = score;
      
      EEPROM.put(highscoreAddrIII, highII);
      EEPROM.put(highscoreAddrII, newHigh);
    }else{
      if(score > highIII.scoreH) {
        highscore newHigh;
        strcpy(newHigh.playerName, Name);
        newHigh.scoreH = score;
      
        EEPROM.put(highscoreAddrIII, newHigh);      
      }
    }
  }
}

void viewTopPlayersList(int count) {
  highscore highI, highII, highIII;
  EEPROM.get(highscoreAddrI, highI);
  EEPROM.get(highscoreAddrII, highII);
  EEPROM.get(highscoreAddrIII, highIII);

  switch(count) {
    case 1: {
      lcd.setCursor(3,0);
      lcd.print("HIGHSCORE");
      lcd.setCursor(0,1);
      lcd.print("1.");
      lcd.setCursor(2,1);
      lcd.print(highI.playerName);
      lcd.setCursor(11,1);
      lcd.print(highI.scoreH);

      // scroll arrow:
      lcd.setCursor(15,1);
      lcd.write(REVERSE_ARROW);
      
      break;
    }
    case 2: {
      lcd.setCursor(0,0);
      lcd.print("2.");
      lcd.setCursor(2,0);
      lcd.print(highII.playerName);
      lcd.setCursor(11,0);
      lcd.print(highII.scoreH);

      lcd.setCursor(0,1);
      lcd.print("3.");
      lcd.setCursor(2,1);
      lcd.print(highIII.playerName);
      lcd.setCursor(11,1);
      lcd.print(highIII.scoreH);

      // scroll arrow:
      lcd.setCursor(15,0);
      lcd.write(ARROW);
      lcd.setCursor(15,1);
      lcd.write(REVERSE_ARROW);
      
      break;
    }
    case 3: {
      lcd.setCursor(0,0);
      lcdPrintMessage(endMsg);

      // scroll arrow:
      lcd.setCursor(7,1);
      lcd.write(ARROW);

      redButtonPressed();
      break;
    }
  }
}

void displayIntroduction() {
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
      }
    }
  }
}

void displayMenu(unsigned int option) {
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
      lcd.setCursor(14,1);
      lcd.write(ARROW);
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
      lcd.setCursor(14,0);
      lcd.write(ARROW);
      lcd.setCursor(15,0);
      lcd.write(REVERSE_ARROW);
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

void story() {
  currentLevel = level = startingLevel;
  enemyCounter = 0; 
  yValue = analogRead(pinY);
  if(yValue < minThreshold) {
    if(joyMovedOy == false) {
      displacement--;
      joyMovedOy = true;
    }
    if(displacement < 1) {
      displacement = 1;
    }
  }else {
    if(yValue > maxThreshold) {
      if(joyMovedOy == false) {
        displacement++;
        joyMovedOy = true;
      }
      if(displacement > 7) {
        displacement = 7;
      }
    }else{
      joyMovedOy = false;
    }
  }

  if(joyMovedOy == true) {
    currMsgBit = 0;
    lcd.clear();
  }
  switch(displacement) {
    case 1: {
      lcd.setCursor(0,0);
      lcdPrintMessage(skipMsg);
      
      // scroll arrow:
      lcd.setCursor(7,1);
      lcd.write(REVERSE_ARROW);

      buttonPressedToExit();
      break;
    }
    case 2: {
      lcd.setCursor(0,0);
      lcdPrintMessage(storyMsgI);
      
      // scroll arrow:
      lcd.setCursor(7,1);
      lcd.write(REVERSE_ARROW);
      
      buttonPressedToExit();
      break;
    }
    case 3: {
      lcd.setCursor(0,0);
      lcdPrintMessage(storyMsgII);
      
      //scroll arrows:
      lcd.setCursor(7,1);
      lcd.write(ARROW);
      lcd.setCursor(8,1);
      lcd.write(REVERSE_ARROW);
      
      buttonPressedToExit();
      break;
    }
    case 4: {
      lcd.setCursor(0,0);
      lcdPrintMessage(storyMsgIII);
      
      //scroll arrows:
      lcd.setCursor(7,1);
      lcd.write(ARROW);
      lcd.setCursor(8,1);
      lcd.write(REVERSE_ARROW);

      buttonPressedToExit();
      break;
    }
    case 5: {
      lcd.setCursor(0,0);
      lcdPrintMessage(storyMsgIV);
      
      //scroll arrows:
      lcd.setCursor(7,1);
      lcd.write(ARROW);
      lcd.setCursor(8,1);
      lcd.write(REVERSE_ARROW);
      
      buttonPressedToExit();
      break;
    }
    case 6: {
      lcd.setCursor(0,0);
      lcdPrintMessage(legendMsg);
      
      //scroll arrows:
      lcd.setCursor(7,1);
      lcd.write(ARROW);
      lcd.setCursor(8,1);
      lcd.write(REVERSE_ARROW);
      
      buttonPressedToExit();
      break;
      
    }
    case 7: {
      lcd.setCursor(0,0);
      lcdPrintMessage(startMsg);
      
      // scroll arrow:
      lcd.setCursor(7,1);
      lcd.write(ARROW);
      
      buttonPressedToExit();
      break;
    }
  }
}

void gameIsOver() {
  if(!updatedTopPlayersList) {
    updatedTopPlayersList = true;
    updateTopPlayersList();
  }
  
  if(playerWon == false) {
    if(loseSound) {
      lcd.setCursor(6,0);
      lcd.print("YOU");
      lcd.setCursor(5,1);
      lcd.print("LOSED!");
      loseSound = false;
      for(int note = 0; note < NOTES; note++) {
        int noteDuration = 500/ loseNoteDuration[note];
        tone(buzzerPin, loseNotes[note], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.50;
        delay(pauseBetweenNotes);
        noTone(buzzerPin);
      }
    }
    lcd.setCursor(3,0);
    lcd.print("YOU LOSED!");
  }else {
    if(winSound) {
      lcd.setCursor(6,0);
      lcd.print("YOU");
      lcd.setCursor(5,1);
      lcd.print("WON!");
      winSound = false;
      for(int note = 0; note < NOTES; note++) {
        int noteDuration = 500/ loseNoteDuration[note];
        tone(buzzerPin, loseNotes[note], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.20;
        delay(pauseBetweenNotes);
        noTone(buzzerPin);
      }
    }
    lcd.setCursor(4,0);
    lcd.print("YOU WON!");
  }
  //exit game and return to main menu
  lcd.setCursor(0,1);
  lcdPrintMessage(endMsg);
  redButtonPressed();
}

void showSettingsOptions(unsigned int count) {
  switch(count) {
    case 1: {
      lcd.setCursor(0,0);
      lcd.print(">Start level:");
      lcd.setCursor(13,0);
      lcd.print(startingLevel);
      lcd.setCursor(1,1);
      lcd.print("Player:");
      lcd.setCursor(8,1);
      lcd.print(Name);

      // scroll arrow:
      lcd.setCursor(15,0);
      lcd.write(REVERSE_ARROW);
      break;
    }
    case 2: {
      lcd.setCursor(1,0);
      lcd.print("Start level:");
      lcd.setCursor(13,0);
      lcd.print(startingLevel);
      lcd.setCursor(0,1);
      lcd.print(">Player:");
      lcd.setCursor(8,1);
      lcd.print(Name);
      
      // scroll arrow:
      lcd.setCursor(15,0);
      lcd.write(ARROW);
      lcd.setCursor(15,1);
      lcd.write(REVERSE_ARROW);
      break;
    }
    case 3: {
      lcd.setCursor(0,0);
      lcdPrintMessage(endMsg);
      redButtonPressed();
      
      // scroll arrow:
      lcd.setCursor(7,1);
      lcd.write(ARROW);
      break;
    }
  }
}

void displaySettings() {
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
        
        char let;
        if(joyMovedOy == true) {
          if(letter == 26) {
            let = ' ';
          }else{
            let = 'A' + letter;
          }
          lcd.print(let);
          Name[pos] = let;
        }
      
      if(switchToLetter == true) {
        lcd.setCursor(9+pos,1);
      }  
    }
    break;
   }
   case 3: {
      lcd.setCursor(0,1);
      lcd.print("               ");
      lcd.setCursor(0,0);
      lcdPrintMessage(endMsg);

      // scroll arrow:
      lcd.setCursor(15,1);
      lcd.write(ARROW);
      
      redButtonPressed();
      break;
    }  
  }
}

void displayHighscore() {
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
    currMsgBit = 0;
    lcd.clear();
  }

  viewTopPlayersList(displacement);  
}

void displayInfo() {
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
    currMsgBit = 0;
    lcd.clear();
  }

  switch(displacement) {
    case 1: {
      lcd.setCursor(1,0);
      lcd.print("Space Invaders");
      lcd.setCursor(0,1);
      lcd.print("@UnibucRobotics");

      // scroll arrow:
      lcd.setCursor(15,1);
      lcd.write(REVERSE_ARROW);
      break;
    }
    case 2: {
      lcd.setCursor(1,0);
      lcd.print("GitHub Link:");
      lcd.setCursor(0,1);

      lcdPrintMessage(gitHubLink);

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
      lcdPrintMessage(endMsg);

      // scroll arrow:
      lcd.setCursor(7,1);
      lcd.write(ARROW);

      redButtonPressed();
      break;
    }
  }
}

void refreshRackets() {
  for(int i = 0; i < noOfRackets; i++) {
    enemyRackets[i].posX = RACKET_OUT_OF_RANGE;
    playerRackets[i].posX = RACKET_OUT_OF_RANGE;
    playerSpecialRackets[i].posX = RACKET_OUT_OF_RANGE;
  }
}

void levelPassed() {
  unsigned long currentMillisBlinking = millis();
  redValue = blueValue;
  if(currentMillisBlinking - previousMillisBlinking >= interval) {
    previousMillisBlinking = currentMillisBlinking;
    if(greenValue == 255) {
      greenValue = 0;
    }else {
      greenValue = 255;
    }
    setColors(redValue, greenValue, blueValue);
  }
}

void newLevel() {
  if(!clearedNewLevel) {
   lcd.clear();
   clearedNewLevel = true; 
  }
  
  lcd.setCursor(3,0);
  lcd.print("You reached");
  lcd.setCursor(5,1);
  lcd.print("level ");
  lcd.setCursor(11,1);
  lcd.print(level);

  levelPassed();
  unsigned long newLevelMillis = millis();
  if(newLevelMillis - lastLevelMillis >= passingLevelInterval) {
    if(firstTimeRGB == false) {
      lastLevelMillis = newLevelMillis;
      firstTimeRGB = true;
    }else {
      level++;
      clearedNewLevel = false;
      newLevelBegin = true;
      clearedDisplayNewLevel = false;
      lastLevelMillis = 0;
      firstTimeRGB = false;
      displayed = true;
      refreshRackets();
      setColors(0, 0, 0);
      lcd.clear();
    }
  }
}

void checkMargins() {
  if(playerPos <= 0) {
    playerPos = 1;
  }
  if(playerPos >= 7) {
    playerPos = 6;
  }
}

void showPlayer() {
  checkMargins();
  lc.setLed(0, 7, playerPos, true);
  lc.setLed(0, 7, playerPos - 1, true);
  lc.setLed(0, 7, playerPos + 1, true);
  lc.setLed(0, 6, playerPos, true);

  lc.setLed(0, 7, playerPos, false);
  lc.setLed(0, 7, playerPos - 1, false);
  lc.setLed(0, 7, playerPos + 1, false);
  lc.setLed(0, 6, playerPos, false);
}

void getPlayerDecisions() {
  xValue = analogRead(pinX);
  switchState = !digitalRead(pinSW);
}

boolean buttonPressedPlayer() {
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
    return true;
  }else{
    return false;
  }
}

void playerShoot() {
  Racket racket;
  racket.posX = playerPos;
  racket.posY = 7;
  racket.moveDelay = millis();
  for(int i = 0; i < noOfRackets; i++) {
    if(playerRackets[i].posX == RACKET_OUT_OF_RANGE) {
      playerRackets[i] = racket;
      break;
    }
  }
}

void playerShootSpecialRacket() {
  SpecialRacket racket;
  racket.posX = playerPos;
  racket.posY = 7;
  racket.moveDelay = millis();
  for(int i = 0; i < noOfRackets; i++) {
    if(playerSpecialRackets[i].posX == RACKET_OUT_OF_RANGE) {
      playerSpecialRackets[i] = racket;
      break;
    }
  }
}

void getPlayerMovement() {
  getPlayerDecisions();
  
  if((xValue < minThreshold) and (millis() - movementTime > movementDelay)) {
    playerPos--;
    movementTime = millis();
  }

  if((xValue > maxThreshold) and (millis() - movementTime > movementDelay)) {
    playerPos++;
    movementTime = millis();
  }

  if((switchState == HIGH) and (millis() - firedTime > firedDelay)) {
    firedTime = millis();
    playerShoot();
  }

  if(buttonPressedPlayer() and (millis() - firedTime > firedDelay)) {
    if(specialPower > 0) {
      specialPower--;
      firedTime = millis();
      playerShootSpecialRacket();
    }
  }
}

void displayStatus() {
  lcd.setCursor(0,0);
  lcd.print("Lives:");
  lcd.setCursor(6,0);
  lcd.print(lives);
  lcd.setCursor(9,0);
  lcd.print("Level:");
  lcd.setCursor(15,0);
  lcd.print(currentLevel);
  lcd.setCursor(0,1);
  lcd.print("Score:");
  lcd.setCursor(6,1);
  lcd.print(score);
  lcd.setCursor(11,1);
  lcd.print("SP:");
  lcd.setCursor(14,1);
  lcd.print(specialPower);
}

void showRackets() {
  for(int i = 0; i < noOfRackets; i++) {
    if(playerRackets[i].posY == -1) {
      playerRackets[i].posX = RACKET_OUT_OF_RANGE;
    }
    if(playerRackets[i].posX != RACKET_OUT_OF_RANGE) {
      lc.setLed(0, playerRackets[i].posY, playerRackets[i].posX, true);
    }
  }
}

void updateRackets() {
  for(int i = 0; i < noOfRackets; i++) {
    if((playerRackets[i].posX != RACKET_OUT_OF_RANGE) and (millis() - playerRackets[i].moveDelay > racketDelay)){
      lc.setLed(0, playerRackets[i].posY, playerRackets[i].posX, false);
      playerRackets[i].moveDelay = millis();
      playerRackets[i].posY--;
    }
  }
}

Enemie* generateEnemiesCurrentLevel() {
  displayed = false;
  firstStarship = true;
  noDamageTakenCurrentLevel = true;
  
  currentLevel = level;
  const int currentLevelNumberOfEnemies = enemiesGenerated[currentLevel-1];
  noOfEnemies = currentLevelNumberOfEnemies;
  int enemyIncreaseSpeed = 0;
  for(int count = 0; count < currentLevel; count++) {
    enemyIncreaseSpeed = 22 * (count + 1);
  }
  enemyMovementDelay = 220 - enemyIncreaseSpeed;

  Enemie *enemies = new Enemie[currentLevelNumberOfEnemies];
  for(int i = 0; i < currentLevelNumberOfEnemies; i++) {
    enemies[i].posY = -2;
    enemies[i].posX = random(1,6);
    enemies[i].created = false;
    enemies[i].dead = false;
    enemies[i].createdTime = millis();
    enemies[i].movementTime = millis();
  }

  return enemies;
}

void enemyShoot(int enemyPosition) {
  Racket racket;
  racket.posX = enemyPosition;
  racket.posY = 0;
  racket.moveDelay = millis();
  for(int i = 0; i < noOfRackets; i++) {
    if(enemyRackets[i].posX == RACKET_OUT_OF_RANGE) {
      enemyRackets[i] = racket;
      break;
    }
  }
}

void checkEnemyShoot(int enemyPosition, int index) {
  if(enemies[index].firstTimeShoot == false) {
    enemies[index].firstTimeShoot = true;
    enemyFiredTime = millis();
  }else{
    if(millis() - enemyFiredTime > enemyFiredDelay) {
      enemyShoot(enemyPosition);
      enemyFiredTime = millis();
    }
  }
}

void showEnemies() {
  for(int i = 0; i < noOfEnemies; i++) {
    if(millis() - enemyCreateTime > enemyCreateDelay) {
      enemies[enemyCounter].posY = 1;
      enemies[enemyCounter].created = true;
      enemyCounter++;
      enemyCreateTime = millis();
    }

    if((enemies[i].created == true) and (enemies[i].dead == false)) {
      if(enemies[i-1].dead == true) {
        firstStarship = false;
        lc.setLed(0, enemies[i].posY - 1, enemies[i].posX - 1, true);
        lc.setLed(0, enemies[i].posY, enemies[i].posX, true);
        lc.setLed(0, enemies[i].posY - 1, enemies[i].posX + 1, true);
        checkEnemyShoot(enemies[i].posX, i);
      }else {
        if(firstStarship == true) {
          lc.setLed(0, enemies[i].posY - 1, enemies[i].posX - 1, true);
          lc.setLed(0, enemies[i].posY, enemies[i].posX, true);
          lc.setLed(0, enemies[i].posY - 1, enemies[i].posX + 1, true);
          checkEnemyShoot(enemies[i].posX, i);
        }
      }
    }
  }
}

void updateEnemyPosition() {
  for(int i = 0; i < noOfEnemies; i++) {
    if((enemies[i].created == true) and (enemies[i].dead == false)) {
      if(millis() - enemies[i].movementTime > enemyMovementDelay) {
        lc.setLed(0, enemies[i].posY - 1, enemies[i].posX - 1, false);
        lc.setLed(0, enemies[i].posY, enemies[i].posX, false);
        lc.setLed(0, enemies[i].posY - 1, enemies[i].posX + 1, false);
        enemies[i].movementTime = millis();

        int newPosition = random(0,8) % 3;
        if((newPosition == 0) or (newPosition == 2)) {
          enemies[i].posX--;
        }else {
          enemies[i].posX++;
        }

        // check margins for the new poition of the enemy:
        if(enemies[i].posX < 1) {
          enemies[i].posX = 1;
        }
        if(enemies[i].posX > 6) {
          enemies[i].posX = 6;
        }
      }
    }
  }
}

void showEnemiesRackets() {
  for(int i = 0; i < noOfRackets; i++) {
    if(enemyRackets[i].posY >= 8) {
      enemyRackets[i].posX = RACKET_OUT_OF_RANGE;
    }
    if(enemyRackets[i].posX != RACKET_OUT_OF_RANGE) {
      lc.setLed(0, enemyRackets[i].posY, enemyRackets[i].posX, true);
    }
  }
}

void updateEnemiesRackets() {
  for(int i = 0; i < noOfRackets; i++) {
    if((enemyRackets[i].posX != RACKET_OUT_OF_RANGE) and (millis() - enemyRackets[i].moveDelay > enemyRacketDelay)){
      lc.setLed(0, enemyRackets[i].posY, enemyRackets[i].posX, false);
      enemyRackets[i].moveDelay = millis();
      enemyRackets[i].posY++;
    }
  }
}

// check collision between player's racket and enemy
void checkRacketEnemyCollision() {
  for(int i = 0; i < noOfRackets; i++) {
    for(int j = 0; j < noOfEnemies; j++) {
      if((playerRackets[i].posX != RACKET_OUT_OF_RANGE) and (enemies[j].posX != ENEMY_DESTROYED)) {
        if((playerRackets[i].posX == enemies[j].posX and playerRackets[j].posY == enemies[j].posY) or
           (playerRackets[i].posX == enemies[j].posX - 1 and playerRackets[j].posY == enemies[j].posY - 1) or
           (playerRackets[i].posX == enemies[j].posX + 1 and playerRackets[j].posY == enemies[j].posY - 1) or
           (playerRackets[i].posX == enemies[j].posX and playerRackets[j].posY == enemies[j].posY - 1)) {

            score = score + 5;
            lc.setLed(0, enemies[j].posY - 1, enemies[j].posX - 1, false);
            lc.setLed(0, enemies[j].posY, enemies[j].posX, false);
            lc.setLed(0, enemies[j].posY - 1, enemies[j].posX + 1, false);
            enemies[j].dead = true;
            enemies[j].posX = ENEMY_DESTROYED;
            tone(buzzerPin, boom, boomNoteDuration);
        }
      }
    }
  }

  if(checkLevelOver()) {
    if(noDamageTakenCurrentLevel) {
      score = score + 100;
      specialPower++;
      displayStatus();
    }
    lcd.clear();
    displayed = false;
    firstStarship = true;
    newLevelBegin = false;
  }
}

// check collision between enemy's racket and player
void checkRacketPlayerCollision() {
  for(int i = 0; i < noOfRackets; i++) {
    if(enemyRackets[i].posX != RACKET_OUT_OF_RANGE) {
      if((enemyRackets[i].posX == playerPos and enemyRackets[i].posY == 6) or
          (enemyRackets[i].posX == playerPos - 1 and enemyRackets[i].posY == 7) or
          (enemyRackets[i].posX == playerPos + 1 and enemyRackets[i].posY == 6)) {
            lives--;
            enemyRackets[i].posX = RACKET_OUT_OF_RANGE;
            noDamageTakenCurrentLevel = false;
            tone(buzzerPin, boom, 200);
      }
    }
  }
}

boolean checkLevelOver() {
  int numberOfDeadEnemies = 0;
  for(int i = 0; i < noOfEnemies; i++) {
    if(enemies[i].dead == true) {
      numberOfDeadEnemies++;
    }
  }
  if(numberOfDeadEnemies == noOfEnemies) {
    enemyCounter = 0;
    delete[] enemies;
    return true;
  } 
  return false;
}

boolean checkGameOver() {
  if(lives == 0){
    playerWon = false;
    return true;
  }
  if(bigBoss.dead == true) {
    playerWon = true;
    return true;
  }
  return false;
}

void clearLedMatrix() {
  for(int row = 0; row < MATRIX_DIMENSION; row++) {
    lc.setRow(0, row, matrix[row]);
  }
}

void showSpecialRacket() {
  for(int i = 0; i < noOfRackets; i++) {
    if(playerSpecialRackets[i].posY == -1) {
      playerSpecialRackets[i].posX = RACKET_OUT_OF_RANGE;
    }
    if(playerSpecialRackets[i].posX != RACKET_OUT_OF_RANGE) {
      lc.setLed(0, playerSpecialRackets[i].posY, playerSpecialRackets[i].posX - 1, true);
      lc.setLed(0, playerSpecialRackets[i].posY, playerSpecialRackets[i].posX, true);
      lc.setLed(0, playerSpecialRackets[i].posY, playerSpecialRackets[i].posX + 1, true);
    }
  }
}

void updateSpecialRackets() {
  for(int i = 0; i < noOfRackets; i++) {
    if((playerSpecialRackets[i].posX != RACKET_OUT_OF_RANGE) and 
      (millis() - playerSpecialRackets[i].moveDelay > racketDelay)) {
        lc.setLed(0, playerSpecialRackets[i].posY, playerSpecialRackets[i].posX - 1, false);
        lc.setLed(0, playerSpecialRackets[i].posY, playerSpecialRackets[i].posX, false);
        lc.setLed(0, playerSpecialRackets[i].posY, playerSpecialRackets[i].posX + 1, false);
        playerSpecialRackets[i].moveDelay = millis();
        playerSpecialRackets[i].posY--;
    }
  }
}

void checkSpecialRacketCollision() {
  for(int i = 0; i < noOfRackets; i++) {
    for(int j = 0; j < noOfEnemies; j++) {
      if((playerSpecialRackets[i].posX != RACKET_OUT_OF_RANGE) and (enemies[j].posX != ENEMY_DESTROYED)) {
        if((playerSpecialRackets[i].posX == enemies[j].posX and playerSpecialRackets[j].posY == enemies[j].posY) or
           (playerSpecialRackets[i].posX == enemies[j].posX - 1 and playerSpecialRackets[j].posY == enemies[j].posY - 1) or
           (playerSpecialRackets[i].posX == enemies[j].posX + 1 and playerSpecialRackets[j].posY == enemies[j].posY - 1) or
           (playerSpecialRackets[i].posX - 1 == enemies[j].posX and playerSpecialRackets[j].posY == enemies[j].posY) or
           (playerSpecialRackets[i].posX - 1 == enemies[j].posX + 1 and playerSpecialRackets[j].posY == enemies[j].posY + 1) or
           (playerSpecialRackets[i].posX - 1 == enemies[j].posX - 1 and playerSpecialRackets[j].posY == enemies[j].posY - 1) or 
           (playerSpecialRackets[i].posX + 1 == enemies[j].posX and playerSpecialRackets[j].posY == enemies[j].posY) or
           (playerSpecialRackets[i].posX + 1 == enemies[j].posX + 1 and playerSpecialRackets[j].posY == enemies[j].posY + 1) or
           (playerSpecialRackets[i].posX + 1 == enemies[j].posX - 1 and playerSpecialRackets[j].posY == enemies[j].posY - 1)) {

            score = score + 10;
            lc.setLed(0, enemies[j].posY - 1, enemies[j].posX - 1, false);
            lc.setLed(0, enemies[j].posY, enemies[j].posX, false);
            lc.setLed(0, enemies[j].posY - 1, enemies[j].posX + 1, false);
            enemies[j].dead = true;
            enemies[j].posX = ENEMY_DESTROYED;
            tone(buzzerPin, boom, 200);
        }
      }
    }
  }
}


void starshipsFight() {
  if(newLevelBegin == true) {
    if(clearedDisplayNewLevel == false) {
      lcd.clear();
      clearedDisplayNewLevel = true;
    }
    
    displayStatus();
  
    showPlayer();
    getPlayerMovement();

    updateRackets();
    showRackets();
    checkRacketEnemyCollision();

    showEnemies();
    updateEnemyPosition();
    
    showEnemiesRackets();
    updateEnemiesRackets();
    checkRacketPlayerCollision();

    showSpecialRacket();
    updateSpecialRackets();

    checkSpecialRacketCollision();
  
    if(checkGameOver()) {
      lcd.clear();
      delete[] enemies;
      gameOver = true;
    }
  }else{
    clearLedMatrix();
    enemies = generateEnemiesCurrentLevel();
    if(currentLevel != startingLevel) {
      newLevelBegin = true;
      clearedDisplayNewLevel = false;
      while(displayed == false) {
        newLevel();
      }
    }else {
      level++;
      newLevelBegin = true;
    }
  }
}

void bigBossShoot() {
  Racket racket;
  racket.posX = bigBoss.posX;
  racket.posY = 0; 
  racket.moveDelay = millis();
  for(int i = 0; i < noOfRackets; i++) {
    if(bigbossRackets[i].posX == RACKET_OUT_OF_RANGE) {
      bigbossRackets[i] = racket;
      break;
    }
  }
}

void checkBigBossShoot() {
  if(bigBoss.firstTimeShoot == false) {
    bigBoss.firstTimeShoot = true;
    bigBoss.firedTime = millis();
  }else {
    if(millis() - bigBoss.firedTime > bigbossFiredDelay) {
      bigBossShoot();
      bigBoss.firedTime = millis();
    }
  }
}

void showBigBoss() {
  if(bigBoss.created == false) {
    bigBoss.posX = random(1,6);
    bigBoss.created = true;
    bigBoss.lives = 2 * LIVES;
  }
  lc.setLed(0, bigBoss.posY, bigBoss.posX - 1, true);
  lc.setLed(0, bigBoss.posY + 1, bigBoss.posX - 1, true);
  lc.setLed(0, bigBoss.posY, bigBoss.posX, true);
  lc.setLed(0, bigBoss.posY, bigBoss.posX + 1, true);
  lc.setLed(0, bigBoss.posY + 1, bigBoss.posX + 1, true);

  checkBigBossShoot();
}

void updateBigBoss() {
  if((bigBoss.created == true) and (bigBoss.dead == false)) {
    if(millis() - bigBoss.movementTime > bigbossMovementDelay) {
      lc.setLed(0, bigBoss.posY, bigBoss.posX - 1, false);
      lc.setLed(0, bigBoss.posY + 1, bigBoss.posX - 1, false);
      lc.setLed(0, bigBoss.posY, bigBoss.posX, false);
      lc.setLed(0, bigBoss.posY, bigBoss.posX + 1, false);
      lc.setLed(0, bigBoss.posY + 1, bigBoss.posX + 1, false);
      bigBoss.movementTime = millis();

      int newPosition = random(0,8) % 3;
      if((newPosition == 0) or (newPosition == 2)) {
        bigBoss.posX++;
      }else {
        bigBoss.posX--;
      }

      // check margins for the new poition of the enemy:
      if(bigBoss.posX < 1) {
        bigBoss.posX = 1;
      }
      if(bigBoss.posX > 6) {
        bigBoss.posX = 6;
      }
    }
  }
}

// check collision between player's racket and enemy
void checkRacketBigBossCollision() {
  for(int i = 0; i < noOfRackets; i++) {
    if((playerRackets[i].posX != RACKET_OUT_OF_RANGE) and (bigBoss.posX != ENEMY_DESTROYED)) {
        if((playerRackets[i].posX == bigBoss.posX and playerRackets[i].posY == bigBoss.posY) or
           (playerRackets[i].posX == bigBoss.posX - 1 and playerRackets[i].posY == bigBoss.posY + 1) or
           (playerRackets[i].posX == bigBoss.posX + 1 and playerRackets[i].posY == bigBoss.posY + 1) or
           (playerRackets[i].posX == bigBoss.posX - 1 and playerRackets[i].posY == bigBoss.posY ) or
           (playerRackets[i].posX == bigBoss.posX + 1 and playerRackets[i].posY == bigBoss.posY)) {

            score = score + 10;
            bigBoss.lives--;
            if(bigBoss.lives == 0) {
              gameOver = true;
              bigBoss.dead = true;
              bigBoss.posX = ENEMY_DESTROYED;
              lc.setLed(0, bigBoss.posY + 1, bigBoss.posX  - 1, false);
              lc.setLed(0, bigBoss.posY + 1, bigBoss.posX, false);
              lc.setLed(0, bigBoss.posY, bigBoss.posX, false);
              lc.setLed(0, bigBoss.posY, bigBoss.posX + 1, false);
              lc.setLed(0, bigBoss.posY + 1, bigBoss.posX + 1, false);
            }
            tone(buzzerPin, boom, 125);
            playerRackets[i].posX = RACKET_OUT_OF_RANGE;
        }
      }
  }
}

void checkSpecialRacketBigBossCollision() {
  for(int i = 0; i < noOfRackets; i++) {
    if((playerSpecialRackets[i].posX != RACKET_OUT_OF_RANGE) and (bigBoss.posX != ENEMY_DESTROYED)) {
      if((playerSpecialRackets[i].posX == bigBoss.posX and playerSpecialRackets[i].posY == bigBoss.posY) or
         (playerSpecialRackets[i].posX - 1 == bigBoss.posX and playerSpecialRackets[i].posY == bigBoss.posY) or
         (playerSpecialRackets[i].posX + 1 == bigBoss.posX and playerSpecialRackets[i].posY == bigBoss.posY) or
         (playerSpecialRackets[i].posX == bigBoss.posX - 1 and playerRackets[i].posY == bigBoss.posY + 1) or
         (playerSpecialRackets[i].posX == bigBoss.posX + 1 and playerSpecialRackets[i].posY == bigBoss.posY + 1) or
         (playerSpecialRackets[i].posX - 1 == bigBoss.posX  and playerSpecialRackets[i].posY == bigBoss.posY + 1) or
         (playerSpecialRackets[i].posX - 1 == bigBoss.posX + 1  and playerSpecialRackets[i].posY == bigBoss.posY + 1) or
         (playerSpecialRackets[i].posX + 1 == bigBoss.posX - 1  and playerSpecialRackets[i].posY == bigBoss.posY + 1)or
         (playerSpecialRackets[i].posX + 1 == bigBoss.posX + 1  and playerSpecialRackets[i].posY == bigBoss.posY + 1)) {

          score = score + 10;
          bigBoss.lives--;
          tone(buzzerPin, boom, 150);
          if(bigBoss.lives == 0) {
            bigBoss.posX = ENEMY_DESTROYED;
            bigBoss.dead = true;
            gameOver = true;
            lc.setLed(0, bigBoss.posY + 1, bigBoss.posX  - 1, false);
            lc.setLed(0, bigBoss.posY + 1, bigBoss.posX, false);
            lc.setLed(0, bigBoss.posY, bigBoss.posX, false);
            lc.setLed(0, bigBoss.posY, bigBoss.posX + 1, false);
            lc.setLed(0, bigBoss.posY + 1, bigBoss.posX + 1, false);
          }
      }
    }
  }
}

void showBigBossRackets() { 
  for(int i = 0; i < noOfRackets; i++) {
    if(bigbossRackets[i].posY >= 8) {
      bigbossRackets[i].posX = RACKET_OUT_OF_RANGE;
    }
    if(bigbossRackets[i].posX != RACKET_OUT_OF_RANGE) {
      lc.setLed(0, bigbossRackets[i].posY, bigbossRackets[i].posX - 1, true);
      lc.setLed(0, bigbossRackets[i].posY, bigbossRackets[i].posX + 1, true);
    }
  }
}

void updateBigBossRackets() {
  for(int i = 0; i < noOfRackets; i++) {
    if((bigbossRackets[i].posX != RACKET_OUT_OF_RANGE) and (millis() - bigbossRackets[i].moveDelay > enemyRacketDelay)) {
      lc.setLed(0, bigbossRackets[i].posY, bigbossRackets[i].posX - 1, false);
      lc.setLed(0, bigbossRackets[i].posY, bigbossRackets[i].posX + 1, false);
      bigbossRackets[i].posY++;
    }
  }
}

// check collision between big boss's racket and player
void checkRacketPlayerBBCollision() {
  for(int i = 0; i < noOfRackets; i++) {
    if(bigbossRackets[i].posX != RACKET_OUT_OF_RANGE) {
      if((bigbossRackets[i].posX - 1 == playerPos and bigbossRackets[i].posY == 6) or
          (bigbossRackets[i].posX - 1 == playerPos - 1 and bigbossRackets[i].posY == 7) or
          (bigbossRackets[i].posX == playerPos and bigbossRackets[i].posY == 6)) {
            lives--;
            bigbossRackets[i].posX = RACKET_OUT_OF_RANGE;
            tone(buzzerPin, boom, 150);
      }
    }
  }
}

void fightWithBigBoss() {
  displayStatus();
  
  showPlayer();
  getPlayerMovement();

  updateRackets();
  showRackets();

  showBigBoss();
  updateBigBoss();

  showBigBossRackets();
  updateBigBossRackets();
  
  checkRacketPlayerBBCollision();
  checkRacketBigBossCollision();

  showSpecialRacket();
  updateSpecialRackets();
  checkSpecialRacketBigBossCollision();
  
  if(checkGameOver()) {
    lcd.clear();
    gameOver = true;
  }
}

void game() {
  if(gameOver) {
    refreshRackets();
    gameIsOver();
  }else {
    if(firstTime == false) {
      Enemie *enemies = new Enemie;
      firstTime = true;
    }
    if(currentLevel < 5) {
      starshipsFight();
    }
    if(currentLevel == 5) {
      fightWithBigBoss();
    }
  }
}

void optionChoosed(unsigned int option) {
  switch(option) {
    case 1:{
      if(storyDisplayed == false) {
        restartGame();
        story();
      }else{
        game();
      }
      break;
    }
    case 2: {
      displaySettings();
      break;
    }
    case 3: {
      firstTime = false;
      displayHighscore();
      break;
    }
    case 4: {
      displayInfo();
      break;
    }
  }
}

void restartGame() {
  playerWon = gameOver = newLevelBegin = displayed = false;
  noDamageTakenCurrentLevel = true;
  level = startingLevel;
  storyDisplayed = false;
  for(int row = 0; row < MATRIX_DIMENSION; row++) {
    lc.setRow(0, row, matrix[row]);
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

  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 4); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  
  //Used to generate random numbers based on the noise from the analog pin 4, which I don't use
  randomSeed(analogRead(A4));
}

void loop() {
  if(introductionDisplayed == false) {
    displayIntroduction();
  }
  else {
    int reading = digitalRead(pushButton);
    if(reading != lastButtonState) {
      lastDebounceTime = millis();
    }
    if((millis() - lastDebounceTime) > debounceDelay) {
      if(reading != buttonState) {
        buttonState = reading;
      }
    }
    lastButtonState = reading;
 
    if(mainMenu == false && pressToStart == false) {
      if(cleared == false){
        lcd.clear();
        cleared = true;
      }
      lcd.setCursor(1,0);
      lcd.print("Space Invaders");
      lcd.setCursor(0,1);
      
      lcdPrintMessage(startMsg);

      if(buttonState == LOW) {
        mainMenu = true;
        pressToStart = true;
        firstTime = false;
      }
    }else {
      if(mainMenu == true) {
        if(clearedOnce == false) {
          lcd.clear();
          clearedOnce = true;
          lcd.setCursor(1,0);
        }
      
        displayMenu(option);

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
          displacement = 1;
          mainMenu = false; 
          lcd.clear();
          joyMovedOx = false;
          joyMovedOy = false;
        }
      }else{
        //MAIN MENU: switch to choose: Play, Settings, Highscore or Info
        optionChoosed(option);
      }
    }
  }
}
