#include <MemoryFree.h>
#include <Arduboy.h>

#define SNAKE_WEIGHT 3
#define SNAKE_SPACE 1
#define FRAME_THICKNESS 1

#define MAX_POSITIONS 465

enum SnakeMove {
    left,
    right,
    up,
    down
};

struct Snake {
  SnakeMove lastMove;
  SnakeMove nextMove;
  int headIndex;
  int lastPositionIndex;
  int* positions;
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
  Snake snake;
  DisplayProperties displayProperties;
};

Arduboy arduboy;
GameState gameState;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(1);
  initGame();
}

GameState initGame() {
  gameState.started = false;
  gameState.lost = false;
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
    if(buttons & A_BUTTON) {
      startGame();
    }
  }
}

bool gameStartedAndNotLost() {
  return gameState.started && !gameState.lost;
}

void startGame() {
  gameState.started = true;
  gameState.lost = false;
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
}

void appendToSnakeHead(int column, int row) {
  int index = findIndexOfPosition(column, row);
  swap(++gameState.snake.headIndex, index);
}

void shufflePositions(int firstIndex, int lastIndex) {
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
  if (!gameState.started) return;
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
  } else {
    gameState.lost = true;
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
    gameState.snake.headIndex++;
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
    if (gameState.lost) {
      renderReplayScreen();
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
  for(int i = 0 ; i <= snake.headIndex ; i++) {
    int columnPixel = calcPixelPosition(encodeColumn(snake.positions[i]));
    int rowPixel = calcPixelPosition(encodeRow(snake.positions[i]));
    arduboy.fillRect(columnPixel, rowPixel, SNAKE_WEIGHT, SNAKE_WEIGHT, WHITE);
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

void renderReplayScreen() {
  arduboy.setCursor(37, 15);
  arduboy.print(F("GAME OVER"));
  arduboy.setCursor(16, 30);
  arduboy.print(F("Press A to start"));
}

void renderInitialScreen() {
  arduboy.setCursor(16, 50);
  arduboy.print(F("Press A to start"));
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
