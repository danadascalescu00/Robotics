#ifndef FIRESIREN_INCLUDED
#define FIRESIREN_INCLUDED

const uint8_t buzzerPin = 45;
const int soundDuration = 50, periodDuration = 40;
unsigned long lastPeriodStart;

void sirenSetup() {
  pinMode(buzzerPin, OUTPUT);
}

void effects() {
  for (int hz = 440; hz < 1000; hz++) {
    tone(buzzerPin, hz, soundDuration);
    lastPeriodStart = millis();
    while (millis() - lastPeriodStart > soundDuration and millis() - lastPeriodStart <= soundDuration + 5) {
      noTone(buzzerPin);
    }
  }
}

uint8_t sirenDelay = 50;
uint64_t lastSiren = 0;
int hz = 440;
bool ascending = true;

// This function will be used only  one time in the setup, so the delay() won't interference with the functionality of the program
void playSiren() {
  //Serial.println(hz);
  if (ascending) {
    if (hz < 1000) {
      if (millis() - lastSiren >= 5) {
        lastSiren = millis();
        tone(buzzerPin, hz++, sirenDelay);
      }
    } else {
      ascending = false;
    }
  } else {
    if (hz >= 440) {
      if (millis() - lastSiren >= 5) {
        lastSiren = millis();
        tone(buzzerPin, hz--, sirenDelay);
      }
    } else {
      ascending = true;
    }
  }
}

void resetSiren() {
  hz = 440;
}

#endif
