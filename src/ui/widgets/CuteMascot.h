#pragma once
#include "Widget.h"

class CuteMascot : public Widget {
private:
    Color bodyColor;
    Color eyeColor;
    Color accentColor;
    bool peekFromBottom;

    float blinkTimer;
    float nextBlinkTime;
    bool isBlinking;
    float blinkProgress;
    float eyeOpenRatio;

    Vector2 pupilOffset;

public:
    CuteMascot(float x, float y, float width, float height, 
               bool peekFromBottom = true, 
               Color bodyColor = Color{ 255, 236, 140, 255 });

    void update() override;
    void draw() override;

    void setBodyColor(Color c) { bodyColor = c; }
    void setPeekFromBottom(bool peek) { peekFromBottom = peek; }
};
