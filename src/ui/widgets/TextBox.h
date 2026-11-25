#pragma once
#include "Widget.h"
#include <string>

class TextBox : public Widget {
private:
    std::string text;
    std::string placeholder;
    bool isFocused;
    bool isPasswordMode;
    int maxChars;
    int fontSize;

    float cursorBlinkTimer;
    bool showCursor;

    Color normalBg;
    Color focusedBorder;
    Color normalBorder;
    Color textColor;
    Color placeholderColor;

public:
    TextBox(float x, float y, float width, float height, 
            const std::string& placeholderText = "", 
            bool isPassword = false, 
            int maxCharacters = 25);

    void update() override;
    void draw() override;

    std::string getText() const { return text; }
    void setText(const std::string& newText) { text = newText; }
    void clear() { text.clear(); }
    bool getIsFocused() const { return isFocused; }
    void setFocused(bool focused) { isFocused = focused; }
};
