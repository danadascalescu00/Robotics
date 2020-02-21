const uint8_t buzzerPin = 45;
const int soundDuration = 50, periodDuration = 40;
unsigned long lastPeriodStart;

void sirenSetup() {
  pinMode(buzzerPin, OUTPUT);
}
 
void effects() {
  for(int hz = 440; hz < 1000; hz++) {
    tone(buzzerPin, hz, soundDuration);
    lastPeriodStart = millis();
    while(millis() - lastPeriodStart > soundDuration and millis() - lastPeriodStart <= soundDuration + 5) {
      noTone(buzzerPin);
    }
  }
}

uint8_t sirenDelay = 50;
uint64_t lastSiren;
 
// This function will be used only  one time in the setup, so the delay() won't interference with the functionality of the program  
void sirenSound() {
  lastSiren = millis();
  int hz = 440;
  while(hz < 1000) {
    if(millis() - lastSiren >= 5) {
      lastSiren = millis();      
      tone(buzzerPin, hz++, sirenDelay);
    }
  }
    while(hz >= 440) {
    if(millis() - lastSiren >= 5) {
      lastSiren = millis();
      tone(buzzerPin, hz--, sirenDelay);
    }
  }
}
