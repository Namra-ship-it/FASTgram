#include "raylib.h"
#include "resource_dir.h"
#include "ui/widgets/Button.h"
#include "ui/widgets/TextBox.h"
#include "ui/widgets/CuteMascot.h"
#include "services/UserManager.h"
#include "services/PostManager.h"
#include "services/ChatManager.h"
#include "services/StorageManager.h"
#include "core/Session.h"
#include "ui/screens/FeedScreen.h"
#include "ui/screens/SearchScreen.h"
#include "ui/screens/ChatScreen.h"
#include "ui/screens/ProfileScreen.h"
#include "ui/screens/SignupScreen.h"
#include "ui/screens/SettingsScreen.h"

enum AppScreen {
    SCREEN_LOGIN,
    SCREEN_SIGNUP,
    SCREEN_FEED,
    SCREEN_SEARCH,
    SCREEN_CHAT,
    SCREEN_PROFILE,
    SCREEN_SETTINGS
};

int main()
{
    InitWindow(480, 820, "FASTagram - Campus Social App");
    SetTargetFPS(60);

    SearchAndSetResourceDir("resources");

    Font appFont = LoadFontEx("fonts/Fredoka.ttf", 64, 0, 0);
    SetTextureFilter(appFont.texture, TEXTURE_FILTER_BILINEAR);
    Widget::setFont(appFont);

    UserManager userManager;
    PostManager postManager;
    ChatManager chatManager;
    StorageManager storageManager;

    FeedScreen feedScreen;
    SearchScreen searchScreen;
    ChatScreen chatScreen;
    ProfileScreen profileScreen;
    SignupScreen signupScreen;
    SettingsScreen settingsScreen;

    // Load persistent binary data (.dat with FTG1 header) and session config
    std::string activeSessionUser = "";
    bool isDarkMode = false;
    storageManager.loadConfig(activeSessionUser, isDarkMode);
    storageManager.loadBinaryData(userManager, postManager, chatManager);

    if (isDarkMode) {
        settingsScreen.setIsDarkMode(true);
    }

    AppScreen currentScreen = SCREEN_LOGIN;

    // Restore persistent session (Section 6.1 A2)
    if (!activeSessionUser.empty()) {
        Account* savedUser = userManager.findByUsername(activeSessionUser);
        if (savedUser) {
            Session::getInstance().login(savedUser);
            currentScreen = SCREEN_FEED; // Direct to Feed if already logged in!
        }
    }

    // Login Form Widgets
    TextBox rollBox(70.0f, 280.0f, 340.0f, 52.0f, "Roll Number (e.g. i261234)");
    TextBox passBox(70.0f, 345.0f, 340.0f, 52.0f, "Password", true);
    Button logInBtn(70.0f, 415.0f, 340.0f, 54.0f, "Log In", STYLE_PRIMARY_DARK);
    Button signUpBtn(70.0f, 480.0f, 340.0f, 54.0f, "Sign Up", STYLE_CUTE_YELLOW);

    // Cute animated bottom mascot (peeking up with cartoon blinking eyes!)
    CuteMascot bottomMascot(60.0f, 620.0f, 360.0f, 200.0f, true, Color{ 255, 235, 135, 255 });

    std::string loginError = "";
    Color darkNavy = Color{ 22, 32, 52, 255 };
    Color warmBeige = Color{ 250, 246, 238, 255 };

    while (!WindowShouldClose())
    {
        switch (currentScreen)
        {
        case SCREEN_LOGIN: {
            bottomMascot.update();
            rollBox.update();
            passBox.update();
            logInBtn.update();
            signUpBtn.update();

            if (logInBtn.isClicked()) {
                Account* user = userManager.authenticate(rollBox.getText(), passBox.getText());
                if (user) {
                    Session::getInstance().login(user);
                    storageManager.saveConfig(user->getUsername(), settingsScreen.getIsDarkMode());
                    storageManager.saveBinaryData(userManager, postManager, chatManager);
                    loginError = "";
                    currentScreen = SCREEN_FEED; // Direct to Feed on login!
                } else {
                    loginError = "Invalid roll number or password!";
                }
            }

            if (signUpBtn.isClicked()) {
                loginError = "";
                signupScreen.reset();
                currentScreen = SCREEN_SIGNUP;
            }
            break;
        }

        case SCREEN_SIGNUP: {
            bool shouldGoBack = false;
            bool signupSuccess = false;
            signupScreen.update(userManager, shouldGoBack, signupSuccess);

            if (signupSuccess) {
                storageManager.saveBinaryData(userManager, postManager, chatManager);
            }

            if (shouldGoBack || signupSuccess) {
                currentScreen = SCREEN_LOGIN;
            }
            break;
        }

        case SCREEN_FEED: {
            bool shouldOpenProfile = false;
            bool shouldOpenChat = false;
            bool shouldOpenSearch = false;
            feedScreen.update(postManager, Session::getInstance().getCurrentUser(), 
                              shouldOpenProfile, shouldOpenChat, shouldOpenSearch);

            if (shouldOpenSearch) {
                currentScreen = SCREEN_SEARCH;
            }
            if (shouldOpenChat) {
                currentScreen = SCREEN_CHAT;
            }
            if (shouldOpenProfile) {
                currentScreen = SCREEN_PROFILE;
            }
            break;
        }

        case SCREEN_SEARCH: {
            bool shouldGoBack = false;
            searchScreen.update(userManager, Session::getInstance().getCurrentUser(), shouldGoBack);

            if (shouldGoBack) {
                currentScreen = SCREEN_FEED;
            }
            break;
        }

        case SCREEN_CHAT: {
            bool shouldGoBack = false;
            chatScreen.update(chatManager, Session::getInstance().getCurrentUser(), shouldGoBack);

            if (shouldGoBack) {
                currentScreen = SCREEN_FEED;
            }
            break;
        }

        case SCREEN_PROFILE: {
            bool shouldLogout = false;
            bool shouldOpenSettings = false;
            bool shouldGoFeed = false;
            profileScreen.update(shouldLogout, shouldOpenSettings, shouldGoFeed);

            if (shouldGoFeed) {
                currentScreen = SCREEN_FEED;
            }
            if (shouldLogout) {
                Session::getInstance().logout();
                storageManager.saveConfig("", settingsScreen.getIsDarkMode());
                rollBox.clear();
                passBox.clear();
                currentScreen = SCREEN_LOGIN;
            }
            if (shouldOpenSettings) {
                currentScreen = SCREEN_SETTINGS;
            }
            break;
        }

        case SCREEN_SETTINGS: {
            bool shouldGoBack = false;
            bool shouldLogout = false;
            settingsScreen.update(shouldGoBack, shouldLogout);

            if (shouldGoBack) {
                currentScreen = SCREEN_PROFILE;
            }
            if (shouldLogout) {
                Session::getInstance().logout();
                storageManager.saveConfig("", settingsScreen.getIsDarkMode());
                rollBox.clear();
                passBox.clear();
                currentScreen = SCREEN_LOGIN;
            }
            break;
        }
        }

        BeginDrawing();
        ClearBackground(warmBeige);

        // Decorative pastel background blobs (on Login & Settings)
        if (currentScreen == SCREEN_LOGIN || currentScreen == SCREEN_SETTINGS) {
            DrawCircle(400, 100, 180, Color{ 255, 230, 210, 160 }); // soft peach blob
            DrawCircle(40, 580, 160, Color{ 220, 240, 255, 170 });  // soft pastel blue blob
            DrawCircle(420, 720, 130, Color{ 255, 248, 205, 180 }); // warm butter blob
        }

        switch (currentScreen)
        {
        case SCREEN_LOGIN: {
            // Header Title
            Vector2 titleSize = MeasureTextEx(appFont, "FASTagram", 46, 1.0f);
            DrawTextEx(appFont, "FASTagram", Vector2{ (480.0f - titleSize.x) * 0.5f, 130.0f }, 46, 1.0f, darkNavy);

            Vector2 subSize = MeasureTextEx(appFont, "FAST NUCES Campus Community", 16, 1.0f);
            DrawTextEx(appFont, "FAST NUCES Campus Community", Vector2{ (480.0f - subSize.x) * 0.5f, 185.0f }, 16, 1.0f, Color{ 110, 115, 125, 255 });

            rollBox.draw();
            passBox.draw();
            logInBtn.draw();
            signUpBtn.draw();

            if (!loginError.empty()) {
                Vector2 errSize = MeasureTextEx(appFont, loginError.c_str(), 14, 1.0f);
                DrawTextEx(appFont, loginError.c_str(), 
                           Vector2{ (480.0f - errSize.x) * 0.5f, 545.0f }, 
                           14, 1.0f, Color{ 230, 60, 60, 255 });
            }

            bottomMascot.draw();
            break;
        }

        case SCREEN_SIGNUP: {
            signupScreen.draw();
            break;
        }

        case SCREEN_FEED: {
            feedScreen.draw(postManager, Session::getInstance().getCurrentUser());
            break;
        }

        case SCREEN_SEARCH: {
            searchScreen.draw(userManager, Session::getInstance().getCurrentUser());
            break;
        }

        case SCREEN_CHAT: {
            chatScreen.draw(chatManager, Session::getInstance().getCurrentUser());
            break;
        }

        case SCREEN_PROFILE: {
            profileScreen.draw(Session::getInstance().getCurrentUser(), &postManager);
            break;
        }

        case SCREEN_SETTINGS: {
            settingsScreen.draw();
            break;
        }
        }

        if (settingsScreen.getIsDarkMode()) {
            DrawRectangle(0, 0, 480, 820, Color{ 15, 20, 32, 140 });
        }

        EndDrawing();
    }

    // Auto-save persistent binary data and config on exit
    std::string sUser = Session::getInstance().isLoggedIn() ? Session::getInstance().getCurrentUser()->getUsername() : "";
    storageManager.saveConfig(sUser, settingsScreen.getIsDarkMode());
    storageManager.saveBinaryData(userManager, postManager, chatManager);

    UnloadFont(appFont);
    CloseWindow();
    return 0;
}
