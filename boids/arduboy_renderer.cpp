#include "arduboy_renderer.h"
#include "bitmaps.h"

void ArduboyRenderer::Render(const Boid& boid) const {
  if (!arduboy_)
    return;

  arduboy_->drawLine(boid.position().x, boid.position().y, boid.previous_position(0).x, boid.previous_position(0).y, WHITE);
  arduboy_->drawLine(boid.previous_position(0).x, boid.previous_position(0).y, boid.previous_position(1).x, boid.previous_position(1).y, WHITE);
  arduboy_->drawPixel(boid.position().x, boid.position().y, BLACK);
  arduboy_->drawBitmap(boid.position().x - 1, boid.position().y - 1, boid_bitmaps[boid.personality().shape], 3, 3, WHITE);
}
