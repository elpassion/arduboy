#include "game_renderer.h"
#include "assets.h"

GameRenderer::GameRenderer(SnakeGame& game, Arduboy& arduboy) :
      game(game),
      arduboy(arduboy) {
}

void GameRenderer::render() {
  arduboy.clear();
  if (game.getState() == INIT) {
    renderInitialScreen();
  } else {
    renderInGameScreen();
    if (game.getState() == LOST && game.getSettings().isShowGameOverText()) {
      renderGameOverText();
    }
  }
  arduboy.display();
}

void GameRenderer::renderInitialScreen() {
  arduboy.drawBitmap(0, 0, snelk_intro + 2, snelk_intro[0], snelk_intro[1], WHITE);
  arduboy.setCursor(0, 57);
  arduboy.print(F("level"));
  arduboy.setCursor(35, 57);
  arduboy.print(game.getSettings().getLevel());
  arduboy.setCursor(72, 57);
  arduboy.print(F("sound"));
  arduboy.setCursor(72 + 39, 57);
  arduboy.print(game.getSettings().isSound() ? F("on") : F("off"));
}

void GameRenderer::renderInGameScreen() {
  renderFrame(WHITE);
  renderSnake();
  renderFood();
}

void GameRenderer::renderFrame(uint8_t color) {
  arduboy.drawFastVLine(0, 0, displayConfig.getFrameY(), color);
  arduboy.drawFastVLine(displayConfig.getFrameX(), 0, displayConfig.getFrameY() + FRAME_THICKNESS, color);
  arduboy.drawFastHLine(0, 0, displayConfig.getFrameX(), color);
  arduboy.drawFastHLine(0, displayConfig.getFrameY(), displayConfig.getFrameX() + FRAME_THICKNESS, color);
}

void GameRenderer::renderSnake() {
  Snake snake = game.getSnake();

  int column = snake.columnOfPosition(snake.getPositions()[0]);
  int row = snake.rowOfPosition(snake.getPositions()[0]);
  for(int i = 1 ; i <= snake.getHeadIndex() ; i++) {
    int nextColumn = snake.columnOfPosition(snake.getPositions()[i]);
    int nextRow = snake.rowOfPosition(snake.getPositions()[i]);

    renderSnakePart(column, row, nextColumn - column, nextRow - row);

    column = nextColumn;
    row = nextRow;
  }
  renderSnakePart(column, row, 0, 0);
}

void GameRenderer::renderSnakePart(int column, int row, int nextColumnDiff, int nextRowDiff) {
  int columnToPrint = calcPixelPosition(column);
  int rowToPrint = calcPixelPosition(row);
  int widthToPrint = SNAKE_WEIGHT;
  int heightToPrint = SNAKE_WEIGHT;

  if (nextRowDiff != 0) {
    heightToPrint += SNAKE_SPACE;
    if (nextRowDiff < 0) {
      rowToPrint -= SNAKE_SPACE;
    }
  }

  if (nextColumnDiff != 0) {
    widthToPrint += SNAKE_SPACE;
    if (nextColumnDiff < 0) {
      columnToPrint -= SNAKE_SPACE;
    }
  }

  arduboy.fillRect(columnToPrint, rowToPrint, widthToPrint, heightToPrint, WHITE);
}

void GameRenderer::renderFood() {
  Snake snake = game.getSnake();
  int foodPosition = snake.getPositions()[snake.getFoodIndex()];
  renderFoodPart(snake.columnOfPosition(foodPosition), snake.rowOfPosition(foodPosition));
}

void GameRenderer::renderFoodPart(int column, int row) {
  int columnPixel = calcPixelPosition(column);
  int rowPixel = calcPixelPosition(row);

  // left-up
  arduboy.drawLine(
    columnPixel,
    rowPixel + SNAKE_WEIGHT/2,
    columnPixel + SNAKE_WEIGHT/2,
    rowPixel,
    WHITE);

  // left-down
  arduboy.drawLine(
    columnPixel,
    rowPixel + SNAKE_WEIGHT/2,
    columnPixel + SNAKE_WEIGHT/2,
    rowPixel + SNAKE_WEIGHT - 1,
    WHITE);

  // right-up
  arduboy.drawLine(
    columnPixel + SNAKE_WEIGHT - 1,
    rowPixel + SNAKE_WEIGHT/2,
    columnPixel + SNAKE_WEIGHT/2,
    rowPixel,
    WHITE);

  // right-down
  arduboy.drawLine(
    columnPixel + SNAKE_WEIGHT - 1,
    rowPixel + SNAKE_WEIGHT/2,
    columnPixel + SNAKE_WEIGHT/2,
    rowPixel + SNAKE_WEIGHT - 1,
    WHITE);
}

int GameRenderer::calcPixelPosition(int snakePosition) {
  return FRAME_THICKNESS + SNAKE_SPACE + (SNAKE_WEIGHT + SNAKE_SPACE) * snakePosition;
}

void GameRenderer::renderGameOverText() {
  arduboy.setCursor(37, 16);
  arduboy.print(F("GAME OVER"));
  arduboy.setCursor(37, 28);
  arduboy.print(F("Score"));
  arduboy.setCursor(72, 28);
  arduboy.print(game.getScore());
  arduboy.setCursor(3, 53);
  arduboy.print(F("level"));
  arduboy.setCursor(38, 53);
  arduboy.print(game.getSettings().getLevel());
}
