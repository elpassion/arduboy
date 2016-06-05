#ifndef PERSONALITY_H_
#define PERSONALITY_H_

struct Personality {
  float stickiness;   // 1.0 - 1.5
  float scope;        // 600 - 2000
  float max_velocity; // 4 - 12
  float min_distance; // 50 - 150
  bool  predator;
};

#endif
