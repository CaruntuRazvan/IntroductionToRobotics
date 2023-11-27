#include "LedControl.h"

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

const int numRows = 8;
const int numCols = 8;

const int xPin = A0;
const int yPin = A1;
const int swPin = 2;

const int lowerThreshold = 450;
const int upperThreshold = 600;

const int playerBlinkRate = 550;
const int bombBlinkRate = 250; 
const int bombExplosionTime = 3000; 
const int explosionRange = 1; 
bool bombIsActive = false;
bool playerIsDead = false;
bool deathAnimationActive = false;

const int redLedPin = 8; 
int gameBoard[numRows][numCols];
int playerRow = 0;
int playerCol = 0;
bool playerBlinkState = false;
bool mapGenerated = false;

const int animationDelay = 100;
const int maxRandomValue = 100;
const int percentageWall = 50;
const int twoNumber = 2;

const int freeSpaces = 2;

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

void setup() {
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(redLedPin, OUTPUT);
  randomSeed(analogRead(5));
  if (!mapGenerated) {
    generateRandomMap();
    mapGenerated = true;
  }
}

void loop() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  int swValue = digitalRead(swPin);
  Serial.print("SW Pin Value: ");
  Serial.print(swValue);
  updatePlayerPosition(xValue, yValue);
  updatePlayerBlink();
  // Print joystick button state for debugging
  if (swValue == LOW && !playerIsDead) {
    Serial.println("Joystick Button Pressed");
    if (!bombIsActive) {
      placeBomb();
      bombIsActive = true;  // Set the flag to indicate an active bomb
    }
  }

  updateBombs();
  displayGameBoard();
  // Check if the mission is complete (all walls destroyed)
  if (playerIsDead && !deathAnimationActive) {
    playAnimation(loseImages, sizeof(loseImages) / sizeOfElement);
    deathAnimationActive = true;
    restartGame();
  }
  if (isMissionComplete() && !playerIsDead) {
    playAnimation(winImages, sizeof(winImages) / sizeOfElement);
    restartGame();
  }
  delay(100);
}

// Function to check if the mission is complete
bool isMissionComplete() {
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      if (gameBoard[row][col] == 1) {
        return false;  // There is still a wall remaining
      }
    }
  }
  return true;  // All walls have been destroyed
}

void generateRandomMap() {
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      gameBoard[row][col] = 0;
      lc.setLed(0, row, col, 0);
    }
  }

  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      if (random(maxRandomValue) < percentageWall) {
        gameBoard[row][col] = 1;
        lc.setLed(0, row, col, 1);
      }
    }
  }

  int availablePositions[numRows * numCols][twoNumber];
  int count = 0;

  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      if (gameBoard[row][col] == 0 && countFreeSpacesAround(row, col) >= freeSpaces * explosionRange) {
        availablePositions[count][0] = row;
        availablePositions[count][1] = col;
        ++count;
      }
    }
  }

  if (count > 0) {
    int randomIndex = random(count);
    playerRow = availablePositions[randomIndex][0];
    playerCol = availablePositions[randomIndex][1];
    lc.setLed(0, playerRow, playerCol, playerBlinkState);
  }
}

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

void updatePlayerPosition(int xValue, int yValue) {
  static int lastXValue = xValue;
  static int lastYValue = yValue;
  static unsigned long lastUpdateTime = millis();
  // Sensitivity threshold to avoid rapid updates
  const int sensibilityThreshold = 50;

  // Check if enough time has passed since the last update
  if (millis() - lastUpdateTime < sensibilityThreshold) {
    return;
  }

  if (abs(xValue - lastXValue) > sensibilityThreshold) {
    // Move the player left if the joystick is tilted left
    if (xValue < lowerThreshold && playerCol > 0 && !gameBoard[playerRow][playerCol - 1]) {
      lc.setLed(0, playerRow, playerCol, 0);
      --playerCol;
    }
    // Move the player right if the joystick is tilted right
    else if (xValue > upperThreshold && playerCol < numCols - 1 && !gameBoard[playerRow][playerCol + 1]) {
      lc.setLed(0, playerRow, playerCol, 0);
      ++playerCol;
    }
    // Update the last X-axis value
    lastXValue = xValue;
  }

  if (abs(yValue - lastYValue) > sensibilityThreshold) {
    // Move the player up if the joystick is tilted up
    if (yValue < lowerThreshold && playerRow > 0 && !gameBoard[playerRow - 1][playerCol]) {
      lc.setLed(0, playerRow, playerCol, 0);
      --playerRow;
    }
    // Move the player down if the joystick is tilted down
    else if (yValue > upperThreshold && playerRow < numRows - 1 && !gameBoard[playerRow + 1][playerCol]) {
      lc.setLed(0, playerRow, playerCol, 0);
      ++playerRow;
    }
    // Update the last Y-axis value
    lastYValue = yValue;
  }
  lastUpdateTime = millis();
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
    if (row >= 0 && row < numRows && col >= 0 && col < numCols && gameBoard[row][col] == 1) {
      gameBoard[row][col] = 0;
      lc.setLed(0, row, col, 0);
    } else {
      break;  // Stop exploding in this direction if a barrier is encountered
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
  playerRow = -1;
  playerCol = -1;
  playerIsDead = true;
  // Clear the game board (set all elements to 0)
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      gameBoard[row][col] = 0;
      lc.setLed(0, row, col, 0);
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
  const unsigned long animationDuration = 3000;  // 5 seconds in milliseconds
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
//reload the game
void restartGame() {
  generateRandomMap();
  playerIsDead = false;
  deathAnimationActive = false;
  bombIsActive = false;
}
