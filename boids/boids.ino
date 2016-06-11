#include <Arduino.h>

#include <Arduboy.h>
#include "arduboy_renderer.h"
#include "engine.h"

auto arduboy  = Arduboy();
auto renderer = ArduboyRenderer(arduboy);
auto engine   = Engine({WIDTH, HEIGHT}, renderer, 10);

void setup() {
  arduboy.begin();
  arduboy.initRandomSeed();
  arduboy.setFrameRate(60);
}

void loop() {
  if (!arduboy.nextFrame())
    return;

  arduboy.clear();
  engine.Render();
  arduboy.display();
  handleInput();
  engine.Move();
}

void handleInput() {
  if (arduboy.pressed(A_BUTTON)) {
    engine.EnableScatter();
  } else {
    engine.DisableScatter();
  }
}
