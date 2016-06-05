#ifndef BITMAPS_H_
#define BITMAPS_H_

// Bitmaps are rotated 90 degrees CCW
PROGMEM const unsigned char boid_bitmaps[][3] = {
  {B111, B111, B111}, // Full Square
  {B111, B101, B111}, // Empty Square
  {B010, B111, B010}, // Full Circle
  {B010, B101, B010}, // Empty Circle
  {B101, B010, B101}, // Full Star
  {B101, B000, B101}  // Empty Star
};

#endif
