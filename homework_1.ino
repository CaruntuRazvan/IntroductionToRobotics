const int blueLedPin = 11;
const int greenLedPin = 10;
const int redLedPin = 9;

const int bluePotPin = A0;
const int greenPotPin = A1;
const int redPotPin = A2;

unsigned int greenPotValue = 0;
unsigned int bluePotValue = 0;
unsigned int redPotValue = 0;

unsigned int greenPinValue = 0;
unsigned int bluePinValue = 0;
unsigned int redPinValue = 0;

const int minPot = 0;
const int maxPot = 1023;
const int minLed = 0;
const int maxLed = 255;

void setup() {
  pinMode(redPotPin, INPUT);
  pinMode(greenPotPin, INPUT);
  pinMode(bluePotPin, INPUT);

  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //citirea valorilor potentiometrelor
  greenPotValue = analogRead(greenPotPin);
  bluePotValue = analogRead(bluePotPin);
  redPotValue = analogRead(redPotPin); 

  //calculare valoare analog pentru led
  greenPinValue = map(greenPotValue, minPot, maxPot, minLed, maxLed);
  bluePinValue = map(bluePotValue, minPot, maxPot, minLed, maxLed);
  redPinValue = map(redPotValue, minPot, maxPot, minLed, maxLed);

  analogWrite(redLedPin, redPinValue); 
  analogWrite(greenLedPin, greenPinValue);
  analogWrite(blueLedPin, bluePinValue);
  
}
