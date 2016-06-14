#include <Arduboy.h>

#define A_BUTTON 0b00000100
#define B_BUTTON 0b00001000
#define DOWN     0b00010000
#define LEFT     0b00100000
#define RIGHT    0b01000000
#define UP       0b10000000

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

struct GameState {
  bool started;
  bool lost;
  Snake* snake;
};

Arduboy arduboy;
GameState* gameState;

void setup() {
  arduboy.begin();
  gameState = initGameState();
}

GameState* initGameState() {
  GameState* gameState = new GameState;
  gameState->started = false;
  gameState->lost = false;
  return gameState;
}

void loop() {
  if (!arduboy.nextFrame()) return;
  handleInput(gameState);
  renderGame(gameState);
}

void handleInput(GameState* gameState) {
  uint8_t buttons = arduboy.getInput();

  if(gameState->started) {
    if(buttons & LEFT) {
      moveSnake(gameState->snake, left);
    } else if(buttons & RIGHT) {
      moveSnake(gameState->snake, right);
    } else if(buttons & UP) {
      moveSnake(gameState->snake, up);
    } else if(buttons & DOWN) {
      moveSnake(gameState->snake, down);
    }
  } else {
    if(buttons & A_BUTTON) {
      startGame(gameState);
    }
  }
}

void moveSnake(Snake* snake, enum SnakeMove move) {
  int newColumn = snake->head->column;
  int newRow = snake->head->row;

  switch(move) {
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
  if (validateMove(newColumn, newRow)) {
    makeTailToBecomeHead(snake, newColumn, newRow);    
  }
}

void startGame(GameState* gameState) {
  gameState->started = true;
  gameState->snake = initSnake(50, 50);
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

bool validateMove(int column, int row) {
  return true;
}

void renderGame(GameState* gameState) {
  arduboy.clear();
  if (gameState->started) {
    renderInGameScreen(gameState);
  } else {
    if (gameState->lost) {
      renderReplayScreen();
    } else {
      renderInitialScreen();
    }
  }
  arduboy.display();
}

void renderInGameScreen(GameState* gameState) {
  renderFrame(WHITE);
  renderSnake(gameState->snake);
}

void renderReplayScreen() {
  
}

void renderInitialScreen() {
  arduboy.setCursor(16, 50);
  arduboy.print("Press A to start");
}

void renderFrame(uint8_t color) {
  arduboy.drawFastVLine(0, 0, 64, color);
  arduboy.drawFastVLine(127, 0, 64, color);
  arduboy.drawFastHLine(0, 63, 128, color);
  arduboy.drawFastHLine(0, 0, 128, color);
}

void renderSnake(Snake* snake) {
  SnakePart* snakePart = snake->head;
  while(snakePart) {
    arduboy.drawPixel(snakePart->column, snakePart->row, WHITE);
    snakePart = snakePart->prev;
  }
}

