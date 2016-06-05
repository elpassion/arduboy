#ifndef PERSONALITY_H_
#define PERSONALITY_H_

struct Personality {
  float stickiness;   // 1.0 - 1.5
  int   scope;        // 600 - 2000
  float max_velocity; // 4 - 12
  int   min_distance; // 50 - 150
  int   tail_length;  // 2 - 32
  byte  shape;
};

#endif
