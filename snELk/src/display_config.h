#ifndef display_config_hpp
#define display_config_hpp

#define SNAKE_WEIGHT 3
#define SNAKE_SPACE 1
#define FRAME_THICKNESS 1

class DisplayConfig {
  public:
    DisplayConfig();
    int getFrameX();
    int getFrameY();
    int getMaxSnakeRows();
    int getMaxSnakeColumns();

  private:
    int maxSnakeColumns;
    int maxSnakeRows;
    int frameX;
    int frameY;
    int snakeLinesInSpace(int screenLength);
    int framePosition(int snakeLines);
};

#endif
