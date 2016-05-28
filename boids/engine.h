#ifndef ENGINE_H_
#define ENGINE_H_

#include "renderer.h"
#include "point.h"

class Engine {
  public:
    Engine(Point dimensions, byte boids_count = 10, Renderer* renderer = NULL);

    void Move();
    void Render() const;
    
  private:
    static const byte kMaxCount = 30;
    
    Boid*     boids_[kMaxCount];
    byte      boids_count_;
    Point     dimensions_;
    Renderer* renderer_;

    void  InitializeBoids();
    Point RandomPosition() const;
};

#endif
