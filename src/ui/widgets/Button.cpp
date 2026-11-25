#include "Button.h"

Button::Button(float x, float y, float width, float height, const std::string& label, ButtonStyle btnStyle)
    : Widget(x, y, width, height),
      text(label),
      fontSize(22),
      wasClicked(false),
      currentScale(1.0f),
      targetScale(1.0f)
{
    setStyle(btnStyle);
}

void Button::setStyle(ButtonStyle btnStyle) {
    style = btnStyle;

    if (style == STYLE_PRIMARY_DARK) {
        // Deep cute navy
        bgColor     = Color{ 22, 32, 52, 255 };
        hoverColor  = Color{ 36, 48, 72, 255 };
        textColor   = WHITE;
        borderColor = BLANK;
    } 
    else if (style == STYLE_CUTE_YELLOW) {
        // Light butter yellow
        bgColor     = Color{ 255, 235, 135, 255 };
        hoverColor  = Color{ 255, 242, 165, 255 };
        textColor   = Color{ 22, 32, 52, 255 };
        borderColor = BLANK;
    }
    else if (style == STYLE_OUTLINE_BEIGE) {
        // Cream with soft pastel border
        bgColor     = Color{ 255, 255, 255, 240 };
        hoverColor  = Color{ 250, 246, 236, 255 };
        textColor   = Color{ 22, 32, 52, 255 };
        borderColor = Color{ 215, 205, 190, 255 };
    }
    else if (style == STYLE_SOFT_BLUE) {
        // Pastel sky blue
        bgColor     = Color{ 142, 195, 245, 255 };
        hoverColor  = Color{ 165, 210, 252, 255 };
        textColor   = Color{ 22, 32, 52, 255 };
        borderColor = BLANK;
    }
    else if (style == STYLE_OUTLINE_LIME) {
        bgColor     = Color{ 255, 255, 255, 240 };
        hoverColor  = Color{ 250, 246, 236, 255 };
        textColor   = Color{ 22, 32, 52, 255 };
        borderColor = Color{ 255, 230, 120, 255 };
    } 
    else if (style == STYLE_SOLID_LIME) {
        bgColor     = Color{ 255, 235, 135, 255 };
        hoverColor  = Color{ 255, 242, 165, 255 };
        textColor   = Color{ 22, 32, 52, 255 };
        borderColor = BLANK;
    }
}

void Button::update() {
    if (!isVisible) {
        wasClicked = false;
        return;
    }

    bool hovered = isHovered();

    if (hovered) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        targetScale = IsMouseButtonDown(MOUSE_BUTTON_LEFT) ? 0.96f : 1.03f;

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            wasClicked = true;
            targetScale = 1.03f;
        } else {
            wasClicked = false;
        }
    } else {
        targetScale = 1.0f;
        wasClicked = false;
    }

    float speed = 18.0f * GetFrameTime();
    currentScale += (targetScale - currentScale) * speed;
}

void Button::draw() {
    if (!isVisible) return;

    float scaledW = width * currentScale;
    float scaledH = height * currentScale;
    float scaledX = x - (scaledW - width) * 0.5f;
    float scaledY = y - (scaledH - height) * 0.5f;

    Rectangle drawBounds = { scaledX, scaledY, scaledW, scaledH };
    Color currentBg = isHovered() ? hoverColor : bgColor;

    // Draw pill rounded button
    float roundness = 0.5f;
    DrawRectangleRounded(drawBounds, roundness, 20, currentBg);

    if (borderColor.a > 0) {
        DrawRectangleRoundedLinesEx(drawBounds, roundness, 20, 2.0f, borderColor);
    }

    if (usesCustomFont()) {
        Font font = getFont();
        Vector2 textSize = MeasureTextEx(font, text.c_str(), (float)fontSize, 1.0f);
        Vector2 textPos = {
            scaledX + (scaledW - textSize.x) * 0.5f,
            scaledY + (scaledH - textSize.y) * 0.5f
        };
        DrawTextEx(font, text.c_str(), textPos, (float)fontSize, 1.0f, textColor);
    } else {
        int textW = MeasureText(text.c_str(), fontSize);
        int textX = (int)(scaledX + (scaledW - textW) * 0.5f);
        int textY = (int)(scaledY + (scaledH - fontSize) * 0.5f);
        DrawText(text.c_str(), textX, textY, fontSize, textColor);
    }
}
