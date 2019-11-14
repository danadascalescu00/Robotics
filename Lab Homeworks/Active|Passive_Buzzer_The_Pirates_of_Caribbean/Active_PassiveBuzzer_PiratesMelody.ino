#include "pitches.h"
#include "themes.h"

const int passiveBuzzerPin = A0;
const int activeBuzzerPin = 8;
const int pushButton = 2;

int buttonState;
int lastButtonState = LOW;
int speakerValue = 0;

bool buzzerActive = false;
bool knockHappened = false;
unsigned long knockDelay = 5000; //we wait for 5 seconds to play the melody

unsigned long lastKnock = 0;
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50;

//sensitivity
const int thresholdKnock = 25;

void Play_Pirates()
{
  for (int thisNote = 0; thisNote < (sizeof(Pirates_note)/sizeof(int)); thisNote++) {
    int noteDuration = 1000 / Pirates_duration[thisNote];//convert duration to time delay
    tone(8, Pirates_note[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.05; //Here 1.05 is tempo, increase to play it slower
    delay(pauseBetweenNotes);
    noTone(8);
    }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(passiveBuzzerPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(buzzerActive){
    //The melody will be stopped when we pressed the button
    if(buttonState == LOW) {
      Serial.println("Pirates of Caribbean theme song stops!");
      digitalWrite(activeBuzzerPin, LOW);
      knockHappened = buzzerActive = false;
      buttonState = HIGH;
    }
    else{
      int reading = digitalRead(pushButton);
      // If the switch changed, due to noise or pressing
      if(reading != lastButtonState){
        //reset the debouncing timer
        lastDebounceTime = millis(); 
      }
      
      if((millis() - lastDebounceTime) > debounceDelay){
        //if the button state has changed:
        if(reading != buttonState){
          buttonState = reading;
        }
      }
      lastButtonState = reading;
    }
  }
  else{
    if(knockHappened) {
      if(millis() - lastKnock >= knockDelay){
        Play_Pirates();
        buzzerActive = true;
      }
    }
    else {
      speakerValue = analogRead(passiveBuzzerPin);
      if(speakerValue > thresholdKnock){
        Serial.println("Knock");
        knockHappened = true;
        lastKnock = millis();
      }
    }
  }
}
