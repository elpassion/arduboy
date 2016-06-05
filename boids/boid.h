#ifndef BOID_H_
#define BOID_H_

#include <Arduboy.h>
#include "personality.h"
#include "point.h"

class Boid {
  public:
    Boid(Point position = {0, 0}, Point velocity = {0, 0}, Personality personality = {1, 100, 3.0, 25.0});

    void AddVelocity(const Point& velocity);
    void MultiplyVelocity(const float& factor);
    void Move();

    Point       position()    const;
    Point       velocity()    const;
    Personality personality() const;
    Point       previous_position(const int& index) const;

  private:
    Point       position_;
    Point       velocity_;
    Point       previous_positions_[2];
    Personality personality_;
};

#endif
