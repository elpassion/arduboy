#include "boid.h"

Boid::Boid(Point position, Point velocity, byte type, Personality personality)
  : position_(position), velocity_(velocity), type_(type), personality_(personality) {
  previous_positions_[0] = previous_positions_[1] = position_;
}

void Boid::AddVelocity(const Point& velocity) {
  velocity_ += velocity;
}

void Boid::LimitVelocity(int limit) {
  if (limit < 0)
    limit = personality_.max_velocity;

  auto magnitudeSquared = velocity_.magnitudeSquared();

  if (magnitudeSquared > limit)
    velocity_ *= sqrt(limit / magnitudeSquared);
}

void Boid::Move() {
  if (previous_positions_[0].distanceSquared(position_ + velocity_) > 16.0) {
    previous_positions_[1] = previous_positions_[0];
    previous_positions_[0] = position_;
  }
  
  position_ += velocity_;
}

Point Boid::previous_position(const int index) const {
  return previous_positions_[index];
}
