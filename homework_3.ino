// Define the pin numbers for various components
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;
const int pinX = A0;
const int pinY = A1;
const int swPin = 2;
const int numSegments = 8; 
byte segmentState = HIGH; // Initial segment state

int xJoyValue = 0;
int yJoyValue = 0;
byte swState = HIGH;
byte lastSwState = HIGH;
long lastSwitchTime = 0;
long lastSwitchTimeLong = 0;
int debounceInterval = 100;
int longDebounceInterval = 1000;
byte currentSwitchReading = LOW;
byte lastSwitchReading = LOW;
bool joystickReading = false;
bool lastJoystickReading = false;
byte longPressState = LOW;

// Pin assignments for the 7-segment display
const int segmentPins[numSegments] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

byte segmentStates[numSegments] = {
  LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
};
const int lowerThreshold = 480;
const int upperThreshold = 580;
bool hasJoystickMoved = false;
bool currentState = true;
int currentSegmentIndex = numSegments - 1;
int lastSegmentIndex = numSegments - 1;

unsigned long blinkInterval = 200;
unsigned long lastBlinkTime = 0;
const int restartTime = 2000;
unsigned long restartTimer = 2000;
const int numDirections = 4;

// Defines the relationship between segments and their neighboring segments for joystick control
int segmentNeighbors[numSegments][numDirections] = {
  // UP DOWN LEFT RIGHT (-1 - None)
  -1, 6, 5, 1,   // LED a
  0, 6, 5, -1,   // LED b
  6, 3, 4, 7,   // LED c
  6, -1, 4, 2,   // LED d
  6, 3, -1, 2,   // LED e
  0, 6, -1, 1,   // LED f
  0, 3, -1, -1,  // LED g
  -1, -1, 2, -1  // LED dp
};
const int directionUp = 0;
const int directionDown = 1;
const int directionLeft = 2;
const int directionRight = 3;
int neighborIndex = 0;

void setup() {
  pinMode(swPin, INPUT_PULLUP); 
  for (int i = 0; i < numSegments; ++i) {
    pinMode(segmentPins[i], OUTPUT); // 
  }
}

void loop() {
  readSwitchState(); 
  blinkSegment(); 
  handleJoystick(); 
}

// Function to read and debounce the state
void readSwitchState() {
  currentSwitchReading = digitalRead(swPin);

  if (currentSwitchReading != lastSwitchReading) {
    lastSwitchTime = millis();
  }

  // Handle long press and normal press events
  if (millis() - lastSwitchTime > longDebounceInterval) {
    if (currentSwitchReading != longPressState) {
      handleLongPress(currentSwitchReading);
    }
  } else if (millis() - lastSwitchTime > debounceInterval) {
    handleNormalPress(currentSwitchReading);
  }
  lastSwitchReading = currentSwitchReading;
}

// Function to handle a long press 
void handleLongPress(byte newSwitchState) {
  longPressState = newSwitchState;
  if (longPressState == LOW) {
    resetSegmentStates(); // Reset segment states
  }
}

// Function to handle a normal press 
void handleNormalPress(byte newSwitchState) {
  swState = newSwitchState;
  if (swState == LOW) {
    currentState = !currentState; // Toggle the current state
  }
}

// Function to blink the current segment
void blinkSegment() {
  if (millis() - lastBlinkTime >= blinkInterval) {
    lastBlinkTime = millis();
    segmentState = !segmentState;
    digitalWrite(segmentPins[currentSegmentIndex], segmentState);
  }
}

void handleJoystick() {
  xJoyValue = analogRead(pinX);
  yJoyValue = analogRead(pinY);
   
  // Handle joystick button press
  if (swState == 0 && swState != lastSwState) { 
    segmentStates[currentSegmentIndex] = !segmentStates[currentSegmentIndex];
    restartTimer = millis();
  }
  lastSwState = swState;

  if (!hasJoystickMoved) {
    int direction = getJoystickDirection(xJoyValue, yJoyValue);   
    if (direction != -1) {
      moveToNeighbor(currentSegmentIndex, direction);
      hasJoystickMoved = true;
    }
  }
  
  if (xJoyValue > lowerThreshold && xJoyValue < upperThreshold && yJoyValue > lowerThreshold && yJoyValue < upperThreshold) {
    hasJoystickMoved = false;
  }
}

// Function to determine the joystick direction
int getJoystickDirection(int xValue, int yValue) {
  if (xValue > upperThreshold) {
    return directionRight;//move right
  } else if (xValue < lowerThreshold) {
    return directionLeft;//move left
  } else if (yValue > upperThreshold) {
    return directionDown;//move down
  } else if (yValue < lowerThreshold) {
    return directionUp;//move up
  } else {
    return -1;
  }
}

// Function to move to a neighboring segment based on joystick input
void moveToNeighbor(int &currentSegmentIndex, int direction) {
  int neighborIndex = segmentNeighbors[currentSegmentIndex][direction];
  if (neighborIndex != -1){// -1 means wall 
    int lastSegmentIndex = currentSegmentIndex; 
    currentSegmentIndex = neighborIndex;
    digitalWrite(segmentPins[lastSegmentIndex], segmentStates[lastSegmentIndex]);
  }
}

// Function to reset all segment states
void resetSegmentStates() {
  for (int i = 0; i < numSegments; ++i) {
    digitalWrite(segmentPins[i], LOW);
    segmentStates[i] = LOW;
  }
  currentSegmentIndex = numSegments - 1;
  currentState = true;
}
