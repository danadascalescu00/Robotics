const int potPinB = A0; 
const int potPinG = A1;
const int potPinR = A2;
const int bluePin = 9; //BLUE
const int greenPin = 10; //GREEN
const int redPin = 11; //RED
unsigned int potValue_Blue = 0;
unsigned int potValue_Green = 0;
unsigned int potValue_Red = 0;
unsigned int blueValue = 0;
unsigned int greenValue = 0;
unsigned int redValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(potPinB, INPUT);
  pinMode(potPinG, INPUT);
  pinMode(potPinR, INPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  Serial.begin(9600);
}

void readPotValues(unsigned int &potValueBlue, unsigned int &potValueGreen, unsigned int &potValueRed)
{
  potValueBlue = analogRead(potPinB);
  potValueGreen = analogRead(potPinG);
  potValueRed = analogRead(potPinR);
}

void SetColors(unsigned int Red, unsigned int Green, unsigned int Blue)
{
  analogWrite(bluePin, Blue);
  analogWrite(greenPin, Green);
  analogWrite(redPin, Red);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  readPotValues(potValue_Blue, potValue_Green, potValue_Red);
   
  blueValue = map(potValue_Blue, 0, 1023, 0, 255);
  greenValue = map(potValue_Green, 0, 1023, 0, 255);
  redValue = map(potValue_Red, 0, 10023, 0, 255);

  SetColors(redValue, greenValue, blueValue);
}
