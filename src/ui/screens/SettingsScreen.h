#pragma once
#include "raylib.h"
#include "Button.h"

class SettingsScreen {
private:
    Button backBtn;
    Button themeToggleBtn;
    Button logoutBtn;
    bool isDarkMode;

public:
    SettingsScreen();

    void update(bool& shouldGoBack, bool& shouldLogout);
    void draw();

    bool getIsDarkMode() const { return isDarkMode; }
    void setIsDarkMode(bool dark) { isDarkMode = dark; }
};
