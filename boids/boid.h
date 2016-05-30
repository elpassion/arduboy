#ifndef BOID_H_
#define BOID_H_

#include <Arduboy.h>
#include "personality.h"
#include "point.h"

class Boid {
  public:
    enum BoidType { kEmptyCircle, kFullCircle, kEmptyTriangle, kFullTriangle, BoidType_MAX = kFullTriangle };

    Boid(Point position = {1, 0}, Point velocity = {0, 0}, BoidType type = kFullCircle, Personality personality = {1.0, 3.0, 5.0});

    static BoidType RandomType();

    void AddVelocity(const Point& velocity);
    void LimitVelocity(int limit = -1);
    void Move();

    Point       position()    const { return position_; }
    Point       velocity()    const { return velocity_; }
    BoidType    type()        const { return type_; }
    Personality personality() const { return personality_; }

    Point    previous_position(const int index) const;

  private:
    Point       position_;
    Point       velocity_;
    Point       new_velocity_;
    BoidType    type_;
    Point       previous_positions_[3];
    Personality personality_;
};

#endif
