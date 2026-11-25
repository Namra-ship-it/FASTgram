#pragma once
#include "Widget.h"
#include <string>

enum ButtonStyle {
    STYLE_PRIMARY_DARK,    // Deep Cute Navy
    STYLE_CUTE_YELLOW,     // Light Butter Yellow
    STYLE_OUTLINE_BEIGE,   // Cream with subtle outline
    STYLE_SOFT_BLUE,       // Pastel Sky Blue
    STYLE_OUTLINE_LIME,
    STYLE_SOLID_LIME
};

class Button : public Widget {
private:
    std::string text;
    ButtonStyle style;
    int fontSize;
    bool wasClicked;

    Color bgColor;
    Color hoverColor;
    Color textColor;
    Color borderColor;

    float currentScale;
    float targetScale;

public:
    Button(float x, float y, float width, float height, const std::string& label, ButtonStyle style = STYLE_PRIMARY_DARK);

    void update() override;
    void draw() override;

    bool isClicked() const { return wasClicked; }
    void setText(const std::string& newText) { text = newText; }
    void setStyle(ButtonStyle newStyle);
};
