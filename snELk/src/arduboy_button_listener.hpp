#ifndef arduboy_button_listener_hpp
#define arduboy_button_listener_hpp

#include <arduboy.h>

class ButtonListener {
  public:
    ButtonListener();
    void onAButtonClick(uint8_t buttons, void (*callback)());
    void onBButtonClick(uint8_t buttons, void (*callback)());
    void onUpButtonClick(uint8_t buttons, void (*callback)());
    void onDownButtonClick(uint8_t buttons, void (*callback)());

  private:
    bool a;
    bool b;
    bool up;
    bool down;
};

ButtonListener::ButtonListener() :
      a(false),
      b(false),
      up(false),
      down(false) {
}

void ButtonListener::onAButtonClick(uint8_t buttons, void (*callback)()) {
  if(buttons & A_BUTTON) {
    if (!a) {
      (*callback)();
      a = true;
    }
  } else {
    a = false;
  }
}

void ButtonListener::onBButtonClick(uint8_t buttons, void (*callback)()) {
  if(buttons & B_BUTTON) {
    if (!b) {
      (*callback)();
      b = true;
    }
  } else {
    b = false;
  }
}

void ButtonListener::onUpButtonClick(uint8_t buttons, void (*callback)()) {
  if(buttons & UP_BUTTON) {
    if (!up) {
      (*callback)();
      up = true;
    }
  } else {
    up = false;
  }
}

void ButtonListener::onDownButtonClick(uint8_t buttons, void (*callback)()) {
  if(buttons & DOWN_BUTTON) {
    if (!down) {
      (*callback)();
      down = true;
    }
  } else {
    down = false;
  }
}

#endif
