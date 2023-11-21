#include <EEPROM.h>

// Define pin numbers for sensors and RGB LED
const int ultrasonicTrigPin = 9;
const int ultrasonicEchoPin = 10;
const int ldrPin = A0;
const int redPin = 3;
const int greenPin = 5;
const int bluePin = 6; 

// Variables for sensor settings
int samplingInterval = 5; 
//is a minimum value , what is lower will trigger the alarm
int ultrasonicAlertThreshold = 10;     // Default sampling interval in seconds
//is a minimum value , what is lower will trigger the alarm
int ldrAlertThreshold = 500;        // Default threshold for LDR sensor

// Variables for RGB LED control
int redValue = 255;
int greenValue = 255;
int blueValue = 255;
bool automaticMode = false;
int zeroValue = 0;
const int maxLoggedDataSize = 10;
int loggedData[maxLoggedDataSize][2];  // Each entry is a pair of ultrasonic and LDR values
int loggedDataIndex = 0;
const int millisToSeconds = 1000;
const int minUltrasonicThreshold = 0;
const int maxUltrasonicThreshold = 255;
const int minSamplingValue = 1;
const int maxSamplingValue = 10;
const int minLdrValue = 0;
const int maxLdrValue = 1023;

// Initialize menu state
enum MenuState {
  MAIN_MENU,
  SENSOR_SETTINGS,
  SENSOR_SAMPLING_INTERVAL,
  ULTRASONIC_ALERT_THRESHOLD,
  LDR_ALERT_THRESHOLD,
  BACK_TO_SENSOR_SETTINGS,
  RESET_LOGGER_CONFIRM,
  SYSTEM_STATUS,
  CURRENT_SENSOR_READINGS,
  CURRENT_SENSOR_SETTINGS,
  DISPLAY_LOGGED_DATA,
  BACK_TO_MAIN_MENU,
  RGB_LED_CONTROL,
  MANUAL_COLOR_CONTROL,
  TOGGLE_AUTOMATIC_MODE,
  BACK_TO_MAIN_MENU_RGB_LED
};

MenuState currentMenu = MAIN_MENU;

void setup() {
 
  pinMode(ultrasonicTrigPin, OUTPUT); 
  pinMode(ultrasonicEchoPin, INPUT); 
   for (int i = 0; i < maxLoggedDataSize; ++i) {
    loggedData[i][0] = -1;  // initial value for ultrasonic
    loggedData[i][1] = -1;  // initial value for LDR
  }
  Serial.begin(9600);
  EEPROM.get(0, samplingInterval);
  printMainMenu();
}

void loop() {
  handleUserInput();
}

int readUltrasonicSensor() {
  // Trigger the ultrasonic sensor
  digitalWrite(ultrasonicTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonicTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicTrigPin, LOW);
  // Read the time it takes for the pulse to travel
  long duration = pulseIn(ultrasonicEchoPin, HIGH);
  float distance = duration * 0.034 / 2;
  //Serial.print("Distance:");
  //Serial.println(distance);
  return distance;
}

void handleUserInput() {
  if (Serial.available() > 0) {
    int userInput = Serial.parseInt();
    Serial.read();
    switch (currentMenu) {
      case MAIN_MENU:
        handleMainMenu(userInput);
        break;
      case SENSOR_SETTINGS:
        handleSensorSettings(userInput);
        break;
      case SENSOR_SAMPLING_INTERVAL:
        handleSensorSamplingInterval(userInput);
        break;
      case ULTRASONIC_ALERT_THRESHOLD:
        handleUltrasonicAlertThreshold(userInput);
        break;
      case LDR_ALERT_THRESHOLD:
        handleLdrAlertThreshold(userInput);
        break;
      case BACK_TO_SENSOR_SETTINGS:
        currentMenu = SENSOR_SETTINGS;
        printSensorSettingsMenu();
        break;
      case RESET_LOGGER_CONFIRM:
        handleResetLoggerConfirm(userInput);
        break;
      case SYSTEM_STATUS:
        handleSystemStatus(userInput);
        break;
      case CURRENT_SENSOR_READINGS:
        if(userInput == -1) {
          Serial.println("Stopped current sensor readings.");
          currentMenu = SYSTEM_STATUS; 
          printSystemStatusMenu();
        }
        break;
      case CURRENT_SENSOR_SETTINGS:
        // Implement logic for displaying current sensor settings
        if (userInput == -1) {
          Serial.println("Returning to System Status.");
          currentMenu = SYSTEM_STATUS;
          printSystemStatusMenu();
        }
        break;
      case DISPLAY_LOGGED_DATA:
        if (userInput == -1) {
          Serial.println("Returning to System Status.");
          currentMenu = SYSTEM_STATUS;
          printSystemStatusMenu();
        }
        break;
      case BACK_TO_MAIN_MENU:
        currentMenu = MAIN_MENU;
        printMainMenu();
        break;
      case RGB_LED_CONTROL:
        handleRgbLedControl(userInput);
        break;
      case MANUAL_COLOR_CONTROL:
        break;
      case TOGGLE_AUTOMATIC_MODE:
        if(userInput == -1) {
          Serial.println("Stopped current sensor readings.");
          currentMenu = RGB_LED_CONTROL; 
          printRgbLedControlMenu();
        }
        break;

      case BACK_TO_MAIN_MENU_RGB_LED:
        currentMenu = MAIN_MENU;
        printMainMenu();
        break;
      default:
        Serial.println("Invalid state. Restarting...");
        currentMenu = MAIN_MENU;
        printMainMenu();
        break;
    }
  }
}

void handleMainMenu(int userInput) {
  switch (userInput) {
    case 1:
      currentMenu = SENSOR_SETTINGS;
      printSensorSettingsMenu();
      break;
    case 2:
      currentMenu = RESET_LOGGER_CONFIRM;
      Serial.println("Reset Logger Data:");
      Serial.println("Are you sure? (1. Yes / 2. No)");
      break;
    case 3:
      currentMenu = SYSTEM_STATUS;
      printSystemStatusMenu();
      break;
    case 4:
      currentMenu = RGB_LED_CONTROL;
      printRgbLedControlMenu();
      break;
    default:
      Serial.println("Invalid input. Please try again.");
      printMainMenu();
      break;
  }
}

void handleSensorSettings(int userInput) {
  switch (userInput) {
    case 1:
      currentMenu = SENSOR_SAMPLING_INTERVAL;
      Serial.println("1. Sensor Sampling Interval");
      Serial.println("   Enter value between 1 and 10 seconds:");
      //handleSensorSamplingInterval(userInput);
      break;
    case 2:
      currentMenu = ULTRASONIC_ALERT_THRESHOLD;
      Serial.println("2. Ultrasonic Alert Threshold");
      Serial.println("   Enter threshold value for ultrasonic sensor:");
      break;
    case 3:
      currentMenu = LDR_ALERT_THRESHOLD;
      Serial.println("3. LDR Alert Threshold");
      Serial.println("   Enter threshold value for LDR sensor:");
      break;
    case 4:
      currentMenu = MAIN_MENU;
      printMainMenu();
      break;
    default:
      Serial.println("Invalid input. Please try again.");
      printSensorSettingsMenu();
      break;
  }
}

void handleSensorSamplingInterval(int userInput) {
  
  if (userInput >= minSamplingValue && userInput <= maxSamplingValue) {
    samplingInterval = userInput;
    EEPROM.put(0, samplingInterval);
    currentMenu = SENSOR_SETTINGS;
    Serial.println("Sensor Sampling Interval set successfully.");
    Serial.print("Current Sampling Interval: ");
    Serial.println(samplingInterval);
      // Replace with your actual function to read the sensor value

  
    printMainMenu();
    currentMenu = MAIN_MENU;
    // You can perform actions with the new sampling interval here
  } else {
    Serial.println("Invalid input. Please enter a value between 1 and 10 seconds.");
  }
}

void handleUltrasonicAlertThreshold(int userInput) {
  int ultrasonicSensorValue = readUltrasonicSensor();
  if (userInput >= minUltrasonicThreshold && userInput <= maxUltrasonicThreshold) {
    ultrasonicAlertThreshold = userInput;
    currentMenu = SENSOR_SETTINGS;
    Serial.print("Current Ultrasonic Alert Threshold: ");
    Serial.println(ultrasonicAlertThreshold);
    if (ultrasonicSensorValue < ultrasonicAlertThreshold) {
    Serial.println("ALERT: Object too close!");
  }
    printMainMenu();
    currentMenu = MAIN_MENU;

    // Additional logic to trigger an alert if needed
  } else {
    Serial.println("Invalid input. Please enter a value between 0 and 255.");
  }
}

void handleLdrAlertThreshold(int userInput) {
  int ldrSensorValue = analogRead(ldrPin);
  //ldrSensorValue = 1023 - ldrSensorValue;
  //Serial.print("lumina:");
  //Serial.println(ldrSensorValue);
  if (userInput >= minLdrValue && userInput <= maxLdrValue) {
    ldrAlertThreshold = userInput;
    currentMenu = SENSOR_SETTINGS;
    Serial.print("Current LDR Alert Threshold: ");
    Serial.println(ldrAlertThreshold);

    if (ldrSensorValue < ldrAlertThreshold) {
      Serial.println("ALERT: Low light detected!");
    }

    printMainMenu();
    currentMenu = MAIN_MENU;

    // Additional logic to trigger an alert if needed
  } else {
    Serial.println("Invalid input. Please enter a value between 0 and 1023.");
  }
}

void handleResetLoggerConfirm(int userInput) {
  switch (userInput) {
    case 1:
      Serial.println("User chose to reset logger data.");
      handleResetLoggerYes(userInput);
      break;
    case 2:
      Serial.println("User chose not to reset logger data.");
      handleResetLoggerNo(userInput);
      break;
    default:
      Serial.println("Invalid input. Please try again.");
      break;
  }
}

void handleResetLoggerYes(int userInput) {
  for (int i = 0; i < maxLoggedDataSize; ++i) {
    loggedData[i][0] = -1;  // initial value for ultrasonic
    loggedData[i][1] = -1;  // initial value for LDR
  }
  ultrasonicAlertThreshold = 10;  // initial value for ultrasonic threshold
  ldrAlertThreshold = 500;  // initial value for LDR threshold
  samplingInterval = 5;  //initial value for sampling
  currentMenu = MAIN_MENU;
  Serial.println("Logger data reset successfully.");
  printMainMenu();
}

void handleResetLoggerNo(int userInput) {
  currentMenu = MAIN_MENU;
  Serial.println("Reset canceled. Back to Main Menu...");
  printMainMenu();
}

void handleSystemStatus(int userInput) {
  switch (userInput) {
    case 1:
      currentMenu = CURRENT_SENSOR_READINGS;
      Serial.println("1. Current Sensor Readings");
      Serial.println("   Press any key to stop readings.");
      printCurrentSensorReadings();
      break;
    case 2:
      currentMenu = CURRENT_SENSOR_SETTINGS;
      Serial.println("2. Current Sensor Settings");
      printCurrentSensorSettings();
      break;
    case 3:
      currentMenu = DISPLAY_LOGGED_DATA;
      Serial.println("3. Display Logged Data");
      printLoggedData();  
      break;
    case 4:
      currentMenu = MAIN_MENU;
      printMainMenu();
      break;
    default:
      Serial.println("Invalid input. Please try again.");
      printSystemStatusMenu();
      break;
  }
}
void printCurrentSensorReadings() {
  unsigned long previousMillis = 0;
  const long interval = samplingInterval * millisToSeconds;  // seconds

  Serial.println("Press any key to stop readings.");

  while (currentMenu == CURRENT_SENSOR_READINGS) {
    unsigned long currentMillis = millis();

    // Afiseaza valorile senzorilor la fiecare interval
    if (currentMillis - previousMillis >= interval) {
      int ultrasonicValue = readUltrasonicSensor();
      int ldrValue = analogRead(ldrPin);

      loggedData[loggedDataIndex][0] = ultrasonicValue;
      loggedData[loggedDataIndex][1] = ldrValue;

      Serial.print("Ultrasonic Sensor Value: ");
      Serial.println(ultrasonicValue);
      Serial.print("LDR Sensor Value: ");
      Serial.println(ldrValue);
      loggedDataIndex = (loggedDataIndex + 1)  % maxLoggedDataSize;
      
      previousMillis = currentMillis;
    }

     if (Serial.available() > 0) {
      char inputChar = Serial.read(); // Citim caracterul
      if (inputChar == '\n' || inputChar == '\r') {
        Serial.println("Stopped current sensor readings.");
        currentMenu = SYSTEM_STATUS; 
        printSystemStatusMenu();
      }
    }
  }
}
void printCurrentSensorSettings() {
  Serial.println("Current Sensor Settings:");
  Serial.print("   Sampling Interval: ");
  Serial.print(samplingInterval);
  Serial.println(" seconds");

  Serial.print("   Ultrasonic Alert Threshold: ");
  Serial.println(ultrasonicAlertThreshold);

  Serial.print("   LDR Alert Threshold: ");
  Serial.println(ldrAlertThreshold);

  Serial.println("Press any key to return to System Status.");
  
  // Așteaptă până când se primește un caracter pentru a reveni la System Status
  while (currentMenu == CURRENT_SENSOR_SETTINGS) {
    if (Serial.available() > 0) {
      char inputChar = Serial.read();
      if (inputChar == '\n' || inputChar == '\r') {
        currentMenu = SYSTEM_STATUS;
        printSystemStatusMenu();
      }
    }
  }
}
// Function to print logged sensor data
void printLoggedData() {
  Serial.println("Last 10 Sensor Readings:");
  int startIndex = maxLoggedDataSize - 10;
  if (startIndex < zeroValue) {
    startIndex = 0;
  }
  if (loggedDataIndex <= maxLoggedDataSize) {
      for (int i = startIndex; i < maxLoggedDataSize; ++i) {
        Serial.print("Reading ");
        Serial.print(i - startIndex + 1);
        Serial.print(": Ultrasonic = ");
        Serial.print(loggedData[i][0]);
        Serial.print(" LDR = ");
        Serial.println(loggedData[i][1]);
  }
  }
  if (loggedDataIndex > maxLoggedDataSize){
      for (int i = startIndex; i < maxLoggedDataSize; ++i) {
        int dataIndex = (loggedDataIndex + i) % maxLoggedDataSize;
        Serial.print("Reading ");
        Serial.print(i + 1);
        Serial.print(": Ultrasonic = ");
        Serial.print(loggedData[dataIndex][0]);
        Serial.print(" LDR = ");
        Serial.println(loggedData[dataIndex][1]);
      }
  }
  Serial.println("Introduce -1 to return to System Status.");
}

void handleRgbLedControl(int userInput) {
  switch (userInput) {
    case 1:
      //manual color control doesn't work 
      currentMenu = MANUAL_COLOR_CONTROL;
      Serial.println("1. Manual Color Control");
      Serial.println("   Set RGB colors manually (e.g., R255 G0 B0):");
     /*  while (currentMenu == MANUAL_COLOR_CONTROL) {
        if (Serial.available() > 0) {
          String inputString = Serial.readStringUntil('\n');
          //handleManualColorControl(inputString);
          currentMenu = MAIN_MENU;
          printMainMenu();
        }
      }*/
      break;
    case 2:
      currentMenu = TOGGLE_AUTOMATIC_MODE;
      Serial.println("2. LED: Toggle Automatic ON/OFF");
      printAutomaticModeSubMenu();
      break;
    case 3:
      currentMenu = MAIN_MENU;
      printMainMenu();
      break;

    default:
      Serial.println("Invalid input. Please try again.");
      printRgbLedControlMenu();
      break;
  }
}
//manual color control doesn't work 
/* 
void handleManualColorControl(String& inputString) {

    if (inputString.startsWith("R") && inputString.length() == 7) {
        int r = inputString.substring(1, 4).toInt();
        int g = inputString.substring(4, 7).toInt();
        int b = inputString.substring(7).toInt();
        setRgbColors(r, g, b);
        Serial.println("RGB LED colors set successfully.");
        currentMenu = MAIN_MENU;
      } else {
        Serial.println("Invalid input format. Please use the format R255G0B0.");
        printRgbLedControlMenu();
      }
}
*/
// Function to set RGB LED colors manually
void setRgbColors(int redIntensity, int greenIntensity, int blueIntensity) {
    analogWrite(redPin, redIntensity);
    analogWrite(greenPin, greenIntensity);
    analogWrite(bluePin, blueIntensity);
}

void printAutomaticModeSubMenu() {
  Serial.println("Automatic Mode:");
  Serial.println("   1. ON");
  Serial.println("   2. OFF");
  bool inputReceived = false;// flag to control the loop
  //while is the only solution I have come across.
  while (!inputReceived) {
    if (Serial.available() > 0) {
      int userInput = Serial.parseInt();
      switch (userInput) {
        case 1:
          automaticMode = true;
          updateAutomaticModeLed(); 
          inputReceived = true;
          break;
        case 2: 
          automaticMode = false;
          updateAutomaticModeLed(); 
          inputReceived = true;
          break;
        default:
          Serial.println("Invalid input. Please try again.");
          printAutomaticModeSubMenu();
          break;
      }
    }
  }
  //currentMenu = RGB_LED_CONTROL;
  //printRgbLedControlMenu();
}

void updateAutomaticModeLed() {
  // Adaugă aici logica pentru a actualiza culoarea LED-ului în funcție de modul automat (ON sau OFF)
  while (automaticMode) {
    // Mod automat activat
    // Verifică valorile senzorilor și setează culoarea LED-ului în consecință
    int ultrasonicSensorValue = readUltrasonicSensor();
    int ldrSensorValue = analogRead(ldrPin);
//If either of the two conditions is met, the alarm is triggered.
    if (ultrasonicSensorValue < ultrasonicAlertThreshold || ldrSensorValue < ldrAlertThreshold) {
      // the led is red when there is an alert 
      setRgbColors(redValue, zeroValue, zeroValue);
    } else {
      // the led is green when ther is no alert
      setRgbColors(zeroValue, greenValue, zeroValue);
    }
    delay(500);
  } 
  if(!automaticMode){
    setRgbColors(redValue,greenValue,blueValue);
    Serial.println("Automatic Mode is OFF. Restoring RGB LED colors.");
  }
  currentMenu = RGB_LED_CONTROL;
  printRgbLedControlMenu();
}
 
void printMainMenu() {
  Serial.println("\nMenu start:");
  Serial.println("   1. Sensor Settings");
  Serial.println("   2. Reset logger data");
  Serial.println("   3. System status");
  Serial.println("   4. RGB Led Control");
}

void printSensorSettingsMenu() {
  Serial.println("Sensor Settings:");
  Serial.println("   1. Sensor Sampling Interval");
  Serial.println("   2. Ultrasonic Alert Threshold");
  Serial.println("   3. LDR Alert Threshold");
  Serial.println("   4. Back");
}

void printSystemStatusMenu() {
  Serial.println("System Status:");
  Serial.println("   1. Current Sensor Readings");
  Serial.println("   2. Current Sensor Settings");
  Serial.println("   3. Display Logged Data");
  Serial.println("   4. Back");
}

void printRgbLedControlMenu() {
  Serial.println("RGB Led Control:");
  Serial.println("   1. Manual Color Control");
  Serial.println("   2. LED: Toggle Automatic ON/OFF");
  Serial.println("   3. Back");
}