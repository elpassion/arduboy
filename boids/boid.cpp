#include "boid.h"

Boid::BoidType Boid::RandomType() {
  return static_cast<BoidType>(rand() % (BoidType_MAX + 1));
}

Boid::Boid(Point position, Point velocity, BoidType type, Personality personality) 
  : position_(position), velocity_(velocity), new_velocity_(velocity), type_(type), personality_(personality) {
  previous_positions_[0] = previous_positions_[1] = previous_positions_[2] = position;
}

void Boid::AddVelocity(const Point& velocity) {
  new_velocity_ += velocity;
}

void Boid::LimitVelocity(int limit) {
  if (limit < 0) {
    limit = personality_.max_velocity;
  }
  
  auto magnitude = new_velocity_.magnitude();
  
  if (magnitude > limit) {
    new_velocity_ *= limit / magnitude;
  }
}

void Boid::Move() {
  if (previous_positions_[0].distance(position_ + new_velocity_) >= 3.0) {
    previous_positions_[2] = previous_positions_[1];
    previous_positions_[1] = previous_positions_[0];
    previous_positions_[0] = position_;
  }

  velocity_ = new_velocity_;
  position_.MoveBy(velocity_);
}

Point Boid::previous_position(const int index) const {
  return previous_positions_[index];
}

