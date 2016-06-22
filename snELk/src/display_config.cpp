#include "display_config.h"
#include <Arduboy.h>

DisplayConfig::DisplayConfig() :
      maxSnakeColumns(snakeLinesInSpace(WIDTH)),
      maxSnakeRows(snakeLinesInSpace(HEIGHT)),
      frameX(framePosition(maxSnakeColumns)),
      frameY(framePosition(maxSnakeRows)) {
}

int DisplayConfig::getMaxSnakeRows() {
  return maxSnakeRows;
}

int DisplayConfig::getMaxSnakeColumns() {
  return maxSnakeColumns;
}

int DisplayConfig::getFrameX() {
  return frameX;
}

int DisplayConfig::getFrameY() {
  return frameY;
}

int DisplayConfig::snakeLinesInSpace(int screenLength) {
  int spaceBetweenFrames = screenLength - 2*FRAME_THICKNESS;
  int spaceBeforeRightFrame = SNAKE_SPACE;
  int widthOfSingleSnakeLine = SNAKE_WEIGHT + SNAKE_SPACE;
  return (spaceBetweenFrames - spaceBeforeRightFrame ) / widthOfSingleSnakeLine;
}

int DisplayConfig::framePosition(int snakeLines) {
  int leftFrame = FRAME_THICKNESS;
  int snakeSpace = snakeLines * (SNAKE_WEIGHT + SNAKE_SPACE);
  int spaceBeforeRightFrame = SNAKE_SPACE;
  return leftFrame + snakeSpace + spaceBeforeRightFrame;
}
