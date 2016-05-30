#include "arduboy_renderer.h"

void ArduboyRenderer::Render(const Boid& boid) const {
  if (!arduboy_)
    return;

  const auto x = boid.position().x,
             y = boid.position().y;

  if (x < 0 || y < 0 || x >= WIDTH || y >= WIDTH)
    return;

  RenderTail(boid);
  
  switch (boid.type()) {
    case Boid::BoidType::kEmptyCircle:
      arduboy_->fillCircle(x, y, 2, BLACK);
      arduboy_->drawCircle(x, y, 2, WHITE);
      break;
    case Boid::BoidType::kFullCircle:
      arduboy_->fillCircle(x, y, 1, WHITE);
      break;
    case Boid::BoidType::kEmptyTriangle:
      arduboy_->fillRect(x - 1, y - 1, 3, 3, BLACK);
      arduboy_->drawRect(x - 1, y - 1, 3, 3, WHITE);
      break;
    case Boid::BoidType::kFullTriangle:
      arduboy_->fillRect(x - 1, y - 1, 3, 3, WHITE);
      break;
  }
}

void ArduboyRenderer::RenderTail(const Boid& boid) const {
  arduboy_->drawLine(boid.position().x, boid.position().y, boid.previous_position(0).x, boid.previous_position(0).y, WHITE);
  arduboy_->drawLine(boid.previous_position(0).x, boid.previous_position(0).y, boid.previous_position(1).x, boid.previous_position(1).y, WHITE);
  arduboy_->drawLine(boid.previous_position(1).x, boid.previous_position(1).y, boid.previous_position(2).x, boid.previous_position(2).y, WHITE);
}

