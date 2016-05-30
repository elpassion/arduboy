#include "rules.h"

Point CenterOfMassRule::Compute(const Boid& boid, const Environment& environment) const {
  Point velocity = {0, 0};

  for (byte j = 0; j < boids_count_; j++) {
    if (boids_[j] != &boid) {
      velocity += boids_[j]->position();
    }
  }

  return ((velocity / (boids_count_ - 1)) - boid.position()) * boid.personality().stickiness / (environment.scatter ? -100.0 : 100.0);
}

Point KeepDistanceRule::Compute(const Boid& boid, const Environment& environment) const {
  Point velocity = {0, 0};

  for (byte j = 0; j < boids_count_; j++) {
    if (boids_[j] != &boid) {
      if (boids_[j]->position().distance(boid.position()) < boid.personality().min_distance) {
        velocity -= boids_[j]->position() - boid.position();
      }
    }
  }

  return velocity;
}

Point MatchVelocityRule::Compute(const Boid& boid, const Environment& environment) const {
  Point velocity = {0, 0};

  for (byte j = 0; j < boids_count_; j++) {
    if (boids_[j] != &boid) {
      velocity += boids_[j]->velocity();
    }
  }

  return ((velocity / (boids_count_ - 1)) - boid.velocity()) / 8.0;
}

Point KeepWithinBoundsRule::Compute(const Boid& boid, const Environment& environment) const {
  Point velocity = {0, 0};

  if (boid.position().x < 2.5) {
    velocity.x = 2.5;
  } else if (boid.position().x >= bounds_.x - 2.5) {
    velocity.x = -2.5;
  }

  if (boid.position().y < 2.5) {
    velocity.y = 2.5;
  } else if (boid.position().y >= bounds_.y - 2.5) {
    velocity.y = -2.5;
  }

  return velocity;
}

