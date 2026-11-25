#include "ProfileScreen.h"
#include "services/PostManager.h"

ProfileScreen::ProfileScreen()
    : logoutBtn(70.0f, 725.0f, 340.0f, 44.0f, "Log Out", STYLE_OUTLINE_BEIGE),
      settingsBtn(395.0f, 35.0f, 55.0f, 36.0f, "⚙", STYLE_CUTE_YELLOW),
      feedNavBtn(30.0f, 35.0f, 85.0f, 36.0f, "🏠 Feed", STYLE_PRIMARY_DARK),
      editBioBtn(70.0f, 370.0f, 340.0f, 44.0f, "Edit Bio", STYLE_PRIMARY_DARK)
{
}

void ProfileScreen::update(bool& shouldLogout, bool& shouldOpenSettings, bool& shouldGoFeed) {
    logoutBtn.update();
    settingsBtn.update();
    feedNavBtn.update();
    editBioBtn.update();

    if (logoutBtn.isClicked()) {
        shouldLogout = true;
    }
    if (settingsBtn.isClicked()) {
        shouldOpenSettings = true;
    }
    if (feedNavBtn.isClicked()) {
        shouldGoFeed = true;
    }
}

void ProfileScreen::draw(const Account* user, const PostManager* postManager) {
    if (!user) return;

    Font font = Widget::getFont();
    Color darkNavy = Color{ 22, 32, 52, 255 };
    Color warmGray = Color{ 110, 115, 125, 255 };

    feedNavBtn.draw();
    settingsBtn.draw();

    Vector2 avatarCenter = { 240, 135 };
    float avatarRadius = 46.0f;

    // Cute pastel avatar ring
    DrawCircleV(avatarCenter, avatarRadius + 4, Color{ 142, 195, 245, 255 }); // Pastel blue ring
    DrawCircleV(avatarCenter, avatarRadius, Color{ 255, 242, 175, 255 });      // Butter yellow avatar fill

    std::string initial = user->getDisplayName().empty() ? "U" : user->getDisplayName().substr(0, 1);
    Vector2 initSize = MeasureTextEx(font, initial.c_str(), 42, 1.0f);
    DrawTextEx(font, initial.c_str(), 
               Vector2{ avatarCenter.x - initSize.x * 0.5f, avatarCenter.y - initSize.y * 0.5f }, 
               42, 1.0f, darkNavy);

    Vector2 nameSize = MeasureTextEx(font, user->getDisplayName().c_str(), 26, 1.0f);
    DrawTextEx(font, user->getDisplayName().c_str(), 
               Vector2{ (480.0f - nameSize.x) / 2.0f, 195 }, 26, 1.0f, darkNavy);

    std::string handle = "@" + user->getUsername();
    Vector2 handleSize = MeasureTextEx(font, handle.c_str(), 16, 1.0f);
    DrawTextEx(font, handle.c_str(), 
               Vector2{ (480.0f - handleSize.x) / 2.0f, 224 }, 16, 1.0f, warmGray);

    Vector2 bioSize = MeasureTextEx(font, user->getBio().c_str(), 16, 1.0f);
    DrawTextEx(font, user->getBio().c_str(), 
               Vector2{ (480.0f - bioSize.x) / 2.0f, 246 }, 16, 1.0f, warmGray);

    // Calculate user's dynamic post count
    int userPostCount = 0;
    std::vector<Post> userPosts;
    if (postManager) {
        for (const auto& p : postManager->getAllPosts()) {
            if (p.getAuthorRoll() == user->getUsername()) {
                userPostCount++;
                userPosts.push_back(p);
            }
        }
    }

    // Stats bar with cute rounded container
    Rectangle statsBar = { 50.0f, 275.0f, 380.0f, 75.0f };
    DrawRectangleRounded(statsBar, 0.35f, 16, Color{ 255, 255, 255, 240 });
    DrawRectangleRoundedLinesEx(statsBar, 0.35f, 16, 1.5f, Color{ 220, 215, 202, 255 });

    float statY = 292.0f;
    std::string postCountStr = std::to_string(userPostCount);
    DrawTextEx(font, postCountStr.c_str(), Vector2{ 110, statY }, 22, 1.0f, darkNavy);
    DrawTextEx(font, "Posts", Vector2{ 100, statY + 22 }, 15, 1.0f, warmGray);

    DrawTextEx(font, "312", Vector2{ 228, statY }, 22, 1.0f, darkNavy);
    DrawTextEx(font, "Followers", Vector2{ 210, statY + 22 }, 15, 1.0f, warmGray);

    DrawTextEx(font, "198", Vector2{ 350, statY }, 22, 1.0f, darkNavy);
    DrawTextEx(font, "Following", Vector2{ 338, statY + 22 }, 15, 1.0f, warmGray);

    editBioBtn.draw();

    // Cute grid cards
    float gridStartY = 430.0f;
    float boxSize = 90.0f;
    float gap = 14.0f;
    float startX = (480.0f - (boxSize * 3 + gap * 2)) / 2.0f;

    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 3; ++col) {
            int idx = row * 3 + col;
            float boxX = startX + col * (boxSize + gap);
            float boxY = gridStartY + row * (boxSize + gap);
            Rectangle tileRect = { boxX, boxY, boxSize, boxSize };

            Color tileBg = (idx < (int)userPosts.size()) ? userPosts[idx].getCardColor() : Color{ 255, 255, 255, 230 };
            DrawRectangleRounded(tileRect, 0.25f, 8, tileBg);
            DrawRectangleRoundedLinesEx(tileRect, 0.25f, 8, 1.5f, Color{ 225, 220, 210, 255 });
            
            if (idx < (int)userPosts.size()) {
                // Show post snippet in the tile
                std::string snippet = userPosts[idx].getContent().substr(0, 15) + "..";
                DrawTextEx(font, snippet.c_str(), Vector2{ boxX + 8, boxY + 36 }, 11, 1.0f, darkNavy);
            } else {
                DrawCircle((int)(boxX + boxSize/2), (int)(boxY + boxSize/2 - 6), 14, Color{ 210, 225, 245, 255 });
                DrawRectangleRounded(Rectangle{ boxX + 18, boxY + boxSize - 26, boxSize - 36, 10 }, 0.4f, 4, Color{ 255, 238, 160, 255 });
            }
        }
    }

    logoutBtn.draw();
}
