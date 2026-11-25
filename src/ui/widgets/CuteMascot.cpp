#include "CuteMascot.h"
#include <cmath>

CuteMascot::CuteMascot(float x, float y, float width, float height, bool peekFromBottom, Color bodyColor)
    : Widget(x, y, width, height),
      bodyColor(bodyColor),
      eyeColor(Color{ 24, 34, 54, 255 }),      // Deep cute navy
      accentColor(Color{ 130, 185, 240, 255 }), // Pastel blue accent
      peekFromBottom(peekFromBottom),
      blinkTimer(0.0f),
      nextBlinkTime(3.0f),
      isBlinking(false),
      blinkProgress(0.0f),
      eyeOpenRatio(1.0f),
      pupilOffset(Vector2{ 0.0f, 0.0f })
{
}

void CuteMascot::update() {
    if (!isVisible) return;

    float dt = GetFrameTime();
    blinkTimer += dt;

    // Trigger blink
    if (!isBlinking && blinkTimer >= nextBlinkTime) {
        isBlinking = true;
        blinkProgress = 0.0f;
    }

    if (isBlinking) {
        blinkProgress += dt * 7.5f; // Quick cute blink (~0.13 seconds)
        if (blinkProgress >= 1.0f) {
            isBlinking = false;
            blinkTimer = 0.0f;
            // Blink randomly every 2.5 to 4.5 seconds
            nextBlinkTime = 2.5f + (float)GetRandomValue(0, 200) / 100.0f;
            eyeOpenRatio = 1.0f;
        } else {
            // Smooth triangle wave: 1 -> 0 -> 1
            if (blinkProgress < 0.5f) {
                eyeOpenRatio = 1.0f - (blinkProgress * 2.0f);
            } else {
                eyeOpenRatio = (blinkProgress - 0.5f) * 2.0f;
            }
            if (eyeOpenRatio < 0.06f) eyeOpenRatio = 0.06f;
        }
    } else {
        eyeOpenRatio = 1.0f;
    }

    // Pupils smoothly follow mouse position slightly
    Vector2 mouse = GetMousePosition();
    float centerX = x + width * 0.5f;
    float centerY = y + height * 0.5f;

    float targetDx = (mouse.x - centerX) * 0.025f;
    float targetDy = (mouse.y - centerY) * 0.025f;

    // Clamp pupil offset so it stays naturally inside the eye
    if (targetDx > 7.0f) targetDx = 7.0f;
    if (targetDx < -7.0f) targetDx = -7.0f;
    if (targetDy > 6.0f) targetDy = 6.0f;
    if (targetDy < -6.0f) targetDy = -6.0f;

    pupilOffset.x += (targetDx - pupilOffset.x) * 12.0f * dt;
    pupilOffset.y += (targetDy - pupilOffset.y) * 12.0f * dt;
}

void CuteMascot::draw() {
    if (!isVisible) return;

    float centerX = x + width * 0.5f;

    if (peekFromBottom) {
        // --- Peeking from bottom (Login Screen) ---
        // Body (rounded cute top emerging from bottom)
        Rectangle bodyRect = { x, y, width, height + 80.0f };
        DrawRectangleRounded(bodyRect, 0.42f, 24, bodyColor);

        // Cute top sprout / heart leaf on head
        DrawCircle((int)centerX - 6, (int)y - 4, 9.0f, WHITE);
        DrawCircle((int)centerX + 6, (int)y - 4, 9.0f, WHITE);
        DrawCircle((int)centerX - 6, (int)y - 4, 7.0f, bodyColor);
        DrawCircle((int)centerX + 6, (int)y - 4, 7.0f, bodyColor);

        // Eyeballs
        float eyeY = y + 68.0f;
        float eyeSpacing = 46.0f;
        float leftEyeX = centerX - eyeSpacing;
        float rightEyeX = centerX + eyeSpacing;
        float eyeRadiusX = 30.0f;
        float eyeRadiusY = 34.0f;

        // Draw Eyebrows (slight curved arc)
        DrawRing(Vector2{ leftEyeX, eyeY - 28.0f }, 12.0f, 15.0f, 200.0f, 340.0f, 16, eyeColor);
        DrawRing(Vector2{ rightEyeX, eyeY - 28.0f }, 12.0f, 15.0f, 200.0f, 340.0f, 16, eyeColor);

        if (eyeOpenRatio > 0.22f) {
            // --- Eyes Open ---
            // Eye whites
            DrawEllipse((int)leftEyeX, (int)eyeY, eyeRadiusX, eyeRadiusY * eyeOpenRatio, WHITE);
            DrawEllipse((int)rightEyeX, (int)eyeY, eyeRadiusX, eyeRadiusY * eyeOpenRatio, WHITE);

            // Pupils (dark navy)
            float pupilRadius = 16.0f * eyeOpenRatio;
            if (pupilRadius > 15.0f) pupilRadius = 15.0f;

            DrawCircle((int)(leftEyeX + pupilOffset.x), (int)(eyeY + pupilOffset.y), pupilRadius, eyeColor);
            DrawCircle((int)(rightEyeX + pupilOffset.x), (int)(eyeY + pupilOffset.y), pupilRadius, eyeColor);

            // Cute white catchlights / sparkles (2 per eye)
            DrawCircle((int)(leftEyeX + pupilOffset.x - 5), (int)(eyeY + pupilOffset.y - 5), 5.0f, WHITE);
            DrawCircle((int)(leftEyeX + pupilOffset.x + 4), (int)(eyeY + pupilOffset.y + 4), 2.5f, WHITE);

            DrawCircle((int)(rightEyeX + pupilOffset.x - 5), (int)(eyeY + pupilOffset.y - 5), 5.0f, WHITE);
            DrawCircle((int)(rightEyeX + pupilOffset.x + 4), (int)(eyeY + pupilOffset.y + 4), 2.5f, WHITE);
        } else {
            // --- Eyes Closed / Blinking (Happy curved arcs) ---
            DrawRing(Vector2{ leftEyeX, eyeY }, 14.0f, 18.0f, 20.0f, 160.0f, 16, eyeColor);
            DrawRing(Vector2{ rightEyeX, eyeY }, 14.0f, 18.0f, 20.0f, 160.0f, 16, eyeColor);
        }

        // Cute tiny nostril dots
        float noseY = eyeY + 36.0f;
        DrawCircle((int)centerX - 6, (int)noseY, 2.5f, eyeColor);
        DrawCircle((int)centerX + 6, (int)noseY, 2.5f, eyeColor);

    } else {
        // --- Hanging from Top (Sign Up Screen) ---
        // Body (rounded bottom peeking down from top)
        Rectangle bodyRect = { x, y - 60.0f, width, height + 60.0f };
        DrawRectangleRounded(bodyRect, 0.42f, 24, bodyColor);

        // Cute sprout on top (if visible)
        DrawCircle((int)centerX - 6, (int)y + 8, 8.0f, WHITE);
        DrawCircle((int)centerX + 6, (int)y + 8, 8.0f, WHITE);

        // Eyeballs
        float eyeY = y + height * 0.52f;
        float eyeSpacing = 48.0f;
        float leftEyeX = centerX - eyeSpacing;
        float rightEyeX = centerX + eyeSpacing;
        float eyeRadiusX = 32.0f;
        float eyeRadiusY = 36.0f;

        // Eyebrows
        DrawRing(Vector2{ leftEyeX, eyeY - 30.0f }, 12.0f, 15.0f, 200.0f, 340.0f, 16, eyeColor);
        DrawRing(Vector2{ rightEyeX, eyeY - 30.0f }, 12.0f, 15.0f, 200.0f, 340.0f, 16, eyeColor);

        if (eyeOpenRatio > 0.22f) {
            // Eye whites
            DrawEllipse((int)leftEyeX, (int)eyeY, eyeRadiusX, eyeRadiusY * eyeOpenRatio, WHITE);
            DrawEllipse((int)rightEyeX, (int)eyeY, eyeRadiusX, eyeRadiusY * eyeOpenRatio, WHITE);

            // Pupils
            float pupilRadius = 17.0f * eyeOpenRatio;
            if (pupilRadius > 16.0f) pupilRadius = 16.0f;

            DrawCircle((int)(leftEyeX + pupilOffset.x), (int)(eyeY + pupilOffset.y), pupilRadius, eyeColor);
            DrawCircle((int)(rightEyeX + pupilOffset.x), (int)(eyeY + pupilOffset.y), pupilRadius, eyeColor);

            // Sparkles
            DrawCircle((int)(leftEyeX + pupilOffset.x - 6), (int)(eyeY + pupilOffset.y - 6), 5.5f, WHITE);
            DrawCircle((int)(leftEyeX + pupilOffset.x + 5), (int)(eyeY + pupilOffset.y + 4), 2.8f, WHITE);

            DrawCircle((int)(rightEyeX + pupilOffset.x - 6), (int)(eyeY + pupilOffset.y - 6), 5.5f, WHITE);
            DrawCircle((int)(rightEyeX + pupilOffset.x + 5), (int)(eyeY + pupilOffset.y + 4), 2.8f, WHITE);
        } else {
            // Blinking arcs
            DrawRing(Vector2{ leftEyeX, eyeY }, 15.0f, 19.0f, 20.0f, 160.0f, 16, eyeColor);
            DrawRing(Vector2{ rightEyeX, eyeY }, 15.0f, 19.0f, 20.0f, 160.0f, 16, eyeColor);
        }

        // Tiny nostrils
        float noseY = eyeY + 38.0f;
        DrawCircle((int)centerX - 6, (int)noseY, 2.5f, eyeColor);
        DrawCircle((int)centerX + 6, (int)noseY, 2.5f, eyeColor);
    }
}
