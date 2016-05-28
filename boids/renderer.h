#ifndef RENDERER_H_
#define RENDERER_H_

#include "boid.h"

class Renderer {
  public:
    virtual void Render(const Boid& boid) const = 0;
};

#endif
