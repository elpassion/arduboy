#include "snake.h"
#include <Arduboy.h> // random() function required

Snake::Snake(int columns, int rows) :
      snakeColumns(columns),
      snakeRows(rows),
      positions(new int[snakeColumns * snakeRows]),
      lastPositionIndex(snakeColumns * snakeRows - 1) {
}

void Snake::initSnake() {
  for(int i = 0; i <= lastPositionIndex ; i++)  {
      positions[i] = i;
  }
  headIndex = -1;

  int centerColumn = snakeColumns / 2;
  int centerRow = snakeRows / 2;
  appendToSnakeHead(centerColumn - 1, centerRow);
  appendToSnakeHead(centerColumn, centerRow);
  appendToSnakeHead(centerColumn + 1, centerRow);

  shufflePositions(headIndex + 1, lastPositionIndex);
}

SnakeState Snake::moveSnake(SnakeMove move) {
  int headColumn = columnOfPosition(positions[headIndex]);
  int headRow = rowOfPosition(positions[headIndex]);

  if (move == left) {
    headColumn--;
  } else if (move == right) {
    headColumn++;
  } else if (move == up) {
    headRow--;
  } else if (move == down) {
    headRow++;
  }

  if (isValidMove(headColumn, headRow)) {
    return makeSnakeMove(headColumn, headRow);
  }
  return died;
}

int* Snake::getPositions() {
  return positions;
}

int Snake::getHeadIndex() {
  return headIndex;
}

int Snake::getFoodIndex() {
  return headIndex + 1;
}

int Snake::getLastPositionIndex() {
  return lastPositionIndex;
}

SnakeState Snake::makeSnakeMove(int column, int row) {
  int newHeadIndex = findIndexOfPosition(column, row);

  if (isFood(newHeadIndex)) {
    headIndex++;
    return ate;
  }
  int newHeadPosition = positions[newHeadIndex];
  int tailPosition = positions[0];
  for (int i = 0 ; i < headIndex ; i++) {
    positions[i] = positions[i+1];
  }
  positions[headIndex] = newHeadPosition;
  if (newHeadPosition != tailPosition) {
    positions[newHeadIndex] = tailPosition;
  }
  return moved;
}

bool Snake::isValidMove(int column, int row) {
  return !(isFrame(column, row) || isSnake(column, row));
}

bool Snake::isFrame(int column, int row) {
  return column < 0 || column >= snakeColumns ||
    row < 0 || row >= snakeRows;
}

bool Snake::isSnake(int column, int row) {
  int position = decodePosition(column, row);
  for (int i = 1 ; i <= headIndex ; i++) {
    if (positions[i] == position) {
      return true;
    }
  }
  return false;
}

bool Snake::isFood(int index) {
  return index == getFoodIndex();
}

void Snake::appendToSnakeHead(int column, int row) {
  swapPositions(++headIndex, findIndexOfPosition(column, row));
}

void Snake::shufflePositions(int firstIndex, int lastIndex) {
  for (int i = firstIndex ; i < lastIndex ; i++) {
    int randomIndex = random(firstIndex, lastIndex + 1);
    if (randomIndex != firstIndex) {
      swapPositions(firstIndex, randomIndex);
    }
  }
}

void Snake::swapPositions(int firstIndex, int secondIndex) {
  int temp = positions[firstIndex];
  positions[firstIndex] = positions[secondIndex];
  positions[secondIndex] = temp;
}

int Snake::findIndexOfPosition(int column, int row) {
  int position = decodePosition(column, row);
  for (int i = 0; i <= lastPositionIndex ; i++) {
    if (positions[i] == position) {
      return i;
    }
  }
  return -1;
}

int Snake::decodePosition(int column, int row) {
  return row * snakeColumns + column;
}

int Snake::columnOfPosition(int position) {
  return position % snakeColumns;
}

int Snake::rowOfPosition(int position) {
  return position / snakeColumns;
}
