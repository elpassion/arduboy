#include "arduboy_renderer.h"

void ArduboyRenderer::Render(const Boid& boid) const {
  if (!arduboy_)
    return;

  const auto x = boid.position().x,
             y = boid.position().y;
  const auto r = ceil(boid.size() / 2.0);

  switch (boid.type()) {
    case Boid::BoidType::kEmptyCircle:
      arduboy_->drawCircle(x, y, r, WHITE);
      break;
    case Boid::BoidType::kFullCircle:
      arduboy_->fillCircle(x, y, r, WHITE);
      break;
    case Boid::BoidType::kEmptyTriangle:
      arduboy_->drawTriangle(x, y - r, x - r, y + r, x + r, y + r, WHITE);
      break;
    case Boid::BoidType::kFullTriangle:
      arduboy_->fillTriangle(x, y - r, x - r, y + r, x + r, y + r, WHITE);
      break;
  }
}

