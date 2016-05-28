#include "engine.h"

Engine::Engine(Point dimensions, byte boids_count, Renderer* renderer) 
  : dimensions_(dimensions), boids_count_(boids_count), renderer_(renderer) { 
  InitializeBoids();
}

void Engine::InitializeBoids() {
  for (byte i = 0; i < boids_count_; i++) {
    boids_[i] = new Boid(RandomPosition(), {0, 0}, Boid::RandomType(), random(1, 11));
  }
}

void Engine::Move() {
  for (byte i = 0; i < boids_count_; i++) {
    boids_[i]->MoveBy({random(3) - 1, random(3) - 1}, dimensions_);
  }
}

void Engine::Render() const {
  if (!renderer_)
    return;

  for (byte i = 0; i < boids_count_; i++) {
    renderer_->Render(*boids_[i]);
  }
}

Point Engine::RandomPosition() const {
  return {random(dimensions_.x), random(dimensions_.y)};
}

