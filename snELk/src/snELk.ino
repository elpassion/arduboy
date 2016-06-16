#include <Arduboy.h>

#define SNAKE_WEIGHT 1
#define SNAKE_SPACE 1
#define FRAME_THICKNESS 1

struct Position {
  int row;
  int column;
};

struct SnakePart {
  SnakePart* next;
  SnakePart* prev;
  Position* position;
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
  Snake* snake;
  SnakeMove lastMove;
  DisplayProperties* displayProperties;
};

Arduboy arduboy;
GameState* gameState;

void setup() {
  arduboy.begin();
  gameState = initGameState();
}

GameState* initGameState() {
  GameState* gameState = new GameState;

  gameState->snake = NULL;
  gameState->started = false;
  gameState->lost = false;
  gameState->displayProperties = initDisplayProperties();

  return gameState;
}

DisplayProperties* initDisplayProperties() {
  DisplayProperties* displayProperties = new DisplayProperties;

  displayProperties->maxSnakeColumns = snakeLinesInSpace(WIDTH);
  displayProperties->maxSnakeRows = snakeLinesInSpace(HEIGHT);
  displayProperties->frameX = framePosition(displayProperties->maxSnakeColumns);
  displayProperties->frameY = framePosition(displayProperties->maxSnakeRows);

  return displayProperties;
}

int snakeLinesInSpace(int screenLength) {
  int spaceBetweenFrames = screenLength - 2*FRAME_THICKNESS;
  int spaceBeforeRightFrame = SNAKE_SPACE;
  int widthOfSingleSnakeLine = SNAKE_WEIGHT + SNAKE_SPACE;
  return (spaceBetweenFrames - spaceBeforeRightFrame )/ widthOfSingleSnakeLine;
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
}

// HANDLE INPUT
void handleInput(GameState* gameState) {
  uint8_t buttons = arduboy.getInput();

  if(gameStartedAndNotLost(gameState)) {
    if(buttons & LEFT_BUTTON) {
      gameState->lastMove = left;
    } else if(buttons & RIGHT_BUTTON) {
      gameState->lastMove = right;
    } else if(buttons & UP_BUTTON) {
      gameState->lastMove = up;
    } else if(buttons & DOWN_BUTTON) {
      gameState->lastMove = down;
    }
  } else {
    if(buttons & A_BUTTON) {
      startGame(gameState);
    }
  }
}

bool gameStartedAndNotLost(GameState* gameState) {
  return gameState->started && !gameState->lost;
}

void moveSnake(GameState* gameState) {
  if (!gameState->started) return;

  Position newPositionOfHead = nextHeadPosition(gameState);
  if (validateMove(gameState->displayProperties, newPositionOfHead)) {
    makeTailToBecomeNewHead(gameState->snake, newPositionOfHead);
  } else {
    gameState->lost = true;
  }
}

Position nextHeadPosition(GameState* gameState) {
  SnakePart* head = gameState->snake->head;

  Position position;
  position.column = head->position->column;
  position.row = head->position->row;

  switch(gameState->lastMove) {
    case left:
      position.column--;
      break;
    case right:
      position.column++;
      break;
    case up:
      position.row--;
      break;
    case down:
      position.row++;
      break;
  }
  return position;
}

bool validateMove(DisplayProperties* displayProperties, Position position) {
  return !isFrameHit(displayProperties, position);
}

bool isFrameHit(DisplayProperties* displayProperties, Position position) {
  return position.column == 0 ||
    position.column == displayProperties->frameX ||
    position.row == 0 ||
    position.row == displayProperties->frameY;
}

void makeTailToBecomeNewHead(Snake* snake, Position newPositionOfHead) {
  snake->tail = snake->tail->next;

  SnakePart* newHead = snake->tail->prev;
  snake->tail->prev = NULL;

  newHead->next = NULL;
  newHead->prev = snake->head;
  newHead->position->row = newPositionOfHead.row;
  newHead->position->column = newPositionOfHead.column;
  snake->head->next = newHead;
  snake->head = newHead;
}

void startGame(GameState* gameState) {
  gameState->started = true;
  gameState->lost = false;
  if (gameState->snake) deleteSnake(gameState->snake);
  gameState->snake = initSnake(gameState->displayProperties);
  gameState->lastMove = right;
}

void deleteSnake(Snake* snake) {
  SnakePart* snakePart = snake->tail;
  while(snakePart) {
    SnakePart* nextPart = snakePart->next;
    delete snakePart->position;
    delete snakePart;
    snakePart = nextPart;
  }
  delete snake;
}

Snake* initSnake(DisplayProperties* displayProperties) {
  int column = centerPixel(WIDTH);
  int row = centerPixel(HEIGHT);

  displayProperties->frameY;
  displayProperties->maxSnakeColumns;
  displayProperties->maxSnakeRows;

  Snake* snake = new Snake;
  snake->head = newSnakePart(column - 1, row);
  snake->tail = newSnakePart(column, row);

  snake->head->next = NULL;
  snake->head->prev = snake->tail;

  snake->tail->next = snake->head;
  snake->tail->prev = NULL;

  return snake;
}

int centerPixel(int screenLength) {
  return ((screenLength - 2*FRAME_THICKNESS) / 2) + FRAME_THICKNESS;
}

SnakePart* newSnakePart(int column, int row) {
  Position* position = new Position;
  position->column = column;
  position->row = row;

  SnakePart* snakePart = new SnakePart;
  snakePart->position = position;

  return snakePart;
}

// RENDER GAME
void renderGame(GameState* gameState) {
  arduboy.clear();
  if (gameState->started) {
    renderInGameScreen(gameState);
    if (gameState->lost) {
      renderReplayScreen();
    }
  } else {
     if(!gameState->lost) {
      renderInitialScreen();
    }
  }
  arduboy.display();
}

void renderInGameScreen(GameState* gameState) {
  renderFrame(gameState->displayProperties, WHITE);
  renderSnake(gameState->snake);
}

void renderFrame(DisplayProperties* displayProperties, uint8_t color) {
  arduboy.drawFastVLine(0, 0, displayProperties->frameY, color);
  arduboy.drawFastVLine(displayProperties->frameX, 0, displayProperties->frameY + FRAME_THICKNESS, color);
  arduboy.drawFastHLine(0, 0, displayProperties->frameX, color);
  arduboy.drawFastHLine(0, displayProperties->frameY, displayProperties->frameX + FRAME_THICKNESS, color);
}

void renderSnake(Snake* snake) {
  SnakePart* snakePart = snake->head;
  while(snakePart) {
    arduboy.drawPixel(snakePart->position->column, snakePart->position->row, WHITE);
    snakePart = snakePart->prev;
  }
}

void renderReplayScreen() {
  arduboy.setCursor(37, 15);
  arduboy.print("GAME OVER");
  arduboy.setCursor(16, 30);
  arduboy.print("Press A to start");
}

void renderInitialScreen() {
  arduboy.setCursor(16, 50);
  arduboy.print("Press A to start");
}
