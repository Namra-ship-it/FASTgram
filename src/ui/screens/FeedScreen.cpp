#include "FeedScreen.h"
#include <cstdio>
#include <cmath>

FeedScreen::FeedScreen()
    : scrollOffset(0.0f),
      feedNavBtn(20.0f, 755.0f, 95.0f, 44.0f, "🏠 Feed", STYLE_PRIMARY_DARK),
      newPostBtn(125.0f, 755.0f, 95.0f, 44.0f, "+ Post", STYLE_CUTE_YELLOW),
      chatNavBtn(230.0f, 755.0f, 105.0f, 44.0f, "💬 DMs", STYLE_OUTLINE_BEIGE),
      profileNavBtn(345.0f, 755.0f, 115.0f, 44.0f, "👤 Profile", STYLE_OUTLINE_BEIGE),
      searchTopBtn(350.0f, 12.0f, 38.0f, 32.0f, "🔍", STYLE_OUTLINE_BEIGE),
      notifTopBtn(395.0f, 12.0f, 38.0f, 32.0f, "🔔", STYLE_OUTLINE_BEIGE),
      isCreatePostModalOpen(false),
      activePostTab(0),
      tabPhotoBtn(40.0f, 175.0f, 95.0f, 36.0f, "📷 Photo", STYLE_PRIMARY_DARK),
      tabTextBtn(142.0f, 175.0f, 95.0f, 36.0f, "💬 Text", STYLE_OUTLINE_BEIGE),
      tabPollBtn(244.0f, 175.0f, 95.0f, 36.0f, "📊 Poll", STYLE_OUTLINE_BEIGE),
      tabEventBtn(346.0f, 175.0f, 95.0f, 36.0f, "📅 Event", STYLE_OUTLINE_BEIGE),
      photoPathBox(40.0f, 225.0f, 400.0f, 44.0f, "Image path or drag & drop photo here..."),
      photoCaptionBox(40.0f, 520.0f, 400.0f, 48.0f, "Write an engaging caption... #FAST #NASCON"),
      activeFilter("Normal"),
      filterNormalBtn(40.0f, 470.0f, 75.0f, 32.0f, "Normal", STYLE_PRIMARY_DARK),
      filterGrayBtn(120.0f, 470.0f, 75.0f, 32.0f, "B&W", STYLE_OUTLINE_BEIGE),
      filterWarmBtn(200.0f, 470.0f, 75.0f, 32.0f, "Warm", STYLE_OUTLINE_BEIGE),
      filterContrastBtn(280.0f, 470.0f, 80.0f, 32.0f, "Contrast", STYLE_OUTLINE_BEIGE),
      filterSepiaBtn(365.0f, 470.0f, 75.0f, 32.0f, "Sepia", STYLE_OUTLINE_BEIGE),
      pollQuestionBox(40.0f, 230.0f, 400.0f, 48.0f, "Ask a question to campus..."),
      pollOpt1Box(40.0f, 290.0f, 400.0f, 44.0f, "Option 1 (e.g. Biryani 🍛)"),
      pollOpt2Box(40.0f, 345.0f, 400.0f, 44.0f, "Option 2 (e.g. Shawarma 🌯)"),
      eventTitleBox(40.0f, 225.0f, 400.0f, 44.0f, "Event Title (e.g. CodeSprint '26)"),
      eventDateBox(40.0f, 280.0f, 400.0f, 44.0f, "Date & Time (e.g. 14 MAR • 3:00 PM)"),
      eventVenueBox(40.0f, 335.0f, 400.0f, 44.0f, "Venue (e.g. CS Lab 3 & 4)"),
      eventDetailsBox(40.0f, 390.0f, 400.0f, 54.0f, "Event description & rules..."),
      submitPostBtn(40.0f, 580.0f, 195.0f, 46.0f, "Share to Feed", STYLE_PRIMARY_DARK),
      cancelPostBtn(245.0f, 580.0f, 195.0f, 46.0f, "Cancel", STYLE_OUTLINE_BEIGE),
      activeCommentPostId(-1),
      commentInputBox(40.0f, 620.0f, 300.0f, 46.0f, "Write a comment..."),
      sendCommentBtn(350.0f, 620.0f, 90.0f, 46.0f, "Send", STYLE_PRIMARY_DARK),
      closeCommentBtn(390.0f, 140.0f, 40.0f, 36.0f, "✕", STYLE_OUTLINE_BEIGE),
      isNotificationsOpen(false),
      closeNotifBtn(390.0f, 170.0f, 40.0f, 36.0f, "✕", STYLE_OUTLINE_BEIGE),
      activeStoryIndex(-1),
      storyTimer(0.0f),
      closeStoryBtn(400.0f, 30.0f, 45.0f, 36.0f, "✕", STYLE_OUTLINE_BEIGE)
{
    stories.push_back({ "Sara Ahmed", "10m ago", "NASCON Auto show is packed! Come check the heavy bikes! 🏍️", Color{ 255, 245, 215, 255 } });
    stories.push_back({ "Hamza Tariq", "1h ago", "Coding competition starts in 30 minutes in Lab 2. Good luck! 💻", Color{ 230, 245, 255, 255 } });
    stories.push_back({ "Ali Khan", "3h ago", "Cafeteria Biryani is on point today! Grab it fast! 🍛🔥", Color{ 255, 230, 240, 255 } });
}

FeedScreen::~FeedScreen() {
    for (size_t i = 0; i < textureCache.size(); ++i) {
        UnloadTexture(textureCache[i].second);
    }
    textureCache.clear();
}

Texture2D FeedScreen::getOrLoadTexture(const std::string& path) {
    if (path.empty()) return Texture2D{ 0 };

    for (size_t i = 0; i < textureCache.size(); ++i) {
        if (textureCache[i].first == path) {
            return textureCache[i].second;
        }
    }

    Texture2D tex = LoadTexture(path.c_str());
    if (tex.id != 0) {
        SetTextureFilter(tex, TEXTURE_FILTER_BILINEAR);
        textureCache.push_back({ path, tex });
    }
    return tex;
}

void FeedScreen::reset() {
    scrollOffset = 0.0f;
    isCreatePostModalOpen = false;
    activeCommentPostId = -1;
    isNotificationsOpen = false;
    activeStoryIndex = -1;
}

void FeedScreen::update(PostManager& postManager, const Account* currentUser, 
                        bool& shouldOpenProfile, bool& shouldOpenChat, bool& shouldOpenSearch)
{
    // Bottom bar navigation
    feedNavBtn.update();
    newPostBtn.update();
    chatNavBtn.update();
    profileNavBtn.update();

    if (profileNavBtn.isClicked()) { shouldOpenProfile = true; return; }
    if (chatNavBtn.isClicked()) { shouldOpenChat = true; return; }
    if (searchTopBtn.isClicked()) { shouldOpenSearch = true; return; }

    searchTopBtn.update();
    notifTopBtn.update();
    if (notifTopBtn.isClicked()) {
        isNotificationsOpen = !isNotificationsOpen;
    }

    // Story viewer update
    if (activeStoryIndex >= 0) {
        closeStoryBtn.update();
        storyTimer += GetFrameTime();
        if (closeStoryBtn.isClicked() || IsKeyPressed(KEY_ESCAPE) || storyTimer >= 4.0f) {
            activeStoryIndex++;
            storyTimer = 0.0f;
            if (activeStoryIndex >= (int)stories.size()) {
                activeStoryIndex = -1;
            }
        }
        return;
    }

    // Create post button
    if (newPostBtn.isClicked()) {
        isCreatePostModalOpen = true;
        photoPathBox.setText("images/posts/nascon_autoshow.jpg");
        photoCaptionBox.clear();
    }

    // Drag and drop photo detection (Section 6.4 C2)
    if (isCreatePostModalOpen && IsFileDropped()) {
        FilePathList droppedFiles = LoadDroppedFiles();
        if (droppedFiles.count > 0) {
            photoPathBox.setText(droppedFiles.paths[0]);
        }
        UnloadDroppedFiles(droppedFiles);
    }

    // Create Post Modal Update
    if (isCreatePostModalOpen) {
        tabPhotoBtn.update();
        tabTextBtn.update();
        tabPollBtn.update();
        tabEventBtn.update();

        if (tabPhotoBtn.isClicked()) {
            activePostTab = 0;
            tabPhotoBtn.setStyle(STYLE_PRIMARY_DARK);
            tabTextBtn.setStyle(STYLE_OUTLINE_BEIGE);
            tabPollBtn.setStyle(STYLE_OUTLINE_BEIGE);
            tabEventBtn.setStyle(STYLE_OUTLINE_BEIGE);
        }
        if (tabTextBtn.isClicked()) {
            activePostTab = 1;
            tabPhotoBtn.setStyle(STYLE_OUTLINE_BEIGE);
            tabTextBtn.setStyle(STYLE_PRIMARY_DARK);
            tabPollBtn.setStyle(STYLE_OUTLINE_BEIGE);
            tabEventBtn.setStyle(STYLE_OUTLINE_BEIGE);
        }
        if (tabPollBtn.isClicked()) {
            activePostTab = 2;
            tabPhotoBtn.setStyle(STYLE_OUTLINE_BEIGE);
            tabTextBtn.setStyle(STYLE_OUTLINE_BEIGE);
            tabPollBtn.setStyle(STYLE_PRIMARY_DARK);
            tabEventBtn.setStyle(STYLE_OUTLINE_BEIGE);
        }
        if (tabEventBtn.isClicked()) {
            activePostTab = 3;
            tabPhotoBtn.setStyle(STYLE_OUTLINE_BEIGE);
            tabTextBtn.setStyle(STYLE_OUTLINE_BEIGE);
            tabPollBtn.setStyle(STYLE_OUTLINE_BEIGE);
            tabEventBtn.setStyle(STYLE_PRIMARY_DARK);
        }

        if (activePostTab == 0) {
            photoPathBox.update();
            photoCaptionBox.update();

            filterNormalBtn.update();
            filterGrayBtn.update();
            filterWarmBtn.update();
            filterContrastBtn.update();
            filterSepiaBtn.update();

            if (filterNormalBtn.isClicked()) activeFilter = "Normal";
            if (filterGrayBtn.isClicked()) activeFilter = "Grayscale";
            if (filterWarmBtn.isClicked()) activeFilter = "Warm";
            if (filterContrastBtn.isClicked()) activeFilter = "Contrast";
            if (filterSepiaBtn.isClicked()) activeFilter = "Sepia";

            filterNormalBtn.setStyle(activeFilter == "Normal" ? STYLE_PRIMARY_DARK : STYLE_OUTLINE_BEIGE);
            filterGrayBtn.setStyle(activeFilter == "Grayscale" ? STYLE_PRIMARY_DARK : STYLE_OUTLINE_BEIGE);
            filterWarmBtn.setStyle(activeFilter == "Warm" ? STYLE_PRIMARY_DARK : STYLE_OUTLINE_BEIGE);
            filterContrastBtn.setStyle(activeFilter == "Contrast" ? STYLE_PRIMARY_DARK : STYLE_OUTLINE_BEIGE);
            filterSepiaBtn.setStyle(activeFilter == "Sepia" ? STYLE_PRIMARY_DARK : STYLE_OUTLINE_BEIGE);
        }
        else if (activePostTab == 1) {
            photoCaptionBox.update();
        }
        else if (activePostTab == 2) {
            pollQuestionBox.update();
            pollOpt1Box.update();
            pollOpt2Box.update();
        }
        else if (activePostTab == 3) {
            eventTitleBox.update();
            eventDateBox.update();
            eventVenueBox.update();
            eventDetailsBox.update();
        }

        submitPostBtn.update();
        cancelPostBtn.update();

        if (submitPostBtn.isClicked() && currentUser) {
            if (activePostTab == 0) {
                std::string img = photoPathBox.getText();
                if (img.empty()) img = "images/posts/campus_main.jpg";
                postManager.createPhotoPost(currentUser->getUsername(), currentUser->getDisplayName(),
                                            photoCaptionBox.getText(), img, activeFilter);
            }
            else if (activePostTab == 1) {
                postManager.createTextPost(currentUser->getUsername(), currentUser->getDisplayName(),
                                           photoCaptionBox.getText());
            }
            else if (activePostTab == 2) {
                postManager.createPollPost(currentUser->getUsername(), currentUser->getDisplayName(),
                                           pollQuestionBox.getText(), pollOpt1Box.getText(), pollOpt2Box.getText());
            }
            else if (activePostTab == 3) {
                postManager.createEventPost(currentUser->getUsername(), currentUser->getDisplayName(),
                                            eventTitleBox.getText(), eventDateBox.getText(),
                                            eventVenueBox.getText(), eventDetailsBox.getText());
            }

            isCreatePostModalOpen = false;
            scrollOffset = 0.0f;
        }

        if (cancelPostBtn.isClicked() || IsKeyPressed(KEY_ESCAPE)) {
            isCreatePostModalOpen = false;
        }
        return;
    }

    // Comments modal
    if (activeCommentPostId >= 0) {
        commentInputBox.update();
        sendCommentBtn.update();
        closeCommentBtn.update();

        if (sendCommentBtn.isClicked() && currentUser) {
            std::string text = commentInputBox.getText();
            if (!text.empty()) {
                postManager.addComment(activeCommentPostId, 
                    Comment(currentUser->getUsername(), currentUser->getDisplayName(), text));
                commentInputBox.clear();
            }
        }
        if (closeCommentBtn.isClicked() || IsKeyPressed(KEY_ESCAPE)) {
            activeCommentPostId = -1;
        }
        return;
    }

    // Notifications modal
    if (isNotificationsOpen) {
        closeNotifBtn.update();
        if (closeNotifBtn.isClicked() || IsKeyPressed(KEY_ESCAPE)) {
            isNotificationsOpen = false;
        }
        return;
    }

    // Scroll feed
    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f) {
        scrollOffset += wheel * 36.0f;
        if (scrollOffset > 0.0f) scrollOffset = 0.0f;
    }

    // Story click check
    Vector2 mousePos = GetMousePosition();
    if (mousePos.y >= 50 && mousePos.y <= 130 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        for (int i = 0; i < (int)stories.size(); ++i) {
            float circleX = 50.0f + i * 85.0f;
            if (CheckCollisionPointCircle(mousePos, Vector2{ circleX, 90.0f }, 30.0f)) {
                activeStoryIndex = i;
                storyTimer = 0.0f;
                break;
            }
        }
    }

    // Post interaction clicks (Like, Comment, Poll Vote, RSVP)
    auto& posts = postManager.getAllPosts();
    float currentY = 150.0f + scrollOffset;

    for (size_t i = 0; i < posts.size(); ++i) {
        auto& post = posts[i];
        float cardH = (post.getKind() == POST_KIND_PHOTO) ? 440.0f : (post.getKind() == POST_KIND_NOTICE ? 110.0f : 250.0f);

        if (currentY + cardH > 130.0f && currentY < 750.0f) {
            // Heart button click
            Rectangle likeRect = { 35.0f, currentY + cardH - 42.0f, 65.0f, 32.0f };
            if (CheckCollisionPointRec(mousePos, likeRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && currentUser) {
                post.toggleLike(currentUser->getUsername());
            }

            // Comment button click
            Rectangle commentRect = { 105.0f, currentY + cardH - 42.0f, 75.0f, 32.0f };
            if (CheckCollisionPointRec(mousePos, commentRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                activeCommentPostId = post.getId();
            }

            // Poll vote clicks
            if (post.getKind() == POST_KIND_POLL && currentUser) {
                Rectangle opt1Rect = { 40.0f, currentY + 100.0f, 400.0f, 38.0f };
                Rectangle opt2Rect = { 40.0f, currentY + 148.0f, 400.0f, 38.0f };
                if (CheckCollisionPointRec(mousePos, opt1Rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    post.votePoll(1, currentUser->getUsername());
                }
                if (CheckCollisionPointRec(mousePos, opt2Rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    post.votePoll(2, currentUser->getUsername());
                }
            }

            // Event RSVP clicks
            if (post.getKind() == POST_KIND_EVENT && currentUser) {
                Rectangle goingRect = { 40.0f, currentY + 140.0f, 120.0f, 34.0f };
                Rectangle maybeRect = { 170.0f, currentY + 140.0f, 120.0f, 34.0f };
                Rectangle cantRect  = { 300.0f, currentY + 140.0f, 120.0f, 34.0f };

                if (CheckCollisionPointRec(mousePos, goingRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    post.submitRsvp(1, currentUser->getUsername());
                }
                if (CheckCollisionPointRec(mousePos, maybeRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    post.submitRsvp(2, currentUser->getUsername());
                }
                if (CheckCollisionPointRec(mousePos, cantRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    post.submitRsvp(3, currentUser->getUsername());
                }
            }
        }
        currentY += cardH + 18.0f;
    }
}

void FeedScreen::draw(PostManager& postManager, const Account* currentUser) {
    Font font = Widget::getFont();
    Color darkNavy = Color{ 22, 32, 52, 255 };
    Color warmGray = Color{ 110, 115, 125, 255 };
    Color fastBlue = Color{ 35, 95, 205, 255 };

    // --- 1. Top Instagram Header ---
    DrawRectangle(0, 0, 480, 52, Color{ 250, 246, 238, 255 });
    DrawTextEx(font, "FASTagram", Vector2{ 20, 12 }, 28, 1.0f, darkNavy);
    DrawTextEx(font, "• Campus Feed", Vector2{ 158, 20 }, 14, 1.0f, fastBlue);
    searchTopBtn.draw();
    notifTopBtn.draw();
    DrawLine(0, 52, 480, 52, Color{ 225, 220, 210, 255 });

    // --- 2. Stories Bar ---
    DrawRectangle(0, 52, 480, 84, Color{ 255, 255, 255, 255 });
    for (size_t i = 0; i < stories.size(); ++i) {
        float cx = 50.0f + i * 85.0f;
        // Gradient ring
        DrawCircleGradient(Vector2{ cx, 88.0f }, 30.0f, Color{ 245, 120, 75, 255 }, Color{ 235, 80, 140, 255 });
        DrawCircle((int)cx, 88, 27, WHITE);
        DrawCircle((int)cx, 88, 25, stories[i].color);

        std::string init = stories[i].authorName.empty() ? "U" : stories[i].authorName.substr(0, 1);
        DrawTextEx(font, init.c_str(), Vector2{ cx - 6, 78 }, 18, 1.0f, darkNavy);

        std::string shortName = stories[i].authorName.substr(0, stories[i].authorName.find(' '));
        DrawTextEx(font, shortName.c_str(), Vector2{ cx - 22, 118 }, 11, 1.0f, warmGray);
    }
    DrawLine(0, 136, 480, 136, Color{ 225, 220, 210, 255 });

    // --- 3. Scrollable Feed Posts ---
    BeginScissorMode(0, 137, 480, 618);

    auto& posts = postManager.getAllPosts();
    float currentY = 150.0f + scrollOffset;

    for (size_t i = 0; i < posts.size(); ++i) {
        auto& post = posts[i];
        float cardH = (post.getKind() == POST_KIND_PHOTO) ? 440.0f : (post.getKind() == POST_KIND_NOTICE ? 110.0f : 250.0f);

        if (currentY + cardH > 130.0f && currentY < 760.0f) {
            // Post Card Container
            Rectangle cardRect = { 20.0f, currentY, 440.0f, cardH };
            DrawRectangleRounded(cardRect, 0.04f, 8, WHITE);
            DrawRectangleRoundedLines(cardRect, 0.04f, 8, Color{ 230, 225, 215, 255 });

            // Post Header (Avatar + Username + Time + Badge)
            DrawCircle(45, (int)(currentY + 24.0f), 15.0f, Color{ 235, 240, 250, 255 });
            std::string aInit = post.getAuthorName().empty() ? "U" : post.getAuthorName().substr(0, 1);
            DrawTextEx(font, aInit.c_str(), Vector2{ 40, currentY + 15.0f }, 16, 1.0f, fastBlue);

            DrawTextEx(font, post.getAuthorName().c_str(), Vector2{ 68, currentY + 12.0f }, 15, 1.0f, darkNavy);
            std::string subline = "@" + post.getAuthorRoll() + " • " + post.getTimestamp();
            DrawTextEx(font, subline.c_str(), Vector2{ 68, currentY + 28.0f }, 11, 1.0f, warmGray);

            // Kind badge
            if (post.getKind() == POST_KIND_NOTICE) {
                DrawRectangleRounded(Rectangle{ 330.0f, currentY + 12.0f, 115.0f, 24.0f }, 0.3f, 4, Color{ 255, 235, 200, 255 });
                DrawTextEx(font, "📌 PINNED NOTICE", Vector2{ 336, currentY + 16.0f }, 10, 1.0f, Color{ 175, 80, 15, 255 });

                DrawTextEx(font, post.getContent().c_str(), Vector2{ 35, currentY + 52.0f }, 13, 1.0f, darkNavy);
            }
            else if (post.getKind() == POST_KIND_PHOTO) {
                // PHOTO POST (Section 6.2 P1)
                Texture2D photoTex = getOrLoadTexture(post.getImagePath());
                Rectangle imgDest = { 20.0f, currentY + 48.0f, 440.0f, 260.0f };

                Color tint = WHITE;
                if (post.getFilterName() == "Grayscale" || post.getFilterName() == "B&W") tint = Color{ 180, 180, 180, 255 };
                else if (post.getFilterName() == "Warm") tint = Color{ 255, 240, 220, 255 };
                else if (post.getFilterName() == "Sepia") tint = Color{ 245, 225, 195, 255 };

                if (photoTex.id != 0) {
                    Rectangle srcRect = { 0.0f, 0.0f, (float)photoTex.width, (float)photoTex.height };
                    DrawTexturePro(photoTex, srcRect, imgDest, Vector2{ 0, 0 }, 0.0f, tint);
                } else {
                    // Styled Fallback
                    DrawRectangleRec(imgDest, Color{ 240, 244, 250, 255 });
                    DrawTextEx(font, "📷 [Campus Photo]", Vector2{ 170, currentY + 160.0f }, 16, 1.0f, fastBlue);
                }

                // Action Bar
                bool isLiked = currentUser ? post.isLikedBy(currentUser->getUsername()) : false;
                std::string heart = isLiked ? "♥ " : "♡ ";
                std::string likeStr = heart + std::to_string(post.getLikeCount()) + " likes";
                Color heartColor = isLiked ? Color{ 235, 55, 75, 255 } : darkNavy;
                DrawTextEx(font, likeStr.c_str(), Vector2{ 35, currentY + 318.0f }, 14, 1.0f, heartColor);

                std::string commentStr = "💬 " + std::to_string(post.getCommentCount());
                DrawTextEx(font, commentStr.c_str(), Vector2{ 115, currentY + 318.0f }, 14, 1.0f, darkNavy);

                DrawTextEx(font, "✈️ Share", Vector2{ 165, currentY + 318.0f }, 13, 1.0f, warmGray);
                DrawTextEx(font, "🔖", Vector2{ 420, currentY + 318.0f }, 15, 1.0f, warmGray);

                // Caption
                std::string captionStr = post.getAuthorName() + ": " + post.getContent();
                DrawTextEx(font, captionStr.c_str(), Vector2{ 35, currentY + 342.0f }, 13, 1.0f, darkNavy);

                // View comments hint
                if (post.getCommentCount() > 0) {
                    std::string cHint = "View all " + std::to_string(post.getCommentCount()) + " comments...";
                    DrawTextEx(font, cHint.c_str(), Vector2{ 35, currentY + 412.0f }, 11, 1.0f, warmGray);
                }
            }
            else if (post.getKind() == POST_KIND_POLL) {
                // POLL POST (Section 6.2 P4)
                DrawTextEx(font, post.getContent().c_str(), Vector2{ 35, currentY + 55.0f }, 15, 1.0f, darkNavy);

                int totalVotes = post.getPollVotes1() + post.getPollVotes2();
                int pct1 = (totalVotes > 0) ? (post.getPollVotes1() * 100 / totalVotes) : 50;
                int pct2 = (totalVotes > 0) ? (post.getPollVotes2() * 100 / totalVotes) : 50;

                // Option 1
                DrawRectangleRounded(Rectangle{ 35, currentY + 95, 410, 36 }, 0.25f, 4, Color{ 235, 245, 255, 255 });
                DrawRectangleRounded(Rectangle{ 35, currentY + 95, (float)(410 * pct1 / 100), 36 }, 0.25f, 4, Color{ 190, 220, 255, 255 });
                DrawTextEx(font, post.getPollOption1().c_str(), Vector2{ 48, currentY + 104 }, 13, 1.0f, darkNavy);
                DrawTextEx(font, (std::to_string(pct1) + "%").c_str(), Vector2{ 400, currentY + 104 }, 13, 1.0f, fastBlue);

                // Option 2
                DrawRectangleRounded(Rectangle{ 35, currentY + 140, 410, 36 }, 0.25f, 4, Color{ 255, 242, 242, 255 });
                DrawRectangleRounded(Rectangle{ 35, currentY + 140, (float)(410 * pct2 / 100), 36 }, 0.25f, 4, Color{ 255, 210, 210, 255 });
                DrawTextEx(font, post.getPollOption2().c_str(), Vector2{ 48, currentY + 149 }, 13, 1.0f, darkNavy);
                DrawTextEx(font, (std::to_string(pct2) + "%").c_str(), Vector2{ 400, currentY + 149 }, 13, 1.0f, Color{ 220, 60, 60, 255 });

                std::string vInfo = std::to_string(totalVotes) + " votes total • Tap option to vote!";
                DrawTextEx(font, vInfo.c_str(), Vector2{ 35, currentY + 185 }, 11, 1.0f, warmGray);
            }
            else if (post.getKind() == POST_KIND_EVENT) {
                // EVENT POST (Section 6.13 X3)
                DrawRectangleRounded(Rectangle{ 35, currentY + 52, 60, 52 }, 0.2f, 4, Color{ 255, 230, 220, 255 });
                DrawTextEx(font, "14", Vector2{ 50, currentY + 58 }, 20, 1.0f, Color{ 200, 60, 20, 255 });
                DrawTextEx(font, "MAR", Vector2{ 48, currentY + 84 }, 11, 1.0f, Color{ 200, 60, 20, 255 });

                DrawTextEx(font, post.getContent().c_str(), Vector2{ 105, currentY + 52 }, 15, 1.0f, darkNavy);
                std::string venueStr = "📍 " + post.getEventVenue() + " • " + post.getEventDate();
                DrawTextEx(font, venueStr.c_str(), Vector2{ 105, currentY + 75 }, 12, 1.0f, warmGray);

                // RSVP Buttons
                DrawRectangleRounded(Rectangle{ 35, currentY + 115, 120, 32 }, 0.3f, 4, Color{ 230, 250, 235, 255 });
                std::string goingStr = "✓ Going (" + std::to_string(post.getRsvpGoing()) + ")";
                DrawTextEx(font, goingStr.c_str(), Vector2{ 50, currentY + 122 }, 12, 1.0f, Color{ 30, 140, 60, 255 });

                DrawRectangleRounded(Rectangle{ 165, currentY + 115, 120, 32 }, 0.3f, 4, Color{ 255, 248, 225, 255 });
                std::string maybeStr = "? Maybe (" + std::to_string(post.getRsvpMaybe()) + ")";
                DrawTextEx(font, maybeStr.c_str(), Vector2{ 180, currentY + 122 }, 12, 1.0f, Color{ 180, 120, 20, 255 });

                DrawRectangleRounded(Rectangle{ 295, currentY + 115, 120, 32 }, 0.3f, 4, Color{ 250, 235, 235, 255 });
                std::string cantStr = "✕ Can't (" + std::to_string(post.getRsvpCant()) + ")";
                DrawTextEx(font, cantStr.c_str(), Vector2{ 310, currentY + 122 }, 12, 1.0f, Color{ 180, 50, 50, 255 });
            }
            else {
                // TEXT POST (Section 6.2 P2)
                DrawTextEx(font, post.getContent().c_str(), Vector2{ 35, currentY + 60.0f }, 15, 1.0f, darkNavy);

                bool isLiked = currentUser ? post.isLikedBy(currentUser->getUsername()) : false;
                std::string heart = isLiked ? "♥ " : "♡ ";
                std::string likeStr = heart + std::to_string(post.getLikeCount()) + " likes";
                Color heartColor = isLiked ? Color{ 235, 55, 75, 255 } : darkNavy;
                DrawTextEx(font, likeStr.c_str(), Vector2{ 35, currentY + 190.0f }, 14, 1.0f, heartColor);

                std::string commentStr = "💬 " + std::to_string(post.getCommentCount());
                DrawTextEx(font, commentStr.c_str(), Vector2{ 115, currentY + 190.0f }, 14, 1.0f, darkNavy);
            }
        }
        currentY += cardH + 18.0f;
    }

    EndScissorMode();

    // --- 4. Bottom Tab Bar ---
    DrawRectangle(0, 745, 480, 75, Color{ 255, 255, 255, 255 });
    DrawLine(0, 745, 480, 745, Color{ 230, 225, 215, 255 });
    feedNavBtn.draw();
    newPostBtn.draw();
    chatNavBtn.draw();
    profileNavBtn.draw();

    // --- 5. New Post Modal (C1, C2, C3 Crop & C4 Filters) ---
    if (isCreatePostModalOpen) {
        DrawRectangle(0, 0, 480, 820, Color{ 0, 0, 0, 160 });
        DrawRectangleRounded(Rectangle{ 20, 110, 440, 560 }, 0.03f, 8, Color{ 252, 250, 246, 255 });

        DrawTextEx(font, "Create New Post", Vector2{ 40, 130 }, 22, 1.0f, darkNavy);
        DrawTextEx(font, "Choose kind, crop/filter photo, and share to FASTagram!", Vector2{ 40, 154 }, 11, 1.0f, warmGray);

        tabPhotoBtn.draw();
        tabTextBtn.draw();
        tabPollBtn.draw();
        tabEventBtn.draw();

        if (activePostTab == 0) {
            photoPathBox.draw();

            // Live Square Crop Preview Box (Section 6.4 C3)
            Rectangle previewRect = { 130.0f, 280.0f, 220.0f, 180.0f };
            DrawRectangleRec(previewRect, Color{ 240, 235, 225, 255 });

            Texture2D prevTex = getOrLoadTexture(photoPathBox.getText());
            Color pTint = WHITE;
            if (activeFilter == "Grayscale") pTint = Color{ 170, 170, 170, 255 };
            else if (activeFilter == "Warm") pTint = Color{ 255, 235, 210, 255 };
            else if (activeFilter == "Sepia") pTint = Color{ 240, 220, 190, 255 };

            if (prevTex.id != 0) {
                Rectangle srcR = { 0, 0, (float)prevTex.width, (float)prevTex.height };
                DrawTexturePro(prevTex, srcR, previewRect, Vector2{ 0, 0 }, 0.0f, pTint);
            } else {
                DrawTextEx(font, "Drop Image or enter path\n(Supports JPG / PNG)", Vector2{ 150, 355 }, 12, 1.0f, warmGray);
            }
            DrawRectangleLinesEx(previewRect, 2.0f, fastBlue);

            // Filter Chips (Section 6.4 C4)
            filterNormalBtn.draw();
            filterGrayBtn.draw();
            filterWarmBtn.draw();
            filterContrastBtn.draw();
            filterSepiaBtn.draw();

            photoCaptionBox.draw();
        }
        else if (activePostTab == 1) {
            photoCaptionBox.draw();
        }
        else if (activePostTab == 2) {
            pollQuestionBox.draw();
            pollOpt1Box.draw();
            pollOpt2Box.draw();
        }
        else if (activePostTab == 3) {
            eventTitleBox.draw();
            eventDateBox.draw();
            eventVenueBox.draw();
            eventDetailsBox.draw();
        }

        submitPostBtn.draw();
        cancelPostBtn.draw();
    }

    // --- 6. Comments Dialog ---
    if (activeCommentPostId >= 0) {
        DrawRectangle(0, 0, 480, 820, Color{ 0, 0, 0, 150 });
        DrawRectangleRounded(Rectangle{ 20, 120, 440, 580 }, 0.03f, 8, WHITE);

        DrawTextEx(font, "Comments", Vector2{ 40, 140 }, 20, 1.0f, darkNavy);
        closeCommentBtn.draw();
        DrawLine(20, 180, 460, 180, Color{ 230, 225, 215, 255 });

        Post* p = postManager.getPostById(activeCommentPostId);
        if (p) {
            const auto& cmts = p->getComments();
            float cy = 195.0f;
            if (cmts.empty()) {
                DrawTextEx(font, "No comments yet. Be the first to comment!", Vector2{ 40, 250 }, 14, 1.0f, warmGray);
            }
            for (size_t c = 0; c < cmts.size() && c < 6; ++c) {
                DrawCircle(55, (int)(cy + 12), 12, Color{ 235, 240, 250, 255 });
                std::string cInit = cmts[c].getAuthorName().empty() ? "U" : cmts[c].getAuthorName().substr(0, 1);
                DrawTextEx(font, cInit.c_str(), Vector2{ 50, cy + 3 }, 13, 1.0f, fastBlue);

                DrawTextEx(font, cmts[c].getAuthorName().c_str(), Vector2{ 75, cy }, 14, 1.0f, darkNavy);
                DrawTextEx(font, cmts[c].getText().c_str(), Vector2{ 75, cy + 18 }, 12, 1.0f, warmGray);
                cy += 50.0f;
            }
        }

        commentInputBox.draw();
        sendCommentBtn.draw();
    }

    // --- 7. Fullscreen Story Viewer ---
    if (activeStoryIndex >= 0 && activeStoryIndex < (int)stories.size()) {
        const auto& st = stories[activeStoryIndex];
        DrawRectangle(0, 0, 480, 820, st.color);

        // Progress bar
        float pWidth = 440.0f * (storyTimer / 4.0f);
        DrawRectangle(20, 15, 440, 4, Color{ 0, 0, 0, 50 });
        DrawRectangle(20, 15, (int)pWidth, 4, darkNavy);

        DrawCircle(50, 45, 18, WHITE);
        std::string sInit = st.authorName.empty() ? "U" : st.authorName.substr(0, 1);
        DrawTextEx(font, sInit.c_str(), Vector2{ 44, 34 }, 18, 1.0f, darkNavy);

        DrawTextEx(font, st.authorName.c_str(), Vector2{ 80, 36 }, 16, 1.0f, darkNavy);
        DrawTextEx(font, st.timestamp.c_str(), Vector2{ 80, 56 }, 12, 1.0f, warmGray);
        closeStoryBtn.draw();

        DrawTextEx(font, st.message.c_str(), Vector2{ 40, 380 }, 22, 1.0f, darkNavy);
    }
}
