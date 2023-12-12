#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>

#define UP_ARROW      byte(0)
#define DOWN_ARROW    byte(1)
#define BACK_ARROW    byte(2)
#define INCREASE_ARROW      byte(3)
#define DECREASE_ARROW      byte(4)
#define SAD_FACE      byte(5)
#define HEART         byte(6)
#define BLANK         (char) 254

#define MAIN_MENU       0
#define START_MENU      1
#define SETTINGS_MENU   2
#define ABOUT_MENU 3
#define LCD_BRIGHTNESS_SUBMENU 5
#define MATRIX_BRIGHTNESS_SUBMENU 6
#define BACK_SETTINGS_SUBMENU 4

#define BACK_SELECTED 0
#define RESTART_SELECTED 1

#define UPPER_MAIN_MENU 0
#define LOWER_MAIN_MENU 1

#define UPPER_SETTINGS_MENU 0
#define LOWER_SETTINGS_MENU 1

#define ITEM_START 0
#define ITEM_SETTINGS 1
#define ITEM_ABOUT 2

#define ITEM_SETTINGS_LCD 1
#define ITEM_SETTINGS_MATRIX 2
#define ITEM_SETTINGS_BACK 0

int menuState = MAIN_MENU;
int mainMenuState = UPPER_MAIN_MENU; 
int settingsSubMenuState = UPPER_SETTINGS_MENU;
//int endMenuSelection = BACK_SELECTED;
int mainMenuSelectedValue = ITEM_START;
int settingsSelectedMenu = ITEM_SETTINGS_LCD;
const int lcdBrightnessPin = 3;
const int lcdBrightnessEEPROMAddress = 0;
const int matrixBrightnessEEPROMAddress = 1;
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
//const byte LCDbrightnessPin = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

const int numRows = 8;
const int numCols = 8;

const int xPin = A0;
const int yPin = A1;
const int swPin = 2;

const int lowerThreshold = 300;
const int upperThreshold = 600;

const int playerBlinkRate = 550;
const int bombBlinkRate = 250; 
const int bombExplosionTime = 3000; 
const int explosionRange = 1; 
bool bombIsActive = false;
bool playerIsDead = false;
bool allWallsDestroyed = false;
bool deathAnimationActive = false;
bool winAnimationActive = false;
const int redLedPin = 13; 
int gameBoard[numRows][numCols];
int playerRow = 0;
int playerCol = 0;
bool playerBlinkState = false;
bool mapGenerated = false;

const int animationDelay = 100;
const int maxRandomValue = 100;
const int percentageWall = 5;
const int twoNumber = 2;

const int freeSpaces = 2;
int xValue = 0;
int yValue = 0;
int yPressed = 0;
int swValue;
int currentSWValue = 0;
int previousSWValue = 0;
const unsigned long introDuration = 4000;
bool introMessage = false;
bool showSettingsImage = false;
int lcdBrightnessValue;
int matrixBrightnessValue;
int playerLives = 3;
bool gameOver = false;
struct Bomb {
  int row;
  int col;
  bool blinkState;
  unsigned long explosionTime;
};

const int maxBombs = 5;
Bomb bombs[maxBombs];

const int sizeOfElement = 8;
//for lose animation
const uint64_t loseImages[] = {
    0x0000000000000000,
    0x8142241818244281
};
//for win animation
const uint64_t winImages[] = {
    0x0000000000000000,
    0x00040a1120408000
};

const uint64_t bombImage[] = {
  0x3c7e7e7e7e3c1060
};
const uint64_t aboutImage[] = {
  0x3c4299999999423c
};

const uint64_t settingsImage[] = {
  0x03070e7cf8983830
};
/*
byte blockChar[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
const byte BLOCK_CHAR_CODE = 0;

byte restartArrow[] = {
  B00000,
  B01111,
  B01001,
  B11101,
  B01001,
  B00001,
  B00111,
  B00000
};
*/
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

byte backArrow[] = {
  B00010,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00010,
  B00000
};
byte increaseArrow[] = {
  B00000,
  B01111,
  B00011,
  B00101,
  B01001,
  B10000,
  B00000,
  B00000
};

byte decreaseArrow[] = {
  B00000,
  B00000,
  B10000,
  B01001,
  B00101,
  B00011,
  B01111,
  B00000
};
byte sadFace[] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B00000,
  B01110,
  B10001,
  B00000
};
byte heart[] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};
/*
byte skull[8] = {
  B01110,
  B11111,
  B10101,
  B11111,
  B01110,
  B01010,
  B00000,
};
*/

void setup() {
  
  lcd.begin(16, 2);
  
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);
  
  lcd.createChar(UP_ARROW, upArrow);
  lcd.home();
  lcd.createChar(DOWN_ARROW, downArrow);
  lcd.home();
  lcd.createChar(BACK_ARROW, backArrow);
  lcd.home();
  lcd.createChar(INCREASE_ARROW, increaseArrow);
  lcd.home();
  lcd.createChar(DECREASE_ARROW, decreaseArrow);
  lcd.home();
  lcd.createChar(SAD_FACE, sadFace);
  lcd.home();
  lcd.createChar(HEART, heart);
  lcd.home();
  
  //lcd.createChar(RESTART_ARROW, restartArrow);
  Serial.begin(9600);
  lcdBrightnessValue = EEPROM.read(lcdBrightnessEEPROMAddress);
  updateLcdBrightness();
  matrixBrightnessValue = EEPROM.read(matrixBrightnessEEPROMAddress);
  lc.setIntensity(0, matrixBrightnessValue);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(redLedPin, OUTPUT);
 
}

void loop() {
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  currentSWValue = !digitalRead(swPin);
 
  if(!introMessage){
    displayWelcomeMessage();
} else if (menuState == MAIN_MENU) {
    if (mainMenuState == UPPER_MAIN_MENU) {
      drawUpperMainMenu();
    } else if (mainMenuState == LOWER_MAIN_MENU) {
      drawLowerMainMenu();
    } 
    if(mainMenuSelectedValue == ITEM_START && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = START_MENU;
      lcd.clear();
  } else if (mainMenuSelectedValue == ITEM_SETTINGS && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = SETTINGS_MENU;
      lcd.clear();
  } else if (mainMenuSelectedValue == ITEM_ABOUT && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = ABOUT_MENU;
      lcd.clear();
  }
  previousSWValue = currentSWValue;
  
 }
  else if (menuState == START_MENU) {
      drawStartMenu();
      checkWinOrLoose();
    
 } else if (menuState == SETTINGS_MENU) {
      showSettingsImage = true;
      displaySettingsImage();
      if (settingsSubMenuState == UPPER_SETTINGS_MENU) {
        drawUpperSettingsMenu();
    } else if (settingsSubMenuState == LOWER_SETTINGS_MENU) {
        drawLowerSettingsMenu();
    } 
     if(settingsSelectedMenu == ITEM_SETTINGS_LCD && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = LCD_BRIGHTNESS_SUBMENU;
      //previousMillis = millis();
      //previousMillisFinishGame = millis();
      //currentLevel = startingLevelValue;
      lcd.clear();
  } else if (settingsSelectedMenu == ITEM_SETTINGS_MATRIX && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = MATRIX_BRIGHTNESS_SUBMENU;
      //previousMillisSettings = millis();
      lcd.clear();
  } else if (settingsSelectedMenu == ITEM_SETTINGS_BACK && currentSWValue == 1 && previousSWValue != currentSWValue && showSettingsImage == true) {
      showSettingsImage = false;
      displayClearBoard();
      menuState = BACK_SETTINGS_SUBMENU;
      lcd.clear();
    }
   
  previousSWValue = currentSWValue;

 } else if (menuState == ABOUT_MENU) {
     drawAboutMenu();
 } 
  else if (menuState == LCD_BRIGHTNESS_SUBMENU) {
    drawLCDBrightness();
 } else if (menuState == MATRIX_BRIGHTNESS_SUBMENU) {
    drawMatrixBrightness();
  
 }
  else if (menuState == BACK_SETTINGS_SUBMENU) {
    menuState = MAIN_MENU; 
    mainMenuState = LOWER_MAIN_MENU; 
    lcd.clear();
    previousSWValue = currentSWValue;  
 }
 
}

void drawStartMenu(){
   if (!mapGenerated) {
    generateStaticMap();
    mapGenerated = true;
  }
  //swValue= digitalRead(swPin);
   //Serial.print("SW Pin Value: ");
 // Serial.print(swValue);
  updatePlayerPosition(xValue, yValue);
  updatePlayerBlink();
  // Print joystick button state for debugging
  if (currentSWValue == 1 && previousSWValue == 0 && !playerIsDead) {
    Serial.println("Joystick Button Pressed");
    if (!bombIsActive) {
      placeBomb();
      bombIsActive = true;  // Set the flag to indicate an active bomb
    }
  }
  updateBombs();
  displayGameBoard();
  delay(100);
}

void checkWinOrLoose(){
  if (playerIsDead && !deathAnimationActive && playerLives == 0) {
    playAnimation(loseImages, sizeof(loseImages) / sizeOfElement);
    gameOver = true;
    deathAnimationActive = true;
    //displayClearBoard();
    lcd.setCursor(0, 0);
    lcd.print("You died!");
    lcd.setCursor(10, 0);
    lcd.write(SAD_FACE);
    lcd.setCursor(0, 1);
    lcd.print("Exit!");
    lcd.print(" <");
  }

  allWallsDestroyed = isMissionComplete();
  if (allWallsDestroyed && !playerIsDead && deathAnimationActive == false && winAnimationActive == false && playerLives >= 0) {
    playAnimation(winImages, sizeof(winImages) / sizeOfElement);
    gameOver = true;
    winAnimationActive = true;
    allWallsDestroyed = false;
    //displayClearBoard();
    lcd.setCursor(0, 0);
    lcd.print("Congratulations!");
    lcd.setCursor(0, 1);
    lcd.print("Exit!");
    lcd.print(" <");  
  }
  if (currentSWValue == 1 && previousSWValue != currentSWValue && gameOver == true) {
    menuState = MAIN_MENU;
    displayClearBoard();
    lcd.clear();
    resetParameters();
  }
    previousSWValue = currentSWValue;
}

void displayWelcomeMessage() {
  const char* line1 = "LET THE";
  const char* line2 = "EXPLOSIONS BEGIN";
  unsigned long startTime = 0;
  if(millis()- startTime < introDuration){
    //lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);  
    lcd.print(line2);
    displayBombImage();
  }
  else{
    lcd.clear();
    introMessage = true;
    displayClearBoard();
  }
}

void drawUpperMainMenu() {
  // title
  lcd.setCursor(3, 0);
  lcd.print("Game Menu!");

  // items
  lcd.setCursor(0, 1);
  lcd.print("Start game");
  if (mainMenuSelectedValue == ITEM_START)
    lcd.print(" <");
    
  // scroll arrow
  lcd.setCursor(15, 1);
  lcd.write(DOWN_ARROW);
  if (yValue > upperThreshold && !yPressed) {
    yPressed = 1;
    lcd.clear();
    mainMenuState = LOWER_MAIN_MENU;
    mainMenuSelectedValue = ITEM_SETTINGS;
  } else {
    yPressed = 0;
  }
}

void drawLowerMainMenu() {
  // items
  lcd.setCursor(0, 0);
  lcd.print("Settings");
  if (mainMenuSelectedValue == ITEM_SETTINGS) {
    lcd.print(" <");
  }
    
  lcd.setCursor(0, 1);
  lcd.print("About");
  if (mainMenuSelectedValue == ITEM_ABOUT) {
    lcd.print(" <");
  }
  // scroll arrow
  lcd.setCursor(15, 0);
  lcd.write(UP_ARROW);
  //lcd.setCursor(15, 1);
  //lcd.write(DOWN_ARROW);

  if (yValue < lowerThreshold && !yPressed) {
    yPressed = 1;
    
    if (mainMenuSelectedValue == ITEM_SETTINGS) {
      lcd.clear();
      mainMenuState = UPPER_MAIN_MENU;
      mainMenuSelectedValue = ITEM_START;
    } else if (mainMenuSelectedValue == ITEM_ABOUT) {
        lcd.setCursor(6, 1);
        lcd.print(BLANK);
        mainMenuSelectedValue = ITEM_SETTINGS;
    }
   }
   else if (yValue > upperThreshold && !yPressed) {
        yPressed = 1;
        lcd.setCursor(9, 0);
        lcd.print(BLANK);
        mainMenuSelectedValue = ITEM_ABOUT;
      }
    
    else if (yValue >= lowerThreshold && yValue <= upperThreshold) {
      yPressed = 0;
    }
}

void drawUpperSettingsMenu() {
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(BACK_ARROW);
  lcd.print("Back");
  
  if (settingsSelectedMenu == ITEM_SETTINGS_BACK) {
    lcd.print(" <");
  }

  lcd.setCursor(0, 1);
  lcd.print("LCD bright.");
  if (settingsSelectedMenu == ITEM_SETTINGS_LCD) {
    lcd.print(" <");
  }
  // scroll arrow
  lcd.setCursor(15, 1);
  lcd.write(DOWN_ARROW);

  if (yValue < 400 && !yPressed) {
    yPressed = 1;
    
    if (settingsSelectedMenu ==  ITEM_SETTINGS_LCD) {
      //lcd.clear();
      lcd.setCursor(12, 1);
      lcd.print(BLANK);
      settingsSelectedMenu  = ITEM_SETTINGS_BACK;
    } 
   } 
   else if (yValue > upperThreshold && !yPressed) {
      yPressed = 1;
      if (settingsSelectedMenu == ITEM_SETTINGS_LCD) {
          settingsSubMenuState = LOWER_SETTINGS_MENU;
          settingsSelectedMenu = ITEM_SETTINGS_MATRIX;
          lcd.clear();
        } else if (settingsSelectedMenu == ITEM_SETTINGS_BACK){
          lcd.setCursor(6, 0);
          lcd.print(BLANK);
          settingsSelectedMenu= ITEM_SETTINGS_LCD;
        }
  
  } else if (yValue >= lowerThreshold && yValue <= upperThreshold){
    yPressed = 0;
  } 
  
}
void drawLowerSettingsMenu() {
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Matrix bright.");
  if (settingsSelectedMenu == ITEM_SETTINGS_MATRIX) {
    lcd.print("<");
  }
    
  // scroll arrow
  lcd.setCursor(15, 0);
  lcd.write(UP_ARROW);

  if (yValue < lowerThreshold && !yPressed) {
    yPressed = 1;
    
    if (settingsSelectedMenu == ITEM_SETTINGS_MATRIX) {
      lcd.clear();
      settingsSubMenuState = UPPER_SETTINGS_MENU;
      settingsSelectedMenu = ITEM_SETTINGS_LCD;
    } 
   }
    else if (yValue >= lowerThreshold && yValue <= upperThreshold) {
      yPressed = 0;
    }
}

const int lowBrightness = 5;    // Define low brightness level
const int mediumBrightness = 10; // Define medium brightness level
const int highBrightness = 15; // Define high brightness level
  
const int minBrightness = 0;
const int maxBrightness = 15;
const int brightnessStep = 1;

void drawLCDBrightness() {
  /*lcd.setCursor(0, 0);
  lcd.write(BACK_ARROW);
  lcd.print("Back");*/
  lcd.setCursor(5, 0);
  lcd.print("Move:");

  lcd.setCursor(4, 1);
  lcd.print("UP");
  lcd.setCursor(6, 1);
  lcd.write(INCREASE_ARROW);
  lcd.setCursor(7, 1);
  lcd.print(" DOWN");
  lcd.setCursor(12, 1);
  lcd.write(DECREASE_ARROW);

  if (yValue < lowerThreshold && !yPressed) {
    yPressed = 1;
    lcdBrightnessValue = constrain(lcdBrightnessValue - brightnessStep, minBrightness, maxBrightness);
    updateLcdBrightness();
  } else if (yValue > upperThreshold && !yPressed) {
    yPressed = 1;
    lcdBrightnessValue = constrain(lcdBrightnessValue + brightnessStep, minBrightness, maxBrightness);
    updateLcdBrightness();
  } else if (yValue >= lowerThreshold && yValue <= upperThreshold) {
    yPressed = 0;
  }

  if (currentSWValue == 1 && previousSWValue != currentSWValue) {
    // Save the selected brightness level to EEPROM
    EEPROM.write(lcdBrightnessEEPROMAddress, lcdBrightnessValue);
    lcd.clear();
    menuState = SETTINGS_MENU;
    settingsSubMenuState = UPPER_SETTINGS_MENU;
    settingsSelectedMenu = ITEM_SETTINGS_LCD;
  }
  previousSWValue = currentSWValue;
}

void updateLcdBrightness() {
  // Setează nivelul de luminozitate în funcție de valoarea curentă
  if (lcdBrightnessValue <= lowBrightness) {
    analogWrite(lcdBrightnessPin, map(lcdBrightnessValue, 0, lowBrightness, 0, 255));
  } else if (lcdBrightnessValue <= mediumBrightness) {
    analogWrite(lcdBrightnessPin, map(lcdBrightnessValue, lowBrightness + 1, mediumBrightness, 30, 150));
  } else {
    analogWrite(lcdBrightnessPin, map(lcdBrightnessValue, mediumBrightness + 1, highBrightness, 150, 255));
  }
}

void drawMatrixBrightness() {
  /*lcd.setCursor(0, 0);
  lcd.write(BACK_ARROW);
  lcd.print("Back");
  */
  lcd.setCursor(5, 0);
  lcd.print("Move:");

  lcd.setCursor(4, 1);
  lcd.print("UP");
  lcd.setCursor(6, 1);
  lcd.write(INCREASE_ARROW);
  lcd.setCursor(7, 1);
  lcd.print(" DOWN");
  lcd.setCursor(12, 1);
  lcd.write(DECREASE_ARROW);

  // Adjust matrix brightness based on joystick input
  if (yValue > upperThreshold && !yPressed) {
    yPressed = 1;
    matrixBrightnessValue = constrain(matrixBrightnessValue - brightnessStep, minBrightness, maxBrightness);
    lc.setIntensity(0, matrixBrightnessValue);
  } else if (yValue < lowerThreshold&& !yPressed) {
    yPressed = 1;
    matrixBrightnessValue = constrain(matrixBrightnessValue + brightnessStep, minBrightness, maxBrightness);
    lc.setIntensity(0, matrixBrightnessValue);
  } else if (yValue >= lowerThreshold && yValue <= upperThreshold) {
    yPressed = 0;
  }

  // Save matrix brightness level to EEPROM when the joystick button is pressed
  if (currentSWValue == 1 && previousSWValue != currentSWValue) {
    EEPROM.write(matrixBrightnessEEPROMAddress, matrixBrightnessValue);
    lcd.clear();
    menuState = SETTINGS_MENU;
    settingsSubMenuState = LOWER_SETTINGS_MENU;
    settingsSelectedMenu = ITEM_SETTINGS_MATRIX;
  }

  previousSWValue = currentSWValue;
}

void drawAboutMenu() {
  const int delayOnScroll = 350;
  const int printCharacters = 30;
  static unsigned long lastScrollTime = 0;
  bool showAboutImage = false;
  /*lcd.setCursor(0, 0);
  lcd.write(BACK_ARROW);
  lcd.print("Back");
  lcd.print("<");*/
  lcd.setCursor(4,0);
  lcd.print("Title: Bomberman");

  lcd.setCursor(1, 1);
  lcd.print("Creator: Razvan, Git: CaruntuRazvan");
  if(!showAboutImage){
    displayAboutImage();//for matrix
    showAboutImage = true;
  }
    
  if (millis() - lastScrollTime >= delayOnScroll) {
    lcd.scrollDisplayLeft();
    lastScrollTime = millis();
  }

  if (currentSWValue == 1 && previousSWValue != currentSWValue && showAboutImage == true) {
    lcd.clear();
    menuState = MAIN_MENU;
    showAboutImage= false;
    displayClearBoard();
  }
  previousSWValue = currentSWValue;
}

// Function to check if the mission is complete
bool isMissionComplete() {
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      if (gameBoard[row][col] == 1 && !isIndestructibleWall(row, col)) {
        return false;
      }
    }
  }
  return true;
}

void generateStaticMap() {
  int staticMap[numRows][numCols] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 0, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 0, 0, 1},
    {1, 1, 0, 0, 1, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
  };

  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      gameBoard[row][col] = staticMap[row][col];
      lc.setLed(0, row, col, gameBoard[row][col]);
    }
  }

  playerRow = 1;
  playerCol = 1;
  lc.setLed(0, playerRow, playerCol, playerBlinkState);
}
/*
int countFreeSpacesAround(int row, int col) {
  int freeSpaces = 0;
  if (row > 0 && gameBoard[row - 1][col] == 0) {
    ++freeSpaces;
  }
  if (row < numRows - 1 && gameBoard[row + 1][col] == 0) {
    ++freeSpaces;
  }
  if (col > 0 && gameBoard[row][col - 1] == 0) {
    ++freeSpaces;
  }
  if (col < numCols - 1 && gameBoard[row][col + 1] == 0) {
    ++freeSpaces;
  }
  return freeSpaces;
}
*/
bool isIndestructibleWall(int row, int col) {
  return row == 0 || row == numRows - 1 || col == 0 || col == numCols - 1;
}

void updatePlayerPosition(int xValue, int yValue) {
  static int lastXValue = xValue;
  static int lastYValue = yValue;
  static unsigned long lastUpdateTime = millis();
  const int sensibilityThreshold = 20;
  
  if (millis() - lastUpdateTime >= sensibilityThreshold) {
    // Mișcă jucătorul pe baza valorilor joystick-ului
    if (xValue < lowerThreshold && playerCol > 0 && !gameBoard[playerRow][playerCol - 1]) {
      lc.setLed(0, playerRow, playerCol, 0);
      --playerCol;
    } else if (xValue > upperThreshold && playerCol < numCols - 1 && !gameBoard[playerRow][playerCol + 1]) {
      lc.setLed(0, playerRow, playerCol, 0);
      ++playerCol;
    }

    if (yValue < lowerThreshold && playerRow > 0 && !gameBoard[playerRow - 1][playerCol]) {
      lc.setLed(0, playerRow, playerCol, 0);
      --playerRow;
    } else if (yValue > upperThreshold && playerRow < numRows - 1 && !gameBoard[playerRow + 1][playerCol]) {
      lc.setLed(0, playerRow, playerCol, 0);
      ++playerRow;
    }
    
    lastXValue = xValue;
    lastYValue = yValue;
    lastUpdateTime = millis();
  }
}

void updatePlayerBlink() {
  static unsigned long lastBlinkTime = 0;
  if (millis() - lastBlinkTime >= playerBlinkRate) {
    playerBlinkState = !playerBlinkState;
    lastBlinkTime = millis();
  }
}

void placeBomb() {
  for (int i = 0; i < maxBombs; ++i) {
    if (bombs[i].explosionTime == 0) {
      bombs[i].row = playerRow;
      bombs[i].col = playerCol;
      bombs[i].blinkState = false;
      bombs[i].explosionTime = millis() + bombExplosionTime;
      lc.setLed(0, bombs[i].row, bombs[i].col, bombs[i].blinkState);
      break;
    }
  }
}

void updateBombs() {
  // Iterate through all active bombs
  for (int i = 0; i < maxBombs; ++i) {
    if (bombs[i].explosionTime > 0) {
      unsigned long lastBlinkTime = bombs[i].blinkState;

      // Check if it's time to toggle the bomb's blink state
      if (millis() - lastBlinkTime >= bombBlinkRate) {
        // Toggle the blink state
        bombs[i].blinkState = !bombs[i].blinkState;
        lc.setLed(0, bombs[i].row, bombs[i].col, bombs[i].blinkState);

        // Update the last blink time for the current bomb
        lastBlinkTime = millis();
      }

      // Check if it's time for the bomb to explode
      if (millis() >= bombs[i].explosionTime) {
        // Call the function to handle the bomb explosion
        explodeBomb(i);
      } else {
        // Toggle the red LED for a blinking effect during bomb countdown
        digitalWrite(redLedPin, millis() % (twoNumber * bombBlinkRate) < bombBlinkRate);
      }
    }
  }
}

void displayGameBoard() {
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      if (row == playerRow && col == playerCol) {
        lc.setLed(0, row, col, playerBlinkState);
      } else {
        lc.setLed(0, row, col, gameBoard[row][col] == 1);
      }
    }
  }
}

// Function to explode walls in a specific direction
void explodeInDirection(int row, int col, int rowChange, int colChange, int direction) {
  for (int i = 0; i < explosionRange; ++i) {
    row += rowChange;
    col += colChange;
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
      if (gameBoard[row][col] == 1 && !isIndestructibleWall(row, col)) {
        gameBoard[row][col] = 0;
        lc.setLed(0, row, col, 0);
      }
    } else {
      break;
    }
  }
}

// Function to explode walls in all directions
void explodeWalls(int index) {
  explodeInDirection(bombs[index].row, bombs[index].col, -1, 0, 0);  // Up
  explodeInDirection(bombs[index].row, bombs[index].col, 1, 0, 1);   // Down
  explodeInDirection(bombs[index].row, bombs[index].col, 0, -1, 2);  // Left
  explodeInDirection(bombs[index].row, bombs[index].col, 0, 1, 3);   // Right
}

// Function to check if the player is in the explosion range
bool isPlayerInExplosionRange(int index) {
  int rowDiff = abs(playerRow - bombs[index].row);
  int colDiff = abs(playerCol - bombs[index].col);
  return (rowDiff == 0 && colDiff <= explosionRange) || (colDiff == 0 && rowDiff <= explosionRange);
}

// Function to handle player death and clear the game board
void handlePlayerDeath() {
    lcd.setCursor(0, 0);
    lcd.print("Lives:");
    lcd.print(playerLives);
    lcd.write(HEART);

  if (playerLives > 0) {
      playerLives--;
      
  } else {
  
    playerRow = -1;
    playerCol = -1;
    playerIsDead = true;
    for (int row = 0; row < numRows; ++row) {
      for (int col = 0; col < numCols; ++col) {
        gameBoard[row][col] = 0;
        lc.setLed(0, row, col, 0);
      }
    }
  }
}

// Main explodeBomb function
void explodeBomb(int index) {
  explodeWalls(index);
  // Check if the player is in the explosion range
  if (isPlayerInExplosionRange(index)) {
    handlePlayerDeath();
  }

  digitalWrite(redLedPin, LOW);
  bombs[index].explosionTime = 0;
  bombIsActive = false; // Reset the bomb flag
}

void playAnimation(const uint64_t animationImages[], int imagesLen){
  const int animationSpeed = 500;  // 0.5 seconds in milliseconds
  const unsigned long animationDuration = 1500;  // 1.5 seconds in milliseconds
  const int numFrames = animationDuration / animationSpeed;
  for (int frame = 0; frame < numFrames; ++frame) {
    for (int i = 0; i < imagesLen; ++i) {
      for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
          int bit = (animationImages[i] & (1ULL << (col + row * numCols))) ? 1 : 0;
          lc.setLed(0, row, col, bit);
        }
      }
      delay(animationSpeed);
    }
  }
}

void displayBombImage() {
  // Iterate through the image data and set LEDs accordingly
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      int bit = (bombImage[row] & (1ULL << col + row * numCols)) ? 1 : 0;
      lc.setLed(0, row, col, bit);
    }
  }
}

void displayAboutImage() {
  // Iterate through the image data and set LEDs accordingly
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      int bit = (aboutImage[row] & (1ULL << col + row * numCols)) ? 1 : 0;
      lc.setLed(0, row, col, bit);
    }
  }
}

void displaySettingsImage() {
  // Iterate through the image data and set LEDs accordingly
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      int bit = (settingsImage[row] & (1ULL << col + row * numCols)) ? 1 : 0;
      lc.setLed(0, row, col, bit);
    }
  }
}
void displayClearBoard() {
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
        lc.setLed(0, row, col, 0);
      } 
    }
}

void resetParameters() {
  mapGenerated = false;
  playerIsDead = false;
  deathAnimationActive = false;
  winAnimationActive = false;
  bombIsActive = false;
  playerLives = 3;
  playerBlinkState = false;
  gameOver=false;
  allWallsDestroyed = false;
} 
