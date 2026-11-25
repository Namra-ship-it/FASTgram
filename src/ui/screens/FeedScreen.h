#pragma once
#include "raylib.h"
#include "services/PostManager.h"
#include "models/Account.h"
#include "ui/widgets/Button.h"
#include "ui/widgets/TextBox.h"
#include <string>
#include <vector>

struct StoryItem {
    std::string authorName;
    std::string timestamp;
    std::string message;
    Color color;
};

class FeedScreen {
private:
    float scrollOffset;
    Button feedNavBtn;
    Button newPostBtn;
    Button chatNavBtn;
    Button profileNavBtn;

    // Top action buttons
    Button searchTopBtn;
    Button notifTopBtn;

    // Texture Cache
    std::vector<std::pair<std::string, Texture2D>> textureCache;
    Texture2D getOrLoadTexture(const std::string& path);

    // Create Post Modal State (Section 6.4: C1, C2, C3 Crop & C4 Filters)
    bool isCreatePostModalOpen;
    int activePostTab; // 0=Photo, 1=Text, 2=Poll, 3=Event
    Button tabPhotoBtn;
    Button tabTextBtn;
    Button tabPollBtn;
    Button tabEventBtn;

    // Photo Post Inputs & Filters
    TextBox photoPathBox;
    TextBox photoCaptionBox;
    std::string activeFilter;
    Button filterNormalBtn;
    Button filterGrayBtn;
    Button filterWarmBtn;
    Button filterContrastBtn;
    Button filterSepiaBtn;

    // Poll Inputs
    TextBox pollQuestionBox;
    TextBox pollOpt1Box;
    TextBox pollOpt2Box;

    // Event Inputs
    TextBox eventTitleBox;
    TextBox eventDateBox;
    TextBox eventVenueBox;
    TextBox eventDetailsBox;

    // Modal Actions
    Button submitPostBtn;
    Button cancelPostBtn;

    // Comments Modal State
    int activeCommentPostId;
    TextBox commentInputBox;
    Button sendCommentBtn;
    Button closeCommentBtn;

    // Notifications Modal State
    bool isNotificationsOpen;
    Button closeNotifBtn;

    // Story Viewer State
    int activeStoryIndex;
    float storyTimer;
    Button closeStoryBtn;
    std::vector<StoryItem> stories;

public:
    FeedScreen();
    ~FeedScreen();

    void update(PostManager& postManager, const Account* currentUser, 
                bool& shouldOpenProfile, bool& shouldOpenChat, bool& shouldOpenSearch);
    void draw(PostManager& postManager, const Account* currentUser);

    void reset();
};
