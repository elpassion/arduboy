#include "rules.h"

Point CenterOfMassRule::Compute(const Boid& boid, const Environment& environment) const {
  Point velocity = {0, 0};
  byte  count    = 0;

  for (byte j = 0; j < boids_count_; j++) {
    if (boids_[j] != &boid && boids_[j]->position().distanceSquared(boid.position()) <= boid.personality().scope) {
      count++;
      velocity += boids_[j]->position();
    }
  }

  return count > 0 ? (((velocity / count) - boid.position()) * boid.personality().stickiness / (environment.scatter ? -100.0 : 100.0)) : velocity;
}

Point KeepDistanceRule::Compute(const Boid& boid, const Environment& environment) const {
  Point velocity = {0, 0};

  for (byte j = 0; j < boids_count_; j++) {
    if (boids_[j] != &boid && boids_[j]->position().distanceSquared(boid.position()) <= boid.personality().scope) {
      if (boids_[j]->position().distanceSquared(boid.position()) < boid.personality().min_distance) {
        velocity -= boids_[j]->position() - boid.position();
      }
    }
  }

  return velocity;
}

Point MatchVelocityRule::Compute(const Boid& boid, const Environment& environment) const {
  Point velocity = {0, 0};
  byte  count    = 0;

  for (byte j = 0; j < boids_count_; j++) {
    if (boids_[j] != &boid && boids_[j]->position().distanceSquared(boid.position()) <= boid.personality().scope) {
      count++;
      velocity += boids_[j]->velocity();
    }
  }

  return count > 0 ? (((velocity / count) - boid.velocity()) / 8.0) : velocity;
}

Point LimitVelocityRule::Compute(const Boid& boid, const Environment& environment) const {
  for (byte j = 0; j < boids_count_; j++) {
  auto magnitudeSquared = boids_[j]->velocity().magnitudeSquared();

  if (magnitudeSquared > boids_[j]->personality().max_velocity)
    boids_[j]->MultiplyVelocity(sqrt(boids_[j]->personality().max_velocity / magnitudeSquared));
  }

  return {0, 0};
}

Point KeepWithinBoundsRule::Compute(const Boid& boid, const Environment& environment) const {
  Point velocity = {0, 0};

  if (boid.position().x < 2.5) {
    velocity.x = 1.5;
  } else if (boid.position().x >= bounds_.x - 2.5) {
    velocity.x = -1.5;
  }

  if (boid.position().y < 2.5) {
    velocity.y = 1.5;
  } else if (boid.position().y >= bounds_.y - 2.5) {
    velocity.y = -1.5;
  }

  return velocity;
}
