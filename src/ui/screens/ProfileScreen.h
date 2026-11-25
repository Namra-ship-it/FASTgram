#pragma once
#include "raylib.h"
#include "Account.h"
#include "Button.h"
#include <string>

class PostManager; // forward declaration

class ProfileScreen {
private:
    Button logoutBtn;
    Button settingsBtn;
    Button editBioBtn;
    Button feedNavBtn;

public:
    ProfileScreen();

    void update(bool& shouldLogout, bool& shouldOpenSettings, bool& shouldGoFeed);
    void draw(const Account* user, const PostManager* postManager = nullptr);
};
