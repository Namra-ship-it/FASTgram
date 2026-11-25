#include "SettingsScreen.h"

SettingsScreen::SettingsScreen()
    : backBtn(25.0f, 25.0f, 80.0f, 36.0f, "← Back", STYLE_OUTLINE_BEIGE),
      themeToggleBtn(70.0f, 250.0f, 340.0f, 48.0f, "Theme: Cute Pastel", STYLE_CUTE_YELLOW),
      logoutBtn(70.0f, 540.0f, 340.0f, 48.0f, "Log Out", STYLE_PRIMARY_DARK),
      isDarkMode(false)
{
}

void SettingsScreen::update(bool& shouldGoBack, bool& shouldLogout) {
    backBtn.update();
    themeToggleBtn.update();
    logoutBtn.update();

    if (backBtn.isClicked()) {
        shouldGoBack = true;
    }

    if (logoutBtn.isClicked()) {
        shouldLogout = true;
    }

    if (themeToggleBtn.isClicked()) {
        isDarkMode = !isDarkMode;
        if (isDarkMode) {
            themeToggleBtn.setText("Theme: Dark Mode");
            themeToggleBtn.setStyle(STYLE_PRIMARY_DARK);
        } else {
            themeToggleBtn.setText("Theme: Cute Pastel");
            themeToggleBtn.setStyle(STYLE_CUTE_YELLOW);
        }
    }
}

void SettingsScreen::draw() {
    Font font = Widget::getFont();
    Color darkNavy = Color{ 22, 32, 52, 255 };
    Color warmGray = Color{ 110, 115, 125, 255 };

    backBtn.draw();

    Vector2 titleSize = MeasureTextEx(font, "Settings", 34, 1.0f);
    DrawTextEx(font, "Settings", Vector2{ (480.0f - titleSize.x) * 0.5f, 130.0f }, 34, 1.0f, darkNavy);

    DrawTextEx(font, "APPEARANCE", Vector2{ 70, 215 }, 16, 1.0f, warmGray);
    themeToggleBtn.draw();

    DrawTextEx(font, "ABOUT FASTAGRAM", Vector2{ 70, 340 }, 16, 1.0f, warmGray);
    
    Rectangle infoCard = { 70, 375, 340, 140 };
    DrawRectangleRounded(infoCard, 0.25f, 8, Color{ 255, 255, 255, 240 });
    DrawRectangleRoundedLinesEx(infoCard, 0.25f, 8, 1.5f, Color{ 220, 215, 202, 255 });

    DrawTextEx(font, "FASTagram v1.0", Vector2{ 95, 395 }, 20, 1.0f, darkNavy);
    DrawTextEx(font, "Campus Desktop Social App", Vector2{ 95, 425 }, 16, 1.0f, warmGray);
    DrawTextEx(font, "FAST NUCES Semester Project", Vector2{ 95, 455 }, 14, 1.0f, Color{ 35, 95, 205, 255 });

    logoutBtn.draw();
}
