#ifndef game_renderer_h
#define game_renderer_h

#include <Arduboy.h>
#include "snake_game.h"
#include "display_config.h"

class GameRenderer {
  public:
    GameRenderer(SnakeGame& snakeGame, Arduboy& arduboy);
    void render();

  private:
    SnakeGame& game;
    Arduboy& arduboy;
    DisplayConfig displayConfig;

    void renderInitialScreen();
    void renderInGameScreen();
    void renderFrame(uint8_t color);
    void renderSnake();
    void renderSnakePart(int column, int row, int nextColumnDiff, int nextRowDiff);
    void renderFood();
    void renderFoodPart(int column, int row);
    int calcPixelPosition(int snakePosition);
    void renderGameOverText();
};

#endif
