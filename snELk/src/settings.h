#ifndef settings_h
#define settings_h

class Settings {
  public:
    Settings();

    int getLevel();
    void incrementLevel();
    void decrementLevel();

    bool isSound();
    void triggerSound();

    bool isShowGameOverText();
    void setShowGameOverText(bool show);
    void triggerGameOverText();

  private:
    int level;
    bool sound;
    bool showGameOverText;
};

#endif
