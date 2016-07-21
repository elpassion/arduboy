#ifndef snake_h
#define snake_h

enum SnakeState {moved, ate, died};

enum SnakeMove {left, right, up, down};

class Snake {
  public:
    Snake(int columns, int rows);
    void initSnake();
    SnakeState moveSnake(SnakeMove move);

    int* getPositions();
    int getHeadIndex();
    int getFoodIndex();
    int getLastPositionIndex();

    int decodePosition(int column, int row);
    int columnOfPosition(int position);
    int rowOfPosition(int position);
  private:
    int snakeColumns;
    int snakeRows;

    int* positions;
    int headIndex;
    int lastPositionIndex;

    SnakeState makeSnakeMove(int column, int row);
    bool isValidMove(int column, int row);
    bool isFrame(int column, int row);
    bool isSnake(int column, int row);
    bool isFood(int index);

    void appendToSnakeHead(int column, int row);
    void shufflePositions(int firstIndex, int lastIndex);

    void swapPositions(int firstIndex, int secondIndex);
    int findIndexOfPosition(int column, int row);
};

#endif
