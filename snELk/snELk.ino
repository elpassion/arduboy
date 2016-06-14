#include <Arduboy.h>

#define B_BUTTON 0b00000100
#define A_BUTTON 0b00001000
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

Arduboy arduboy;
Snake* snake;

void setup() {
  arduboy.begin();
  snake = initSnake(50, 50);
}

Snake* initSnake(int column, int row) {
  SnakePart* snakePart = new SnakePart;
  snakePart->column = column;
  snakePart->row = row;
  snakePart->next = NULL;
  snakePart->prev = NULL;

  Snake* snake = new Snake;
  snake->head = snakePart;
  snake->tail = snakePart;

  return snake;
}

void loop() {
  if (!arduboy.nextFrame()) return;
  handleInput();
  renderGame();
}

void handleInput() {
  uint8_t buttons = arduboy.getInput();

  if(buttons & LEFT) {
    moveSnake(snake, left);
  } else if(buttons & RIGHT) {
    moveSnake(snake, right);
  } else if(buttons & UP) {
    moveSnake(snake, up);
  } else if(buttons & DOWN) {
    moveSnake(snake, down);
  }

  if(buttons & A_BUTTON) {
    Serial.println("A button pressed");
  }
  if(buttons & B_BUTTON) {
    Serial.println("B button pressed");
  }
}

void moveSnake(Snake* snake, enum SnakeMove move) {

}

void renderGame() {
  arduboy.clear();
  renderFrame(WHITE);
  renderSnake(snake);
  arduboy.display();
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
    arduboy.drawPixel(snakePart->row, snakePart->column, WHITE);
    snakePart = snakePart->prev;
  }
}
