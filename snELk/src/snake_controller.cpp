#include "snake_controller.h"

SnakeController::SnakeController(Snake& snake) :
    snake(snake),
    lastMove(right),
    nextMove(right),
    fault(false) {
}

void SnakeController::setNextMove(SnakeMove move) {
  if((move == left && lastMove != right) ||
        (move == right && lastMove != left) ||
        (move == up && lastMove != down) ||
        (move == down && lastMove != up)) {
    nextMove = move;
  }
}

SnakeState SnakeController::moveSnake() {
  SnakeState state = snake.moveSnake(nextMove);
  lastMove = nextMove;
  if (state == died) {
    if (fault) {
      lastMove = right;
      nextMove = right;
      return died;
    }
    fault = true;
    return moved;
  }
  return state;
}
