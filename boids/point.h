#ifndef POINT_H_
#define POINT_H_

struct Point {
  double x, y;

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

  Point operator *(const double factor) const {
    return {x * factor, y * factor};
  }

  void operator *=(const double factor) {
    x *= factor;
    y *= factor;
  }

  Point operator /(const double divisor) const {
    return {x / divisor, y / divisor};
  }

  void operator /=(const double divisor) {
    x /= divisor;
    y /= divisor;
  }

  double distance(const Point& other) const {
    return sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y));
  }

  double magnitude() const {
    return sqrt((x * x) + (y * y));
  }
  
  void MoveBy(const Point& point) {
    x += point.x;
    y += point.y;
  }
};

#endif

