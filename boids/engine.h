#ifndef ENGINE_H_
#define ENGINE_H_

#include "environment.h"
#include "renderer.h"
#include "personality.h"
#include "point.h"
#include "rules.h"

class Engine {
  public:
    Engine(Point bounds, Renderer& renderer, byte boids_count = 10);

    void Move();
    void EnableScatter();
    void DisableScatter();
    void Render() const;

  private:
    static const byte kMaxBoidsCount = 20;
    static const byte kRulesCount    = 5;

    Boid*       boids_[kMaxBoidsCount];
    byte        boids_count_;
    Point       bounds_;
    Renderer&   renderer_;
    Rule*       rules_[kRulesCount];
    Environment environment_ = {false};

    void        InitializeBoids();
    void        InitializeRules();
    Point       RandomPosition() const;
    Point       RandomVelocity() const;
    Personality RandomPersonality() const;
};

#endif
