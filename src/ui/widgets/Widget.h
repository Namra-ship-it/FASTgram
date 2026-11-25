#pragma once

#if defined(_WIN32)
    #define NOGDI
    #define NOUSER
#endif

#include "raylib.h"

class Widget {
protected:
    float x;
    float y;
    float width;
    float height;
    bool isVisible;

    inline static Font sharedFont = { 0 };
    inline static bool hasCustomFont = false;

public:
    Widget(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height), isVisible(true) {}

    virtual ~Widget() = default;

    virtual void draw() = 0;
    virtual void update() = 0;

    static void setFont(Font f) {
        sharedFont = f;
        hasCustomFont = true;
    }

    static Font getFont() { return sharedFont; }
    static bool usesCustomFont() { return hasCustomFont; }

    bool isHovered() const {
        return CheckCollisionPointRec(GetMousePosition(), { x, y, width, height });
    }

    void setPosition(float newX, float newY) { x = newX; y = newY; }
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    Rectangle getBounds() const { return { x, y, width, height }; }

    void setVisible(bool visible) { isVisible = visible; }
    bool getVisible() const { return isVisible; }
};
