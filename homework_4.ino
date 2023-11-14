const int latchPin = 11; // Connects to STCP (latch pin) on the shift register
const int clockPin = 10; // Connects to SHCP (clock pin) on the shift register
const int dataPin = 12;  // Connects to DS (data pin) on the shift register

const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

int displayDigits[] = {segD1, segD2, segD3, segD4};
const int displayCount = 4;

byte byteEncodings[] = {
    B11111100, // 0
    B01100000, // 1
    B11011010, // 2
    B11110010, // 3
    B01100110, // 4
    B10110110, // 5
    B10111110, // 6
    B11100000, // 7
    B11111110, // 8
    B11110110, // 9
    B00000000, // Blank
};

const int startStopPin = 2; // Change this to the pin where your button is connected
const int resetButtonPin = 3;
const int lapButtonPin = 8;

unsigned long lastButtonPressTime = 0;
bool resetButtonState = HIGH;
bool lastResetButtonState = HIGH;
unsigned long lastResetPressTime = 0;

bool resetButtonDebounced = true;
unsigned long debounceDelay = 50;
const int millisecondsInSecond = 1000;
const int tenthsInSecond = 100;
const int tenValue = 10;
const int oneHundredValue = 100;

bool lapButtonState = HIGH;
bool lastLapButtonState = HIGH;
unsigned long lastLapPressTime = 0;
unsigned int lapTimes[4] = {0}; // Array to store lap times
int currentLapIndex = 0;
bool lapButtonDebounced = true;

void setup() {
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);

    for (int i = 0; i < displayCount; i++) {
        pinMode(displayDigits[i], OUTPUT);
        digitalWrite(displayDigits[i], LOW);
    }

    pinMode(startStopPin, INPUT_PULLUP);
    pinMode(resetButtonPin, INPUT_PULLUP);
    pinMode(lapButtonPin, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop() {
    static unsigned long startTime = 0;
    unsigned long currentTime = millis();
    static unsigned long tenths = 0;
    static unsigned long seconds = 0;
    static bool running = false;
    static bool buttonState = HIGH;
    static bool lastButtonState = HIGH;

    buttonState = digitalRead(startStopPin);
    handleStartStopButton(startTime, lastButtonPressTime, running, buttonState, lastButtonState, debounceDelay, tenths, seconds);
    lastButtonState = buttonState;

    resetButtonState = digitalRead(resetButtonPin);
    handleResetButton(lastResetPressTime, resetButtonDebounced, resetButtonState, lastResetButtonState, debounceDelay, running, tenths, seconds);
    lastResetButtonState = resetButtonState;

    // lapButtonState = digitalRead(lapButtonPin);
    // handleLapButton(lastLapPressTime, lapButtonDebounced, lapButtonState, running, seconds, tenths, lapTimes, currentLapIndex);

    if (running) {
        unsigned long elapsedTime = currentTime - startTime;
        seconds = elapsedTime / millisecondsInSecond;
        tenths = (elapsedTime % millisecondsInSecond) / tenthsInSecond;
    }
    // Display the time on the 7-segment display

    int displayValues[] = {
        seconds / oneHundredValue,
        (seconds / tenValue) % tenValue,
        seconds % tenValue,
        tenths};
    displayTimeOnSegment(displayValues, displayCount);
}

void handleStartStopButton(unsigned long& startTime, unsigned long& lastButtonPressTime, bool& running, bool buttonState, bool lastButtonState, unsigned long debounceDelay, unsigned long& tenths, unsigned long& seconds) {
    if (buttonState == LOW && lastButtonState == HIGH && millis() - lastButtonPressTime > debounceDelay) {
        if (running) {
            running = false; // Stop the stopwatch
        } else {
            // Start or resume the stopwatch
            startTime = millis() - (seconds * millisecondsInSecond + tenths * tenthsInSecond);
            running = true;
        }
        lastButtonPressTime = millis();
    }
}

void handleResetButton(unsigned long& lastResetPressTime, bool& resetButtonDebounced, bool resetButtonState, bool lastResetButtonState, unsigned long debounceDelay, bool& running, unsigned long& tenths, unsigned long& seconds) {
    unsigned long currentMillis = millis();
    if (resetButtonState == LOW && lastResetButtonState == HIGH && currentMillis - lastResetPressTime > debounceDelay) {
        if (!running) {
            // Reset the timer if in pause mode
            seconds = 0;
            tenths = 0;
        }
        resetButtonDebounced = false;
        lastResetPressTime = currentMillis; // Record the time of the button press
    } else if (resetButtonState == HIGH) {
        resetButtonDebounced = true;
    }
}
//lapButton is not working
/* //Handles the lap button press, recording lap times when the stopwatch is running.
 
void handleLapButton(unsigned long& lastLapPressTime, bool& lapButtonDebounced, bool lapButtonState, bool& running, unsigned long& seconds, unsigned long& tenths, unsigned int lapTimes[], int& currentLapIndex) {
    unsigned long currentTime = millis();

    if (lapButtonState == LOW && lastLapButtonState == HIGH && lapButtonDebounced && currentTime - lastLapPressTime > debounceDelay) {

        if (running) {
            // Save lap time
            if (currentLapIndex < 4) {
                lapTimes[currentLapIndex] = seconds * 1000 + tenths * 100;
                currentLapIndex++;
            }
        }

        lapButtonDebounced = false;
        lastLapPressTime = currentTime; // Record the time of the button press
    } else if (lapButtonState == HIGH) {
        lapButtonDebounced = true;
    }
    lastLapButtonState = lapButtonState;
}
*/

void displayTimeOnSegment(int displayValues[], int displayCount) {
    byte dpDigit = byteEncodings[displayValues[displayCount - 2]] | B00000001;
    for (int i = displayCount - 1; i >= 0; i--) {
        activateDisplay(i);
        writeReg((i == displayCount - 2) ? dpDigit : byteEncodings[displayValues[i]]);
        writeReg(B00000000);
    }
}

// Function to write data to the shift register
void writeReg(int digit) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, digit);
    digitalWrite(latchPin, HIGH);
}

// Function to activate a specific display digit
void activateDisplay(int displayNumber) {
    for (int i = 0; i < displayCount; i++) {
        digitalWrite(displayDigits[i], HIGH);
    }
    digitalWrite(displayDigits[displayNumber], LOW);
}
