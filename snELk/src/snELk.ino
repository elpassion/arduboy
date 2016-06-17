#include <MemoryFree.h>
#include <Arduboy.h>

#define SNAKE_WEIGHT 3
#define SNAKE_SPACE 1
#define FRAME_THICKNESS 1

struct SnakePart {
  SnakePart* next;
  SnakePart* prev;
  int row;
  int column;
};

struct Snake {
  SnakePart* head;
  SnakePart* tail;
};

enum SnakeMove {
    left,
    right,
    up,
    down
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
  SnakeMove lastMove;
  DisplayProperties displayProperties;
};

Arduboy arduboy;
GameState gameState;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(1);
  gameState = initGameState();
}

GameState initGameState() {
  GameState gameState;

  gameState.snake.tail = NULL;
  gameState.snake.head = NULL;
  gameState.started = false;
  gameState.lost = false;
  gameState.displayProperties = initDisplayProperties();

  return gameState;
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
  handleInput(gameState);
  if (!arduboy.nextFrame()) return;
  moveSnake(gameState);
  renderGame(gameState);
  Serial.print(freeMemory());
  Serial.println(F(" free Bytes"));
}

// HANDLE INPUT
void handleInput(GameState& gameState) {
  uint8_t buttons = arduboy.getInput();

  if(gameStartedAndNotLost(gameState)) {
    if(buttons & LEFT_BUTTON) {
      gameState.lastMove = left;
    } else if(buttons & RIGHT_BUTTON) {
      gameState.lastMove = right;
    } else if(buttons & UP_BUTTON) {
      gameState.lastMove = up;
    } else if(buttons & DOWN_BUTTON) {
      gameState.lastMove = down;
    }
  } else {
    if(buttons & A_BUTTON) {
      startGame(gameState);
    }
  }
}

bool gameStartedAndNotLost(GameState gameState) {
  return gameState.started && !gameState.lost;
}

void moveSnake(GameState& gameState) {
  if (!gameState.started) return;

  SnakePart* head = gameState.snake.head;
  int column = head->column;
  int row = head->row;

  if (gameState.lastMove == left) {
    column--;
  } else if (gameState.lastMove == right) {
    column++;
  } else if (gameState.lastMove == up) {
    row--;
  } else if (gameState.lastMove == down) {
    row++;
  }

  if (validateMove(gameState.displayProperties, column, row)) {
    makeTailToBecomeNewHead(gameState.snake, column, row);
  } else {
    gameState.lost = true;
  }
}

bool validateMove(DisplayProperties displayProperties, int column, int row) {
  return !isFrameHit(displayProperties, column, row);
}

bool isFrameHit(DisplayProperties displayProperties, int column, int row) {
  return column <= 0 ||
    column > displayProperties.maxSnakeColumns ||
    row <= 0 ||
    row > displayProperties.maxSnakeRows;
}

void makeTailToBecomeNewHead(Snake& snake, int column, int row) {
  snake.tail = snake.tail->next;

  SnakePart* newHead = snake.tail->prev;
  snake.tail->prev = NULL;

  newHead->next = NULL;
  newHead->prev = snake.head;
  newHead->row = row;
  newHead->column = column;
  snake.head->next = newHead;
  snake.head = newHead;
}

void startGame(GameState& gameState) {
  gameState.started = true;
  gameState.lost = false;
  deleteSnake(gameState.snake);
  gameState.snake = initSnake(gameState.displayProperties);
  gameState.lastMove = right;
}

void deleteSnake(Snake snake) {
  SnakePart* snakePart = snake.tail;
  while(snakePart) {
    SnakePart* nextPart = snakePart->next;
    delete snakePart;
    snakePart = nextPart;
  }
}

Snake initSnake(DisplayProperties displayProperties) {
  int centerRow = displayProperties.maxSnakeRows / 2;
  int centerColumn = displayProperties.maxSnakeColumns / 2;

  Snake snake;
  snake.head = newSnakePart(centerColumn, centerRow);
  SnakePart* middle = newSnakePart(centerColumn - 1, centerRow);
  snake.tail = newSnakePart(centerColumn - 2, centerRow);

  snake.head->next = NULL;
  snake.head->prev = middle;

  middle->next = snake.head;
  middle->prev = snake.tail;

  snake.tail->next = middle;
  snake.tail->prev = NULL;

  return snake;
}

SnakePart* newSnakePart(int column, int row) {
  SnakePart* snakePart = new SnakePart;

  snakePart->column = column;
  snakePart->row = row;

  return snakePart;
}

// RENDER GAME
void renderGame(GameState gameState) {
  arduboy.clear();
  if (gameState.started) {
    renderInGameScreen(gameState);
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

void renderInGameScreen(GameState gameState) {
  renderFrame(gameState.displayProperties, WHITE);
  renderSnake(gameState.snake);
}

void renderFrame(DisplayProperties displayProperties, uint8_t color) {
  arduboy.drawFastVLine(0, 0, displayProperties.frameY, color);
  arduboy.drawFastVLine(displayProperties.frameX, 0, displayProperties.frameY + FRAME_THICKNESS, color);
  arduboy.drawFastHLine(0, 0, displayProperties.frameX, color);
  arduboy.drawFastHLine(0, displayProperties.frameY, displayProperties.frameX + FRAME_THICKNESS, color);
}

void renderSnake(Snake snake) {
  SnakePart* snakePart = snake.head;
  while(snakePart) {
    int column = calcPixelPosition(snakePart->column);
    int row = calcPixelPosition(snakePart->row);
    arduboy.fillRect(column, row, SNAKE_WEIGHT, SNAKE_WEIGHT, WHITE);
    snakePart = snakePart->prev;
  }
}

int calcPixelPosition(int snakePosition) {
  return FRAME_THICKNESS + SNAKE_SPACE + (SNAKE_WEIGHT + SNAKE_SPACE) * (snakePosition - 1);
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
