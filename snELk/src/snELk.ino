#include <MemoryFree.h>
#include <Arduboy.h>
#include "assets.h"

#define SNAKE_WEIGHT 3
#define SNAKE_SPACE 1
#define FRAME_THICKNESS 1

#define MAX_POSITIONS 465

#define EAT_TONE_FREQUENCY 1200
#define EAT_TONE_DURATION 20

#define LOSE_TONE_FREQUENCY 65
#define LOSE_TONE_DURATION 750

enum SnakeMove {
    left,
    right,
    up,
    down
};

struct Snake {
  SnakeMove lastMove;
  SnakeMove nextMove;
  int score;
  int headIndex;
  int lastPositionIndex;
  int* positions;
  bool fault;
};

struct DisplayProperties {
  int maxSnakeRows;
  int maxSnakeColumns;
  int frameX;
  int frameY;
};

struct GameState {
  bool started;
  bool lost;
  bool showGameOverText;
  bool bButtonPressed;
  bool upButtonPressed;
  bool downButtonPressed;
  bool sound;
  int level;
  Snake snake;
  DisplayProperties displayProperties;
};

Arduboy arduboy;
GameState gameState;

void setup() {
  arduboy.begin();
  initGame();
  arduboy.setFrameRate(gameState.level);
}

GameState initGame() {
  gameState.started = false;
  gameState.lost = false;
  gameState.showGameOverText = true;
  gameState.upButtonPressed = false;
  gameState.downButtonPressed = false;
  gameState.bButtonPressed = false;
  gameState.level = 10;
  gameState.displayProperties = initDisplayProperties();
  gameState.snake.positions =  new int[MAX_POSITIONS];
}

DisplayProperties initDisplayProperties() {
  DisplayProperties displayProperties;

  displayProperties.maxSnakeColumns = snakeLinesInSpace(WIDTH);
  displayProperties.maxSnakeRows = snakeLinesInSpace(HEIGHT);
  displayProperties.frameX = framePosition(displayProperties.maxSnakeColumns);
  displayProperties.frameY = framePosition(displayProperties.maxSnakeRows);

  return displayProperties;
}

int snakeLinesInSpace(int screenLength) {
  int spaceBetweenFrames = screenLength - 2*FRAME_THICKNESS;
  int spaceBeforeRightFrame = SNAKE_SPACE;
  int widthOfSingleSnakeLine = SNAKE_WEIGHT + SNAKE_SPACE;
  return (spaceBetweenFrames - spaceBeforeRightFrame ) / widthOfSingleSnakeLine;
}

int framePosition(int snakeLines) {
  int leftFrame = FRAME_THICKNESS;
  int snakeSpace = snakeLines * (SNAKE_WEIGHT + SNAKE_SPACE);
  int spaceBeforeRightFrame = SNAKE_SPACE;
  return leftFrame + snakeSpace + spaceBeforeRightFrame;
}

void loop() {
  handleInput();
  if (!arduboy.nextFrame()) return;
  gameTick();
  renderGame();
  Serial.print(freeMemory());
  Serial.println(F(" free Bytes"));
}

// HANDLE INPUT
void handleInput() {
  uint8_t buttons = arduboy.getInput();

  if(gameStartedAndNotLost()) {
    SnakeMove lastMove = gameState.snake.lastMove;

    if(buttons & LEFT_BUTTON && lastMove != right) {
      gameState.snake.nextMove = left;
    } else if(buttons & RIGHT_BUTTON && lastMove != left) {
      gameState.snake.nextMove = right;
    } else if(buttons & UP_BUTTON && lastMove != down) {
      gameState.snake.nextMove = up;
    } else if(buttons & DOWN_BUTTON && lastMove != up) {
      gameState.snake.nextMove = down;
    }
  } else {
    if(buttons & UP_BUTTON) {
      if (!gameState.upButtonPressed) {
        if (gameState.level < 20) {
          gameState.level++;
        }
        gameState.upButtonPressed = true;
      }
    } else {
      gameState.upButtonPressed = false;
    }

    if(buttons & DOWN_BUTTON) {
      if (!gameState.downButtonPressed) {
        if (gameState.level > 1) {
          gameState.level--;
        }
        gameState.downButtonPressed = true;
      }
    } else {
      gameState.downButtonPressed = false;
    }

    if(buttons & A_BUTTON) {
      startGame();
    }
    if (buttons & B_BUTTON) {
      if (!gameState.bButtonPressed) {
        gameState.showGameOverText = !gameState.showGameOverText;
        if (!gameState.started) {
          gameState.sound = !gameState.sound;
        }
        gameState.bButtonPressed = true;
      }
    } else {
      gameState.bButtonPressed = false;
    }
  }
}

bool gameStartedAndNotLost() {
  return gameState.started && !gameState.lost;
}

void startGame() {
  gameState.started = true;
  gameState.lost = false;
  gameState.showGameOverText = true;
  arduboy.setFrameRate(gameState.level);
  initSnake();
}

void initSnake() {
  clearSnake();

  int centerColumn = gameState.displayProperties.maxSnakeColumns / 2;
  int centerRow = gameState.displayProperties.maxSnakeRows / 2;
  appendToSnakeHead(centerColumn - 2, centerRow);
  appendToSnakeHead(centerColumn - 1, centerRow);
  appendToSnakeHead(centerColumn, centerRow);

  shufflePositions(gameState.snake.headIndex + 1, gameState.snake.lastPositionIndex);
  gameState.snake.lastMove = right;
  gameState.snake.nextMove = right;
}

void clearSnake() {
  for(int i = 0; i < MAX_POSITIONS ; i++)  {
      gameState.snake.positions[i] = i;
  }
  gameState.snake.headIndex = -1;
  gameState.snake.lastPositionIndex = gameState.displayProperties.maxSnakeColumns * gameState.displayProperties.maxSnakeRows - 1;
  gameState.snake.score = 0;
  gameState.snake.fault = false;
}

void appendToSnakeHead(int column, int row) {
  int index = findIndexOfPosition(column, row);
  swap(++gameState.snake.headIndex, index);
}

void shufflePositions(int firstIndex, int lastIndex) {
  arduboy.initRandomSeed();

  for (int i = firstIndex ; i < lastIndex ; i++) {
    int randomIndex = random(firstIndex, lastIndex + 1);
    if (randomIndex != firstIndex) {
      swap(firstIndex, randomIndex);
    }
  }
}

void swap(int firstIndex, int secondIndex) {
  int* positions = gameState.snake.positions;

  int temp = positions[firstIndex];
  positions[firstIndex] = positions[secondIndex];
  positions[secondIndex] = temp;
}

void gameTick() {
  if (!gameState.started || gameState.lost) return;
  int positionOfHead = gameState.snake.positions[gameState.snake.headIndex];
  int column = encodeColumn(positionOfHead);
  int row = encodeRow(positionOfHead);

  SnakeMove nextMove = gameState.snake.nextMove;
  if (nextMove == left) {
    column--;
  } else if (nextMove == right) {
    column++;
  } else if (nextMove == up) {
    row--;
  } else if (nextMove == down) {
    row++;
  }

  if (validateMove(column, row)) {
    moveSnake(column, row);
    gameState.snake.lastMove = nextMove;
    gameState.snake.fault = false;
  } else {
    if (gameState.snake.lastMove == gameState.snake.nextMove && !gameState.snake.fault) {
      gameState.snake.fault = true;
    } else {
      if (gameState.sound) {
        arduboy.tunes.tone(LOSE_TONE_FREQUENCY, LOSE_TONE_DURATION);
      }
      gameState.lost = true;
    }
  }
}

bool validateMove(int column, int row) {
  return !isFrameHit(column, row) && !snakeHit(column, row);
}

bool isFrameHit(int column, int row) {
  return column < 0 ||
    column >= gameState.displayProperties.maxSnakeColumns ||
    row < 0 ||
    row >= gameState.displayProperties.maxSnakeRows;
}

bool snakeHit(int column, int row) {
  int position = decodePosition(column, row);
  Snake snake = gameState.snake;
  for (int i = 1 ; i <= snake.headIndex ; i++) {
    if (snake.positions[i] == position) {
      return true;
    }
  }
  return false;
}

void moveSnake(int column, int row) {
  int newHeadIndex = findIndexOfPosition(column, row);
  if (isFood(newHeadIndex)) {
    if (gameState.sound) {
      arduboy.tunes.tone(EAT_TONE_FREQUENCY, EAT_TONE_DURATION);
    }
    gameState.snake.headIndex++;
    gameState.snake.score += gameState.level;
  } else {
    Snake snake = gameState.snake;
    int newHeadPosition = snake.positions[newHeadIndex];
    int tailPosition = snake.positions[0];
    for (int i = 0 ; i < snake.headIndex; i++) {
      snake.positions[i] = snake.positions[i+1];
    }
    snake.positions[snake.headIndex] = newHeadPosition;
    if (newHeadPosition != tailPosition) {
      snake.positions[newHeadIndex] = tailPosition;
    }
  }
}

int findIndexOfPosition(int column, int row) {
  Snake snake = gameState.snake;
  int position = decodePosition(column, row);
  for (int i = 0; i <= snake.lastPositionIndex ; i++) {
    if (snake.positions[i] == position) {
      return i;
    }
  }
  return -1;
}

bool isFood(int index) {
  return index == (gameState.snake.headIndex + 1);
}

// RENDER GAME
void renderGame() {
  arduboy.clear();
  if (gameState.started) {
    renderInGameScreen();
    if (gameState.lost && gameState.showGameOverText) {
      renderGameOverText();
    }
  } else {
     if(!gameState.lost) {
      renderInitialScreen();
    }
  }
  arduboy.display();
}

void renderInGameScreen() {
  renderFrame(WHITE);
  renderSnake();
  renderFood();
}

void renderFrame(uint8_t color) {
  DisplayProperties displayProperties = gameState.displayProperties;

  arduboy.drawFastVLine(0, 0, displayProperties.frameY, color);
  arduboy.drawFastVLine(displayProperties.frameX, 0, displayProperties.frameY + FRAME_THICKNESS, color);
  arduboy.drawFastHLine(0, 0, displayProperties.frameX, color);
  arduboy.drawFastHLine(0, displayProperties.frameY, displayProperties.frameX + FRAME_THICKNESS, color);
}

void renderSnake() {
  Snake snake = gameState.snake;
  int prevColumnPixel = calcPixelPosition(encodeColumn(snake.positions[0]));
  int prevRowPixel = calcPixelPosition(encodeRow(snake.positions[0]));
  arduboy.fillRect(prevColumnPixel, prevRowPixel, SNAKE_WEIGHT, SNAKE_WEIGHT, WHITE);

  for(int i = 1 ; i <= snake.headIndex ; i++) {
    int columnPixel = calcPixelPosition(encodeColumn(snake.positions[i]));
    int rowPixel = calcPixelPosition(encodeRow(snake.positions[i]));

    int columnToPrint = columnPixel;
    int rowToPrint = rowPixel;
    int widthToPrint = SNAKE_WEIGHT;
    int heightToPrint = SNAKE_WEIGHT;

    if (columnPixel > prevColumnPixel) {
      columnToPrint -= SNAKE_SPACE;
      widthToPrint += SNAKE_SPACE;
    } else if (columnPixel == prevColumnPixel) {
      if (rowPixel > prevRowPixel) {
        rowToPrint -= SNAKE_SPACE;
        heightToPrint += SNAKE_SPACE;
      } else {
        heightToPrint += SNAKE_SPACE;
      }
    } else {
      widthToPrint += SNAKE_SPACE;
    }

    arduboy.fillRect(columnToPrint, rowToPrint, widthToPrint, heightToPrint, WHITE);

    prevColumnPixel = columnPixel;
    prevRowPixel = rowPixel;
  }
}

int calcPixelPosition(int snakePosition) {
  return FRAME_THICKNESS + SNAKE_SPACE + (SNAKE_WEIGHT + SNAKE_SPACE) * snakePosition;
}

void renderFood() {
  int foodIndex = gameState.snake.headIndex + 1;
  int columnPixel = calcPixelPosition(encodeColumn(gameState.snake.positions[foodIndex]));
  int rowPixel = calcPixelPosition(encodeRow(gameState.snake.positions[foodIndex]));

  // left-up
  arduboy.drawLine(
    columnPixel,
    rowPixel + SNAKE_WEIGHT/2,
    columnPixel + SNAKE_WEIGHT/2,
    rowPixel,
    WHITE);

  // left-down
  arduboy.drawLine(
    columnPixel,
    rowPixel + SNAKE_WEIGHT/2,
    columnPixel + SNAKE_WEIGHT/2,
    rowPixel + SNAKE_WEIGHT - 1,
    WHITE);

  // right-up
  arduboy.drawLine(
    columnPixel + SNAKE_WEIGHT - 1,
    rowPixel + SNAKE_WEIGHT/2,
    columnPixel + SNAKE_WEIGHT/2,
    rowPixel,
    WHITE);

  // right-down
  arduboy.drawLine(
    columnPixel + SNAKE_WEIGHT - 1,
    rowPixel + SNAKE_WEIGHT/2,
    columnPixel + SNAKE_WEIGHT/2,
    rowPixel + SNAKE_WEIGHT - 1,
    WHITE);
}

void renderGameOverText() {
  arduboy.setCursor(37, 16);
  arduboy.print(F("GAME OVER"));
  arduboy.setCursor(37, 28);
  arduboy.print(F("Score"));
  arduboy.setCursor(72, 28);
  arduboy.print(gameState.snake.score);
  arduboy.setCursor(3, 53);
  arduboy.print("level");
  arduboy.setCursor(38, 53);
  arduboy.print(gameState.level);
}

void renderInitialScreen() {
  arduboy.drawBitmap(0, 0, snelk_intro + 2, snelk_intro[0], snelk_intro[1], WHITE);
  arduboy.setCursor(0, 57);
  arduboy.print("level");
  arduboy.setCursor(35, 57);
  arduboy.print(gameState.level);
  arduboy.setCursor(72, 57);
  arduboy.print("sound");
  arduboy.setCursor(72 + 39, 57);
  arduboy.print(gameState.sound ? "on" : "off");
}

// POSITION ENCODING
int decodePosition(int column, int row) {
  return row * gameState.displayProperties.maxSnakeColumns + column;
}

int encodeColumn(int position) {
  return position % gameState.displayProperties.maxSnakeColumns;
}

int encodeRow(int position) {
  return position / gameState.displayProperties.maxSnakeColumns;
}
