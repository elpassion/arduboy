#include <Arduboy.h>

#define SNAKE_WEIGHT 1
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

  displayProperties->maxSnakeColumns = (WIDTH - 2*FRAME_THICKNESS - SNAKE_SPACE) / (SNAKE_WEIGHT + SNAKE_SPACE);
  displayProperties->maxSnakeRows = (HEIGHT - 2*FRAME_THICKNESS - SNAKE_SPACE) / (SNAKE_WEIGHT + SNAKE_SPACE);
  displayProperties->frameX = FRAME_THICKNESS + displayProperties->maxSnakeColumns * (SNAKE_WEIGHT + SNAKE_SPACE) + SNAKE_SPACE;
  displayProperties->frameY = FRAME_THICKNESS + displayProperties->maxSnakeRows * (SNAKE_WEIGHT + SNAKE_SPACE) + SNAKE_SPACE;

  return displayProperties;
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

  if(gameState->started && !gameState->lost) {
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

void moveSnake(GameState* gameState) {
  if (!gameState->started) return;

  SnakePart* head = gameState->snake->head;
  int newColumn = head->column;
  int newRow = head->row;

  switch(gameState->lastMove) {
    case left:
      newColumn--;
      break;
    case right:
      newColumn++;
      break;
    case up:
      newRow--;
      break;
    case down:
      newRow++;
      break;
  }
  if (validateMove(gameState, newColumn, newRow)) {
    makeTailToBecomeHead(gameState->snake, newColumn, newRow);
  } else {
    gameState->lost = true;
  }
}

bool validateMove(GameState* gameState, int column, int row) {
  if (isFrameHit(column, row)) {
    return false;
  }
  return true;
}

bool isFrameHit(int column, int row) {
  return (column == 0 || column == gameState->displayProperties->frameX || row == 0 || row == gameState->displayProperties->frameY);
}

void makeTailToBecomeHead(Snake* snake, int headColumn, int headRow) {
  snake->tail = snake->tail->next;

  SnakePart* newHead = snake->tail->prev;
  snake->tail->prev = NULL;

  newHead->next = NULL;
  newHead->prev = snake->head;
  newHead->column = headColumn;
  newHead->row = headRow;
  snake->head->next = newHead;
  snake->head = newHead;
}

void startGame(GameState* gameState) {
  gameState->started = true;
  gameState->lost = false;
  if (gameState->snake) deleteSnake(gameState->snake);
  gameState->snake = initSnake(50, 50);
  gameState->lastMove = right;
}

void deleteSnake(Snake* snake) {
  SnakePart* snakePart = snake->tail;
  while(snakePart) {
    SnakePart* nextPart = snakePart->next;
    delete snakePart;
    snakePart = nextPart;
  }
  delete snake;
}

Snake* initSnake(int column, int row) {
  SnakePart* head = new SnakePart;
  head->column = column;
  head->row = row;

  SnakePart* tail = new SnakePart;
  tail->column = column - 1;
  tail->row = row;

  head->next = NULL;
  head->prev = tail;

  tail->next = head;
  tail->prev = NULL;

  Snake* snake = new Snake;
  snake->head = head;
  snake->tail = tail;

  return snake;
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
  renderFrame(WHITE);
  renderSnake(gameState->snake);
}

void renderFrame(uint8_t color) {
  arduboy.drawFastVLine(0, 0, gameState->displayProperties->frameY, color);
  arduboy.drawFastVLine(gameState->displayProperties->frameX, 0, gameState->displayProperties->frameY + FRAME_THICKNESS, color);
  arduboy.drawFastHLine(0, 0, gameState->displayProperties->frameX, color);
  arduboy.drawFastHLine(0, gameState->displayProperties->frameY, gameState->displayProperties->frameX + FRAME_THICKNESS, color);
}

void renderSnake(Snake* snake) {
  SnakePart* snakePart = snake->head;
  while(snakePart) {
    arduboy.drawPixel(snakePart->column, snakePart->row, WHITE);
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
