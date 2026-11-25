#include "TextBox.h"

TextBox::TextBox(float x, float y, float width, float height, 
                 const std::string& placeholderText, bool isPassword, int maxCharacters)
    : Widget(x, y, width, height),
      placeholder(placeholderText),
      isPasswordMode(isPassword),
      maxChars(maxCharacters),
      fontSize(20),
      isFocused(false),
      cursorBlinkTimer(0.0f),
      showCursor(true),
      normalBg(Color{ 255, 255, 255, 245 }),
      normalBorder(Color{ 220, 215, 202, 255 }),      // Soft beige border
      focusedBorder(Color{ 130, 185, 240, 255 }),     // Pastel sky blue border when focused!
      textColor(Color{ 22, 32, 52, 255 }),            // Deep navy text
      placeholderColor(Color{ 150, 155, 165, 255 })
{
}

void TextBox::update() {
    if (!isVisible) return;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        isFocused = isHovered();
    }

    if (isHovered()) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }

    if (isFocused) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && ((int)text.length() < maxChars)) {
                text.push_back((char)key);
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
            if (!text.empty()) {
                text.pop_back();
            }
        }

        cursorBlinkTimer += GetFrameTime();
        if (cursorBlinkTimer >= 0.5f) {
            cursorBlinkTimer = 0.0f;
            showCursor = !showCursor;
        }
    } else {
        showCursor = false;
    }
}

void TextBox::draw() {
    if (!isVisible) return;

    Rectangle boundsRect = getBounds();
    Color borderColor = isFocused ? focusedBorder : normalBorder;
    float roundness = 0.42f;

    // Draw smooth pill rounded background
    DrawRectangleRounded(boundsRect, roundness, 16, normalBg);
    DrawRectangleRoundedLinesEx(boundsRect, roundness, 16, isFocused ? 2.5f : 1.5f, borderColor);

    std::string displayStr = text;
    if (isPasswordMode && !text.empty()) {
        displayStr = std::string(text.length(), '*');
    }

    float textPaddingX = 22.0f;

    if (usesCustomFont()) {
        Font font = getFont();

        if (text.empty() && !isFocused) {
            Vector2 pPos = { x + textPaddingX, y + (height - (float)fontSize) * 0.5f };
            DrawTextEx(font, placeholder.c_str(), pPos, (float)fontSize, 1.0f, placeholderColor);
        } else {
            Vector2 tPos = { x + textPaddingX, y + (height - (float)fontSize) * 0.5f };
            DrawTextEx(font, displayStr.c_str(), tPos, (float)fontSize, 1.0f, textColor);

            if (isFocused && showCursor) {
                Vector2 textSize = MeasureTextEx(font, displayStr.c_str(), (float)fontSize, 1.0f);
                float cursorX = x + textPaddingX + textSize.x + 3.0f;
                DrawLineEx(
                    Vector2{ cursorX, y + 14.0f },
                    Vector2{ cursorX, y + height - 14.0f },
                    2.0f,
                    textColor
                );
            }
        }
    } else {
        if (text.empty() && !isFocused) {
            DrawText(placeholder.c_str(), (int)(x + textPaddingX), (int)(y + (height - fontSize) * 0.5f), fontSize, placeholderColor);
        } else {
            DrawText(displayStr.c_str(), (int)(x + textPaddingX), (int)(y + (height - fontSize) * 0.5f), fontSize, textColor);

            if (isFocused && showCursor) {
                int textW = MeasureText(displayStr.c_str(), fontSize);
                int cursorX = (int)(x + textPaddingX + textW + 2);
                DrawLine(cursorX, (int)(y + 14), cursorX, (int)(y + height - 14), textColor);
            }
        }
    }
}
