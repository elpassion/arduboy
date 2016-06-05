#include "boid.h"

Boid::Boid(Point position, Point velocity, Personality personality)
  : position_(position), velocity_(velocity), personality_(personality) {
  previous_positions_[0] = previous_positions_[1] = position_;
}

Point Boid::position() const {
  return position_;
}

Point Boid::velocity() const {
  return velocity_;
}

Personality Boid::personality() const {
  return personality_;
}

Point Boid::previous_position(const int& index) const {
  return previous_positions_[index];
}

void Boid::AddVelocity(const Point& velocity) {
  velocity_ += velocity;
}

void Boid::MultiplyVelocity(const float& factor) {
  velocity_ *= factor;
}

void Boid::Move() {
  if (previous_positions_[0].distanceSquared(position_ + velocity_) > personality_.tail_length) {
    previous_positions_[1] = previous_positions_[0];
    previous_positions_[0] = position_;
  }

  position_ += velocity_;
}
