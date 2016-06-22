#ifndef snake_controller_h
#define snake_controller_h

#include "snake.h"

class SnakeController {
  public:
    SnakeController(Snake& snake);
    void setNextMove(SnakeMove move);
    SnakeState moveSnake();

  private:
    Snake& snake;
    SnakeMove lastMove;
    SnakeMove nextMove;
    bool fault;
};

#endif
