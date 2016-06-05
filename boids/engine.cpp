#include "engine.h"
#include "bitmaps.h"

Engine::Engine(Point bounds, byte boids_count, Renderer* renderer)
  : bounds_(bounds), boids_count_(boids_count), renderer_(renderer) {
  if (boids_count_ > kMaxBoidsCount)
    boids_count_ = kMaxBoidsCount;

  InitializeBoids();
  InitializeRules();
}

void Engine::InitializeBoids() {
  for (byte i = 0; i < boids_count_; i++) {
    boids_[i] = new Boid(RandomPosition(), RandomVelocity(), RandomPersonality());
  }
}

void Engine::InitializeRules() {
  rules_[0] = new CenterOfMassRule(boids_, boids_count_);
  rules_[1] = new KeepDistanceRule(boids_, boids_count_);
  rules_[2] = new MatchVelocityRule(boids_, boids_count_);
  rules_[3] = new KeepWithinBoundsRule(boids_, boids_count_, bounds_);
}

void Engine::Move() {
  for (byte i = 0; i < boids_count_; i++) {
    auto boid = boids_[i];

    for (const auto rule : rules_) {
      boid->AddVelocity(rule->Compute(*boid, environment_));
    }

    boid->LimitVelocity();
    boid->Move();
  }
}

void Engine::EnableScatter() {
  environment_.scatter = true;
}

void Engine::DisableScatter() {
  environment_.scatter = false;
}

void Engine::Render() const {
  if (!renderer_)
    return;

  for (byte i = 0; i < boids_count_; i++) {
    renderer_->Render(*boids_[i]);
  }
}

Point Engine::RandomPosition() const {
  return {random(bounds_.x), random(bounds_.y)};
}

Point Engine::RandomVelocity() const {
  return {(random() - 0.5) / 10, (random() - 0.5) / 10};
}

Personality Engine::RandomPersonality() const {
  return {
    random(10, 15) / 10,
    random(600, 2000),
    random(40, 125) / 10,
    random(50, 150),
    random(2, 32),
    random(0, sizeof(boid_bitmaps) / sizeof(boid_bitmaps[0]))
  };
}
