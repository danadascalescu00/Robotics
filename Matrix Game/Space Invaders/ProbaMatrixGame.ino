#include "pitches.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include "LedControl.h"

#define ARROW            byte(0)
#define REVERSE_ARROW    byte(1)
#define NOTES            3
#define MATRIX_DIMENSION 8
#define LIVES            3

uint8_t ch = 0;
uint16_t currMsgBit = 0;

const char startMsg[]    = " Press the red button to get started";
const char endMsg[]      = " Press the red button to exit";
const char githubLink[]  = " https://github.com/danadascalescu00/Robotics/tree/master/Matrix%20Game";
const char storyMsgI[]   = " SPACE INVADERS The Retro Game: Special Christmas Edition"; 
const char storyMsgII[]  = " Do you think you can conqueur the galaxy?";
const char storyMsgIII[] = " Everything will get harder as you play!";
const char storyMsgIV[]  = " Every level passed without losing life will get you a special power";

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
LedControl lc = LedControl(13, 7, 10, 1); //DIN, CLK, LOAD, No. DRIVER

unsigned int redValue   = 0;
unsigned int blueValue  = 0;
unsigned int greenValue = 0;

unsigned int enemieMovementSpeed = 1000;
unsigned long previousMillisBlinking = 0;
unsigned long lastLevelMillis = 0;
unsigned long lastPhaseDuration = 0;
unsigned long lastShown = 0;
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50; // the debounce time; increase if the output flickers

const long interval = 250;
const long passingLevelInterval = 2500;
const long phaseInterval = 2000;

int xValue, yValue, buttonValue;
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
boolean playerWon = false;
boolean gameOver = false;
boolean storyDisplay = true;

int minThreshold = 350;
int maxThreshold = 750;

int buttonState = HIGH;  // the current reading from the input pin
int lastButtonState = HIGH; // the previous reading from the input pin

unsigned int level = 1;
unsigned int lives = LIVES;
unsigned int startingLevel = 1;
unsigned int option = 1;
unsigned int displacement = 1;
unsigned int score = 0;
unsigned int Highscore = 0;

int letter = 0;
int pos = 0;

// variables used for game:
int playerPos = 4, enemyCounter = 0;
const int movementDelay = 100, enemyCreateDelay = 4000, shootDelay = 400, bulletDelay = 8;
unsigned long movementTime, shootTime, enemyCreateTime = 0;
boolean noDamageTakenCurrentLevel = true;
const int noOfBullets = 3, noOfLevels = 6, maxNumberOfEnemies = 6;
int noOfEnemies = 0, enemiesDefeated;
unsigned long randomNumber;
int enemieCounter = 0;

//Numbers of enemies generated each level
int levels[maxNumberOfEnemies] = {3, 6, 9, 12, 15, 0};

struct Enemie {
  unsigned long createdTime, movementTime, bulletTime;
  int posX = 4, posY = 0;
  boolean created = false, notDead = true;
};

struct Bullet {
  unsigned long moveDelay;
  int posX = - 2, posY;  
}bullets[noOfBullets], enemies_bullets[noOfBullets];

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

/* SOUNDS */
int boom = NOTE_C4;
boolean loseSound = true;
int loseNoteDuration[] = {2, 4, 4};
int loseNotes[] = {NOTE_F4, NOTE_A4, NOTE_C5};

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
    playerWon = false;
    loseSound = true;
    gameOver = false;
  }
}

void button_pressed() {
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

void lcd_printMsg(char *str) {
  uint8_t ch = 0;
  while(ch < 16 && str[ch] != '\0') {
    lcd.print((char)str[ch++]);
  }
  while(ch++ < 16) {
    lcd.print(" ");
  }
}

boolean checkGameOver() {
  if(lives == 0) {
    // player losed
    gameOver= true;
    playerWon = false;
    return true;
  }else {
    if(level == 5) {
      // enemy defeated
      gameOver = true;
      playerWon = true;
      return true;
    }
    return false;
  }
}

void checkMargins() {
  if(playerPos == 0) {
    playerPos = 1;
  }
  if(playerPos == 7) {
    playerPos = 6;
  }
}

int checkMarginsEnemie(int pos) {
  if(pos == 0) {
    pos = 1;
  }else {
    if(pos == 7) {
      pos = 6;
    }
  }
  return pos;
}

void readPlayerDecisions() {
  xValue = analogRead(pinX);
  switchState = !digitalRead(pinSW);
  buttonValue = !digitalRead(pushButton);
}

void story() {
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
      if(displacement > 5) {
        displacement = 5;
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
      if(millis() > lastShown + 700) {
        lastShown = millis();
        if(storyMsgI[currMsgBit] == '\0') {
          currMsgBit = 0;        
        }else {
          lcd_printMsg(storyMsgI + currMsgBit);
          currMsgBit++;
        }
      }

      // scroll arrow:
      lcd.setCursor(7,1);
      lcd.write(REVERSE_ARROW);
      
      break;
    }
    case 2: {
      lcd.setCursor(0,0);
      if(millis() > lastShown + 700) {
        lastShown = millis();
        if(storyMsgII[currMsgBit] == '\0') {
          currMsgBit = 0;        
        }else {
          lcd_printMsg(storyMsgII + currMsgBit);
          currMsgBit++;
        }
      }

      // scroll arrow:
      lcd.setCursor(7,1);
      lcd.write(REVERSE_ARROW);
      
      break;
    }
    case 3: {
      lcd.setCursor(0,0);
      if(millis() > lastShown + 700) {
        lastShown = millis();
        if(storyMsgIII[currMsgBit] == '\0') {
          currMsgBit = 0;        
        }else {
          lcd_printMsg(storyMsgIII + currMsgBit);
          currMsgBit++;
        }
      }

      // scroll arrow:
      lcd.setCursor(7,1);
      lcd.write(REVERSE_ARROW);
      
      break;
    }
    case 4: {
      lcd.setCursor(0,0);
      if(millis() > lastShown + 700) {
        lastShown = millis();
        if(storyMsgIV[currMsgBit] == '\0') {
          currMsgBit = 0;        
        }else {
          lcd_printMsg(storyMsgIV + currMsgBit);
          currMsgBit++;
        }
      }

      // scroll arrow:
      lcd.setCursor(7,1);
      lcd.write(REVERSE_ARROW);
      
      break;
    }
    case 5: {
      lcd.setCursor(1,0);
      lcd.print("Are you ready?");
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

      button_pressed();
      
      break;
    }
  }
}

void playerShoot() {
  // different missile firing mechanism:
  Bullet bullet;
  bullet.posX = playerPos;
  bullet.posY = 7;
  bullet.moveDelay = millis();
  for(int i = 0; i < noOfBullets; i++) {
    if(bullets[i].posX == -2) {
      bullets[i] = bullet;
      break;
    }
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

void getPlayerMovement() {
  readPlayerDecisions();
  if((xValue < minThreshold) and (millis() - movementTime > movementDelay)) {
    playerPos--;
    movementTime = millis();
  }

  if((xValue > maxThreshold) and (millis() - movementTime > movementDelay)) {
    playerPos++;
    movementTime = millis();
  }

  if((switchState == HIGH) and (millis() - shootTime > shootDelay)) {
    playerShoot();
    shootTime = millis();
  }
}

void updateRacket() {
  for(int i = 0; i < noOfBullets; i++) {
    if((bullets[i].posX != -2) and (millis() - bullets[i].moveDelay > bulletDelay)){
      lc.setLed(0, bullets[i].posY, bullets[i].posX, false);
      bullets[i].moveDelay = millis();
      bullets[i].posY--;
    }
  }
}

void showRacket() { 
  for(int i = 0; i < noOfBullets; i++) {
    if(bullets[i].posY == -1) {
      bullets[i].posX = -2;
    }
    if(bullets[i].posX != -2) {
      lc.setLed(0, bullets[i].posY, bullets[i].posX, true);
    }
  }
}

boolean checkLevelOver(Enemie *enemies) {
  int numberOfDeadEnemies = 0;
  for(int i = 0; i < noOfEnemies; i++) {
    if(enemies[i].posX == -1) {
      numberOfDeadEnemies++;
    }
  }
  
  if(numberOfDeadEnemies == noOfEnemies) {
    delete[] enemies;
    return true;
  }
  return false;
}

void updateEnemies(Enemie *enemies) {
  for(int i = 0; i < noOfEnemies; i++) {
    if((enemies[i].created == true) and (enemies[i].notDead == true)) {
      if(millis() - enemies[i].movementTime > enemieMovementSpeed) {
        lc.setLed(0, enemies[i].posY - 1, enemies[i].posX - 1, false);
        lc.setLed(0, enemies[i].posY, enemies[i].posX, false);
        lc.setLed(0, enemies[i].posY - 1, enemies[i].posX + 1, false);
        enemies[i].posX++;
      }
    }
  }
}

void showEnemies(Enemie *enemies) {
  for(int i = 0; i < noOfEnemies; i++) {
    if(enemies[i].notDead == false) {
      enemies[i].posX = - 5;
    }
    
    if(millis() - enemyCreateTime > enemyCreateDelay) {
      enemies[enemyCounter].created = true;
      enemyCounter++;
      enemyCreateTime = millis();
    }

    if((enemies[i].created == true) and (enemies[i].notDead == true)) {
        if((enemies[i].posX >= 0) and (enemies[i].posY <= 8)) {
          int newPos = random(0,8) % 3;
          if((newPos == 0) or (newPos == 2)) {
            int position = checkMarginsEnemie(enemies[i].posX-newPos);
            lc.setLed(0, enemies[i].posY + 1, position - 1, true);
            lc.setLed(0, enemies[i].posY, position, true);
            lc.setLed(0, enemies[i].posY + 1, position + 1, true);
          }else{
            int position = checkMarginsEnemie(enemies[i].posX + newPos - 1);
            lc.setLed(0, enemies[i].posY + 1, position - 1, true);
            lc.setLed(0, enemies[i].posY, position, true);
            lc.setLed(0, enemies[i].posY + 1, position + 1, true);
          }
        }
    }
  }
}

void checkBulletEnemiesCollision(Enemie *enemies) {
  for(int i = 0; i < noOfBullets; i++) {
    for(int j = 0; j < noOfEnemies; j++) {
      if((bullets[i].posX != -2) and (enemies[i].posX != -5)) {
        if ((bullets[i].posX == enemies[j].posX and bullets[i].posY == enemies[j].posY) or
            (bullets[i].posX == enemies[j].posX - 1 and bullets[i].posY == enemies[j].posY - 1) or
            (bullets[i].posX == enemies[j].posX + 1 and bullets[i].posY == enemies[j].posY - 1) or
            (bullets[i].posX == enemies[j].posX and bullets[i].posY == enemies[j].posY - 1) or
            (bullets[i].posX == enemies[j].posX - 1 and bullets[i].posY == enemies[j].posY - 2) or
            (bullets[i].posX == enemies[j].posX + 1 and bullets[i].posY == enemies[j].posY - 2)) {
              lc.setLed(0, bullets[i].posY, bullets[i].posX, false);
              lc.setLed(0, enemies[j].posY - 1, enemies[j].posX - 1, false); 
              lc.setLed(0, enemies[j].posY, enemies[j].posX, false);
              lc.setLed(0, enemies[j].posY - 1, enemies[j].posX + 1, false);
              enemies[j].notDead = false;
              bullets[i].posX = -2;
              score += (level * lives ) % enemiesDefeated;
              enemiesDefeated++;
            }
      }
    }
  }
}

void game() {
  if(gameOver == false) {
    //the game:
    Enemie *enemies = new Enemie[maxNumberOfEnemies];
    noOfEnemies = maxNumberOfEnemies;
    enemiesDefeated = 0;
    while(true) {
      lcd.setCursor(0,0);
      lcd.print("Lives ");
      lcd.setCursor(6,0);
      lcd.print(lives);

      lcd.setCursor(8,0);
      lcd.print("Score");
      lcd.setCursor(14,0);
      lcd.print(score);
      
      showPlayer();
      getPlayerMovement();

      updateRacket();
      showRacket();

      checkBulletEnemiesCollision(enemies);

      updateEnemies(enemies);
      showEnemies(enemies);

      if(enemiesDefeated % 20 == 0) {
        score += (level * lives * score) % 100 ;
      }

      if(enemieCounter == 6) {
        enemieCounter = 0;
      }
    
      if(checkGameOver()) {
        game_over();
      }
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

        // scroll arrow:
        lcd.setCursor(15,0);
        lcd.write(REVERSE_ARROW);
      }
      break;
    }
  }
}

void game_over() {
  updateTopPlayersList();
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
  }
  //exit game and return to main menu
  lcd.setCursor(5,1);
  lcd.print("      ");
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

void restart_game() {
  playerWon = false;
  level = startingLevel;

  for(int row = 0; row < MATRIX_DIMENSION; row++) {
    lc.setRow(0, row, matrix[row]);
  }
}

void option_choosed(unsigned int option) {
  switch(option) {
    case 1:{
      if(storyDisplay == true) {
        pressToStart = false;
        storyDisplay = false;
        currMsgBit = 0;
        while(pressToStart == false) {
          story();
        }
      }
      if(firstTime == false) {
        gameOver = false;
        firstTime = true;
        lcd.clear();
        restart_game();
      }
      game();
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
        lcd.setCursor(9+pos,1);
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
    currMsgBit = 0;
    lcd.clear();
  }

  switch(displacement) {
    case 1: {
      viewTopPlayersList(1);
      displacement = 3;
      break;
    }
    case 2: {
      viewTopPlayersList(2);
      displacement = 3;
      break;
    }
    case 3: {
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
      lcd.setCursor(0,1);
      lcd.write(ARROW);
      
      red_button_pressed();
            
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
  lc.setIntensity(0, 5); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  //Serial.begin(9600);

  randomSeed(analogRead(A4));
}

void loop() {

  //updateTopPlayersList(); // MOVE TO GAME MODE
 
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
        //MAIN MENU: switch to choose: Play, Settings, Highscore or Info
        option_choosed(option);
      }
    }
  }
}
