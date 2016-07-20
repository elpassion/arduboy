#include "snake_game.h"

SnakeGame::SnakeGame(int snakeColumns, int snakeRows) :
    snake(Snake(snakeColumns, snakeRows)),
    snakeController(SnakeController(snake)),
    started(false),
    lost(false) {
}

void SnakeGame::newGame() {
  started = true;
  lost = false;
  settings.setShowGameOverText(true);
  score = 0;
  snake.initSnake();
}

void SnakeGame::setNextMove(SnakeMove move) {
  snakeController.setNextMove(move);
}

void SnakeGame::gameTick() {
  if (getState() == IN_GAME) {
    SnakeState state = snakeController.moveSnake();
    if (state == ate) {
      score += settings.getLevel();
    } else if (state == died) {
      lost = true;
    }
  }
}

Settings& SnakeGame::getSettings() {
  return settings;
}

int SnakeGame::getScore() {
  return score;
}

SnakeGameState SnakeGame::getState() {
  if (!started) {return INIT;}
  if (lost) {return LOST;}
  return IN_GAME;
}

Snake& SnakeGame::getSnake() {
  return snake;
}
