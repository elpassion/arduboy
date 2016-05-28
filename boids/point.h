#ifndef POINT_H_
#define POINT_H_

struct Point {
  int x, y;

  void MoveBy(const Point& point) {
    x += point.x;
    y += point.y;
  }

  void MoveBy(const Point& delta, const Point& limit_top_left, const Point& limit_bottom_right) {
    MoveBy(delta);
    
    if (x < limit_top_left.x) {
      x = limit_bottom_right.x - 1;
    } else if (x >= limit_bottom_right.x) {
      x = limit_top_left.x;
    }

    if (y < limit_top_left.y) {
      y = limit_bottom_right.y - 1;
    } else if (y >= limit_bottom_right.y) {
      y = limit_top_left.y;
    }
  }

  void MoveTo(const Point& point) {
    x = point.x;
    y = point.y;
  }
};

#endif

