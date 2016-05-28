#include <Arduboy.h>
#include "arduboy_renderer.h"
#include "engine.h"

auto arduboy  = Arduboy();
auto renderer = ArduboyRenderer(&arduboy);
auto engine   = Engine({WIDTH, HEIGHT}, 20, &renderer);

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);
}

void loop() {
  if (!(arduboy.nextFrame()))
    return;

  engine.Move();
  
  arduboy.clear();
  engine.Render();
  arduboy.display();
}

