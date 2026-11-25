#pragma once
#include "raylib.h"
#include "Button.h"
#include "TextBox.h"
#include "CuteMascot.h"
#include "UserManager.h"
#include <string>

class SignupScreen {
private:
    CuteMascot topMascot;
    TextBox rollBox;
    TextBox nameBox;
    TextBox passBox;
    Button createBtn;
    Button backBtn;

    std::string errorMessage;
    Color errorColor;

public:
    SignupScreen();

    void update(UserManager& userManager, bool& shouldGoBack, bool& signupSuccess);
    void draw();
    void reset();
};
