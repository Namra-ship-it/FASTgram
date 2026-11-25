#include "SignupScreen.h"

SignupScreen::SignupScreen()
    : topMascot(60.0f, 0.0f, 360.0f, 185.0f, false, Color{ 142, 195, 245, 255 }), // Pastel blue mascot at top!
      rollBox(70.0f, 305.0f, 340.0f, 52.0f, "Roll Number (e.g. i261234)"),
      nameBox(70.0f, 370.0f, 340.0f, 52.0f, "Full Name (e.g. Sara Ahmed)"),
      passBox(70.0f, 435.0f, 340.0f, 52.0f, "Create Password", true),
      createBtn(70.0f, 510.0f, 340.0f, 54.0f, "Create Account", STYLE_PRIMARY_DARK),
      backBtn(25.0f, 25.0f, 80.0f, 36.0f, "← Back", STYLE_OUTLINE_BEIGE),
      errorMessage(""),
      errorColor(RED)
{
}

void SignupScreen::reset() {
    rollBox.clear();
    nameBox.clear();
    passBox.clear();
    errorMessage = "";
}

void SignupScreen::update(UserManager& userManager, bool& shouldGoBack, bool& signupSuccess) {
    topMascot.update();
    rollBox.update();
    nameBox.update();
    passBox.update();
    createBtn.update();
    backBtn.update();

    if (backBtn.isClicked()) {
        shouldGoBack = true;
        reset();
        return;
    }

    if (createBtn.isClicked()) {
        std::string roll = rollBox.getText();
        std::string name = nameBox.getText();
        std::string pass = passBox.getText();

        if (roll.empty() || name.empty() || pass.empty()) {
            errorMessage = "Please fill in all fields!";
            errorColor = Color{ 210, 45, 45, 255 };
            return;
        }

        if (pass.length() < 4) {
            errorMessage = "Password must be at least 4 chars!";
            errorColor = Color{ 210, 45, 45, 255 };
            return;
        }

        bool registered = userManager.registerUser(roll, name, pass, ACCOUNT_STUDENT);
        if (registered) {
            errorMessage = "Account created! Please log in.";
            errorColor = Color{ 35, 135, 75, 255 };
            signupSuccess = true;
        } else {
            errorMessage = "Roll number already registered!";
            errorColor = Color{ 210, 45, 45, 255 };
        }
    }
}

void SignupScreen::draw() {
    Font font = Widget::getFont();
    Color darkNavy = Color{ 22, 32, 52, 255 };
    Color subGray  = Color{ 120, 125, 135, 255 };

    // Draw the cute blinking mascot looking down from the top!
    topMascot.draw();
    backBtn.draw();

    Vector2 titleSize = MeasureTextEx(font, "Ready to join?", 34, 1.0f);
    DrawTextEx(font, "Ready to join?", Vector2{ (480.0f - titleSize.x) * 0.5f, 215.0f }, 34, 1.0f, darkNavy);

    Vector2 subSize = MeasureTextEx(font, "Create your campus account", 17, 1.0f);
    DrawTextEx(font, "Create your campus account", Vector2{ (480.0f - subSize.x) * 0.5f, 255.0f }, 17, 1.0f, subGray);

    rollBox.draw();
    nameBox.draw();
    passBox.draw();
    createBtn.draw();

    if (!errorMessage.empty()) {
        Vector2 msgSize = MeasureTextEx(font, errorMessage.c_str(), 17, 1.0f);
        DrawTextEx(font, errorMessage.c_str(), Vector2{ (480.0f - msgSize.x) * 0.5f, 580.0f }, 17, 1.0f, errorColor);
    }
}
