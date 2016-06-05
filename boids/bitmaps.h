#ifndef BITMAPS_H_
#define BITMAPS_H_

// Bitmaps are rotated 90 degrees CCW
PROGMEM const unsigned char boid_bitmaps[][3] = {
  {0b111, 0b111, 0b111}, // Full Square
  {0b111, 0b101, 0b111}, // Empty Square
  {0b010, 0b111, 0b010}, // Full Circle
  {0b010, 0b101, 0b010}, // Empty Circle
  {0b101, 0b010, 0b101}  // Star
};

#endif
