#include <MemoryFree.h>
#include <Arduboy.h>

#include "arduboy_button_listener.hpp"
#include "display_config.h"

#include "snake_game.h"
#include "game_renderer.h"

Arduboy arduboy;
ButtonListener buttonListener;
DisplayConfig displayConfig;

SnakeGame game(displayConfig.getMaxSnakeColumns(), displayConfig.getMaxSnakeRows());
GameRenderer renderer(game, arduboy);

void setup() {
  arduboy.begin();
}

void loop() {
  handleInput(arduboy.getInput());
  if (!arduboy.nextFrame()) return;
  game.gameTick();
  renderer.render();
  Serial.print(freeMemory());
  Serial.println(F(" free Bytes"));
}

// HANDLE INPUT
void handleInput(uint8_t buttons) {
  SnakeGameState state = game.getState();
  if (state == INIT) {
    buttonListener.onUpButtonClick(buttons, incrementLevel);
    buttonListener.onDownButtonClick(buttons, decrementLevel);
    buttonListener.onBButtonClick(buttons, triggerSound);
    buttonListener.onAButtonClick(buttons, startGame);
  } else if (state == IN_GAME) {
    handleSnakeMove(buttons);
  } else if (state == LOST) {
    buttonListener.onUpButtonClick(buttons, incrementLevel);
    buttonListener.onDownButtonClick(buttons, decrementLevel);
    buttonListener.onBButtonClick(buttons, triggerGameOverText);
    buttonListener.onAButtonClick(buttons, startGame);
  }
}

void startGame() {
  arduboy.setFrameRate(game.getSettings().getLevel());
  arduboy.initRandomSeed();
  game.newGame();
}

void incrementLevel() {
  game.getSettings().incrementLevel();
}

void decrementLevel() {
  game.getSettings().decrementLevel();
}

void triggerSound() {
  game.getSettings().triggerSound();
}

void triggerGameOverText() {
  game.getSettings().triggerGameOverText();
}

void handleSnakeMove(uint8_t buttons) {
  if(buttons & LEFT_BUTTON) {
    game.setNextMove(left);
  } else if(buttons & RIGHT_BUTTON) {
    game.setNextMove(right);
  } else if(buttons & UP_BUTTON) {
    game.setNextMove(up);
  } else if(buttons & DOWN_BUTTON) {
    game.setNextMove(down);
  }
}
