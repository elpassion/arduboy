#ifndef ENGINE_H_
#define ENGINE_H_

#include "environment.h"
#include "renderer.h"
#include "point.h"
#include "rules.h"

class Engine {
  public:
    Engine(Point bounds, byte boids_count = 10, Renderer* renderer = NULL);

    void Move();
    void EnableScatter();
    void DisableScatter();
    void Render() const;
    
  private:
    static const byte kMaxCount   = 20;
    static const byte kRulesCount = 4;
    
    Boid*       boids_[kMaxCount];
    byte        boids_count_;
    Point       bounds_;
    Renderer*   renderer_;
    Rule*       rules_[kRulesCount];
    Environment environment_ = {false};

    void  InitializeBoids();
    void  InitializeRules();
    Point RandomPosition() const;
};

#endif
