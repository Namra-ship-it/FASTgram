#pragma once
#include "raylib.h"
#include "services/UserManager.h"
#include "models/Account.h"
#include "ui/widgets/Button.h"
#include "ui/widgets/TextBox.h"
#include <string>
#include <vector>

class SearchScreen {
private:
    TextBox searchInput;
    Button backBtn;
    float scrollOffset;

public:
    SearchScreen();

    void update(UserManager& userManager, const Account* currentUser, bool& shouldGoBack);
    void draw(UserManager& userManager, const Account* currentUser);

    void reset();
};
