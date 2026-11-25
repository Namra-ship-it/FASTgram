#include "SearchScreen.h"

SearchScreen::SearchScreen()
    : searchInput(25.0f, 75.0f, 430.0f, 46.0f, "Search name or roll no (e.g. Ali, i26)"),
      backBtn(20.0f, 20.0f, 75.0f, 36.0f, "← Feed", STYLE_OUTLINE_BEIGE),
      scrollOffset(0.0f)
{
}

void SearchScreen::reset() {
    searchInput.clear();
    scrollOffset = 0.0f;
}

void SearchScreen::update(UserManager& userManager, const Account* currentUser, bool& shouldGoBack) {
    backBtn.update();
    searchInput.update();

    if (backBtn.isClicked()) {
        shouldGoBack = true;
        return;
    }

    // Scroll
    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f) {
        scrollOffset += wheel * 30.0f;
        if (scrollOffset > 0.0f) scrollOffset = 0.0f;
    }

    // Follow button click detection
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && currentUser) {
        Vector2 mouse = GetMousePosition();
        auto results = userManager.searchUsers(searchInput.getText());

        float cardY = 145.0f + scrollOffset;
        for (const auto* acc : results) {
            if (acc->getUsername() != currentUser->getUsername()) {
                Rectangle followBtnRect = { 335.0f, cardY + 16.0f, 100.0f, 34.0f };
                if (CheckCollisionPointRec(mouse, followBtnRect)) {
                    userManager.toggleFollow(currentUser->getUsername(), acc->getUsername());
                    break;
                }
            }
            cardY += 78.0f;
        }
    }
}

void SearchScreen::draw(UserManager& userManager, const Account* currentUser) {
    Font font = Widget::getFont();
    Color darkNavy = Color{ 22, 32, 52, 255 };
    Color warmGray = Color{ 110, 115, 125, 255 };

    // --- 1. Top Bar ---
    DrawRectangle(0, 0, 480, 135, Color{ 250, 246, 238, 255 });
    backBtn.draw();
    DrawTextEx(font, "Explore Campus", Vector2{ 110, 26 }, 22, 1.0f, darkNavy);

    searchInput.draw();
    DrawLine(0, 135, 480, 135, Color{ 230, 225, 215, 255 });

    // --- 2. Search Results List ---
    BeginScissorMode(0, 136, 480, 680);

    auto results = userManager.searchUsers(searchInput.getText());
    float cardY = 145.0f + scrollOffset;

    if (results.empty()) {
        DrawTextEx(font, "No students found matching your search.", Vector2{ 85, 220 }, 15, 1.0f, warmGray);
    }

    for (const auto* acc : results) {
        bool isMe = (currentUser && acc->getUsername() == currentUser->getUsername());

        Rectangle cardRect = { 25.0f, cardY, 430.0f, 66.0f };
        DrawRectangleRounded(cardRect, 0.35f, 6, Color{ 255, 255, 255, 240 });
        DrawRectangleRoundedLinesEx(cardRect, 0.35f, 6, 1.5f, Color{ 220, 215, 205, 255 });

        // Avatar
        DrawCircle(55, (int)(cardY + 33), 18.0f, Color{ 142, 195, 245, 255 });
        std::string init = acc->getDisplayName().empty() ? "U" : acc->getDisplayName().substr(0, 1);
        DrawTextEx(font, init.c_str(), Vector2{ 49, cardY + 23 }, 16, 1.0f, darkNavy);

        // Name & Handle
        DrawTextEx(font, acc->getDisplayName().c_str(), Vector2{ 86, cardY + 14 }, 17, 1.0f, darkNavy);
        std::string handle = "@" + acc->getUsername() + " • " + std::to_string(userManager.getFollowersCount(acc->getUsername())) + " followers";
        DrawTextEx(font, handle.c_str(), Vector2{ 86, cardY + 35 }, 12, 1.0f, warmGray);

        // Follow Button (if not yourself)
        if (!isMe && currentUser) {
            bool following = userManager.isFollowing(currentUser->getUsername(), acc->getUsername());
            Rectangle followPill = { 335.0f, cardY + 16.0f, 100.0f, 34.0f };

            Color bg = following ? Color{ 255, 255, 255, 240 } : Color{ 22, 32, 52, 255 };
            Color txt = following ? darkNavy : WHITE;
            DrawRectangleRounded(followPill, 0.5f, 6, bg);
            if (following) {
                DrawRectangleRoundedLinesEx(followPill, 0.5f, 6, 1.5f, Color{ 200, 195, 185, 255 });
            }

            const char* label = following ? "Following" : "+ Follow";
            Vector2 lSize = MeasureTextEx(font, label, 14, 1.0f);
            DrawTextEx(font, label, Vector2{ followPill.x + (followPill.width - lSize.x) * 0.5f, followPill.y + 8 }, 14, 1.0f, txt);
        } else if (isMe) {
            DrawTextEx(font, "(You)", Vector2{ 380, cardY + 24 }, 14, 1.0f, warmGray);
        }

        cardY += 76.0f;
    }

    EndScissorMode();
}
