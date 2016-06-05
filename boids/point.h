#ifndef POINT_H_
#define POINT_H_

struct Point {
  float x, y;

  Point operator +(const Point& other) const {
    return {x + other.x, y + other.y};
  }

  void operator +=(const Point& other) {
    x += other.x;
    y += other.y;
  }

  Point operator -(const Point& other) const {
    return {x - other.x, y - other.y};
  }

  void operator -=(const Point& other) {
    x -= other.x;
    y -= other.y;
  }

  Point operator *(const float factor) const {
    return {x * factor, y * factor};
  }

  void operator *=(const float factor) {
    x *= factor;
    y *= factor;
  }

  Point operator /(const float divisor) const {
    return {x / divisor, y / divisor};
  }

  void operator /=(const float divisor) {
    x /= divisor;
    y /= divisor;
  }

  // For distance comparisons, we do not need exact distance.
  // For optimisation purposes, we can use squared distance,
  // so we can avoid expensive sqrt call.
  float distanceSquared(const Point& other) const {
    return (other.x - x) * (other.x - x) + (other.y - y) * (other.y - y);
  }

  float magnitudeSquared() const {
    return (x * x) + (y * y);
  }
};

#endif
