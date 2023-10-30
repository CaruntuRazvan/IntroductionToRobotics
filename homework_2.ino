const int elevatorLed = 5;
const int floorLed1 = 9;
const int floorLed2 = 10;
const int floorLed3 = 11;
const int buttonLed1 = 2;
const int buttonLed2 = 3;
const int buttonLed3 = 4;
const int buzzerPin = 6;

const unsigned int blinkInterval = 200;
const unsigned int floorChangeTime = 3000;
const unsigned int debounceDelay = 50;  
const unsigned int doorsCloseTime = 1500;

int currentFloor = 1;
int targetFloor = 1;
bool elevatorMoving = false;
unsigned long lastActionTime = 0;
unsigned long lastBlinkTime = 0;
bool elevatorLedState = HIGH;
unsigned long buttonPressTime = 0;
bool button1Pressed = false;
bool button2Pressed = false;
bool button3Pressed = false;
unsigned long buttonPressStartTime = 0;

const int soundMovingFrequency = 500;
const int soundMovingDuration = 7000;
const int soundArrivalFrequency = 4000;
const int soundArrivalDuration = 200;

void setup() {  
  pinMode(elevatorLed, OUTPUT);
  pinMode(floorLed1, OUTPUT);
  pinMode(floorLed2, OUTPUT);
  pinMode(floorLed3, OUTPUT);
  pinMode(buttonLed1, INPUT_PULLUP);
  pinMode(buttonLed2, INPUT_PULLUP);
  pinMode(buttonLed3, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(floorLed1, HIGH);  // Turn on the LED on floor 1 (initial state)
}

void loop() {
  checkButtonPress(buttonLed1, 1, button1Pressed);
  checkButtonPress(buttonLed2, 2, button2Pressed);
  checkButtonPress(buttonLed3, 3, button3Pressed);

  if (elevatorMoving) {
    moveToNextFloor();
    digitalWrite(elevatorLed, elevatorLedState);
  }
  else{
    digitalWrite(elevatorLed, HIGH);
  }
}

void checkButtonPress(int buttonPin, int destinationFloor, bool &buttonPressed) {
  bool isButtonLow = digitalRead(buttonPin) == LOW;
  unsigned long currentTime = millis();

  if (!buttonPressed && isButtonLow) {
    buttonPressed = true;
    buttonPressTime = currentTime;
    // Serial.println("Button pressed at " + String(buttonPressTime));
  } else if (buttonPressed && !isButtonLow && (currentTime - buttonPressTime) > debounceDelay) {
    buttonPressed = false;
    buttonPressStartTime = currentTime;

    if (destinationFloor != currentFloor && !elevatorMoving) {
      targetFloor = destinationFloor;
      elevatorMoving = true;
      soundMoving();
      lastActionTime = currentTime;
    }
  }
}

void moveToNextFloor() {
  unsigned long currentTime = millis();

  if (currentTime - lastActionTime >= floorChangeTime) {
    lastActionTime = currentTime;
    currentFloor += (currentFloor < targetFloor) ? 1 : -1;
    updateFloorLeds(currentFloor);

    if (currentFloor == targetFloor) {
      elevatorMoving = false;
      soundArrival();
      digitalWrite(elevatorLed, HIGH);
    }
  }

  if (buttonPressStartTime > 0 && (currentTime - buttonPressStartTime) >= doorsCloseTime) {
    if (currentTime - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = currentTime;
      elevatorLedState = !elevatorLedState;
    }
  }
}

void updateFloorLeds(int floor) {
  digitalWrite(floorLed1, (1 == floor));
  digitalWrite(floorLed2, (2 == floor));
  digitalWrite(floorLed3, (3 == floor));
}

void soundMoving() {
  tone(buzzerPin, soundMovingFrequency, soundMovingDuration);
}

void soundArrival() {
  tone(buzzerPin, soundArrivalFrequency, soundArrivalDuration);
}
