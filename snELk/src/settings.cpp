#include "settings.h"

Settings::Settings() :
      level(10),
      sound(true),
      showGameOverText(true) {
}

int Settings::getLevel() {
  return level;
}

void Settings::incrementLevel() {
  if (level < 20) {
    level++;
  }
}

void Settings::decrementLevel() {
  if (level > 1) {
    level--;
  }
}

bool Settings::isSound() {
  return sound;
}

void Settings::triggerSound() {
  sound = !sound;
}

bool Settings::isShowGameOverText() {
  return showGameOverText;
}

void Settings::setShowGameOverText(bool show) {
  showGameOverText = show;
}

void Settings::triggerGameOverText() {
  showGameOverText = !showGameOverText;
}
