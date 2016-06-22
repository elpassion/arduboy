#ifndef snake_game_h
#define snake_game_h

#include "snake.h"
#include "snake_controller.h"
#include "settings.h"

#define EAT_TONE_FREQUENCY 1200
#define EAT_TONE_DURATION 20

#define LOSE_TONE_FREQUENCY 65
#define LOSE_TONE_DURATION 750

enum SnakeGameState {INIT, IN_GAME, LOST};

class SnakeGame {
  public:
    SnakeGame(int snakeColumns, int snakeRows);
    void newGame();
    void setNextMove(SnakeMove move);
    void gameTick();

    Settings& getSettings();

    int getScore();
    SnakeGameState getState();
    Snake& getSnake();

  private:
    Snake snake;
    SnakeController snakeController;
    Settings settings;

    int score;
    bool started;
    bool lost;
};

#endif
