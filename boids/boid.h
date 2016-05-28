#ifndef BOID_H_
#define BOID_H_

#include <Arduboy.h>
#include "point.h"

class Boid {
  public:
    enum BoidType { kEmptyCircle, kFullCircle, kEmptyTriangle, kFullTriangle, BoidType_MAX = kFullTriangle };
  
    Boid(Point position = {0, 0}, Point velocity = {0, 0}, BoidType type = kFullCircle, byte size = 1) 
      : position_(position), velocity_(velocity), type_(type), size_(size) {}

    static BoidType RandomType();
    
    void MoveBy(const Point& delta);
    void MoveBy(const Point& delta, const Point& limit_bottom_right);
    void MoveBy(const Point& delta, const Point& limit_top_left, const Point& limit_bottom_right);

    Point    position() const { return position_; }
    Point    velocity() const { return velocity_; }
    BoidType type()     const { return type_; }
    byte     size()     const { return size_; }
    
  private:
    Point    position_;
    Point    velocity_;
    BoidType type_;
    byte     size_;
};

#endif
