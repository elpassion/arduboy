#include "boid.h"

Boid::BoidType Boid::RandomType() {
  return static_cast<BoidType>(rand() % (BoidType_MAX + 1));
}

void Boid::MoveBy(const Point& delta) {
  position_.MoveBy(delta);
}

void Boid::MoveBy(const Point& delta, const Point& limit_bottom_right) {
  MoveBy(delta, {0, 0}, limit_bottom_right);
}

void Boid::MoveBy(const Point& delta, const Point& limit_top_left, const Point& limit_bottom_right) {
  position_.MoveBy(delta, limit_top_left, limit_bottom_right);
}

