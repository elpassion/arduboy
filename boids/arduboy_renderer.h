#ifndef ARDUBOY_RENDERER_H_
#define ARDUBOY_RENDERER_H_

#include <Arduboy.h>
#include "boid.h"
#include "renderer.h"

class ArduboyRenderer : public Renderer {
  public:
    ArduboyRenderer(Arduboy* arduboy) : arduboy_(arduboy) {}
    
    void Render(const Boid& boid) const;
    
  private:
    Arduboy* arduboy_;
};

#endif
