#include "ChatScreen.h"

ChatScreen::ChatScreen()
    : activePartnerRoll("i265544"),
      activePartnerName("Ali Khan"),
      backBtn(20.0f, 25.0f, 75.0f, 36.0f, "← Feed", STYLE_OUTLINE_BEIGE),
      messageInput(20.0f, 745.0f, 360.0f, 48.0f, "Message classmate..."),
      sendBtn(390.0f, 745.0f, 70.0f, 48.0f, "Send", STYLE_CUTE_YELLOW),
      partner1Btn(20.0f, 75.0f, 135.0f, 34.0f, "👤 Ali Khan", STYLE_PRIMARY_DARK),
      partner2Btn(165.0f, 75.0f, 145.0f, 34.0f, "👤 Zainab M.", STYLE_OUTLINE_BEIGE),
      partner3Btn(320.0f, 75.0f, 140.0f, 34.0f, "👤 Hamza T.", STYLE_OUTLINE_BEIGE),
      scrollOffset(0.0f)
{
}

void ChatScreen::reset() {
    messageInput.clear();
    scrollOffset = 0.0f;
}

void ChatScreen::setActivePartner(const std::string& roll, const std::string& name) {
    activePartnerRoll = roll;
    activePartnerName = name;
    scrollOffset = 0.0f;

    partner1Btn.setStyle(activePartnerRoll == "i265544" ? STYLE_PRIMARY_DARK : STYLE_OUTLINE_BEIGE);
    partner2Btn.setStyle(activePartnerRoll == "i267788" ? STYLE_PRIMARY_DARK : STYLE_OUTLINE_BEIGE);
    partner3Btn.setStyle(activePartnerRoll == "i269900" ? STYLE_PRIMARY_DARK : STYLE_OUTLINE_BEIGE);
}

void ChatScreen::update(ChatManager& chatManager, const Account* currentUser, bool& shouldGoBack) {
    backBtn.update();
    partner1Btn.update();
    partner2Btn.update();
    partner3Btn.update();
    messageInput.update();
    sendBtn.update();

    if (backBtn.isClicked()) {
        shouldGoBack = true;
        return;
    }

    // Switch active chat partner
    if (partner1Btn.isClicked()) {
        setActivePartner("i265544", "Ali Khan");
    }
    if (partner2Btn.isClicked()) {
        setActivePartner("i267788", "Zainab Malik");
    }
    if (partner3Btn.isClicked()) {
        setActivePartner("i269900", "Hamza Tariq");
    }

    // Send message logic
    if ((sendBtn.isClicked() || (messageInput.getVisible() && IsKeyPressed(KEY_ENTER))) && currentUser) {
        std::string text = messageInput.getText();
        if (!text.empty()) {
            chatManager.sendMessage(currentUser->getUsername(), activePartnerRoll, text, "Just now");
            messageInput.clear();
            scrollOffset = 0.0f;
        }
    }

    // Scroll conversation with mouse wheel
    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f) {
        scrollOffset += wheel * 30.0f;
        if (scrollOffset > 0.0f) scrollOffset = 0.0f;
    }
}

void ChatScreen::draw(const ChatManager& chatManager, const Account* currentUser) {
    Font font = Widget::getFont();
    Color darkNavy = Color{ 22, 32, 52, 255 };
    Color warmGray = Color{ 110, 115, 125, 255 };

    // --- 1. Top Bar ---
    DrawRectangle(0, 0, 480, 70, Color{ 250, 246, 238, 255 });
    backBtn.draw();

    // Active partner name & online indicator
    DrawTextEx(font, activePartnerName.c_str(), Vector2{ 110, 22 }, 20, 1.0f, darkNavy);
    DrawCircle(114, 48, 4.0f, Color{ 35, 185, 80, 255 }); // Green online dot
    std::string statusStr = "Active now • " + activePartnerRoll;
    DrawTextEx(font, statusStr.c_str(), Vector2{ 124, 42 }, 12, 1.0f, warmGray);

    DrawLine(0, 70, 480, 70, Color{ 230, 225, 215, 255 });

    // --- 2. Partners Tab Selector ---
    DrawRectangle(0, 70, 480, 46, Color{ 255, 255, 255, 230 });
    partner1Btn.draw();
    partner2Btn.draw();
    partner3Btn.draw();
    DrawLine(0, 116, 480, 116, Color{ 230, 225, 215, 255 });

    // --- 3. Chat Messages Thread ---
    BeginScissorMode(0, 120, 480, 610);

    if (currentUser) {
        auto conv = chatManager.getConversation(currentUser->getUsername(), activePartnerRoll);

        float bubbleY = 135.0f + scrollOffset;

        if (conv.empty()) {
            DrawTextEx(font, "No messages yet. Say hi! 👋", Vector2{ 150, 250 }, 16, 1.0f, warmGray);
        }

        for (const auto& msg : conv) {
            bool isMe = (msg.getSenderRoll() == currentUser->getUsername());

            Vector2 textSize = MeasureTextEx(font, msg.getText().c_str(), 16, 1.0f);
            float bubbleW = textSize.x + 28.0f;
            if (bubbleW < 60.0f) bubbleW = 60.0f;
            if (bubbleW > 330.0f) bubbleW = 330.0f;
            float bubbleH = textSize.y + 18.0f;

            float bubbleX = isMe ? (480.0f - bubbleW - 20.0f) : 20.0f;
            Rectangle bubbleRect = { bubbleX, bubbleY, bubbleW, bubbleH };

            if (isMe) {
                // Outgoing message (You) -> Cute Deep Navy Pill
                DrawRectangleRounded(bubbleRect, 0.45f, 8, Color{ 22, 32, 52, 255 });
                DrawTextEx(font, msg.getText().c_str(), Vector2{ bubbleX + 14, bubbleY + 9 }, 16, 1.0f, WHITE);
                
                // Time
                DrawTextEx(font, msg.getTimestamp().c_str(), Vector2{ bubbleX + bubbleW - 40, bubbleY + bubbleH + 3 }, 10, 1.0f, warmGray);
            } else {
                // Incoming message (Classmate) -> Clean White/Cream Pill with border
                DrawRectangleRounded(bubbleRect, 0.45f, 8, Color{ 255, 255, 255, 245 });
                DrawRectangleRoundedLinesEx(bubbleRect, 0.45f, 8, 1.5f, Color{ 220, 215, 202, 255 });
                DrawTextEx(font, msg.getText().c_str(), Vector2{ bubbleX + 14, bubbleY + 9 }, 16, 1.0f, darkNavy);

                // Time
                DrawTextEx(font, msg.getTimestamp().c_str(), Vector2{ bubbleX + 10, bubbleY + bubbleH + 3 }, 10, 1.0f, warmGray);
            }

            bubbleY += bubbleH + 24.0f;
        }
    }

    EndScissorMode();

    // --- 4. Bottom Input Bar ---
    DrawRectangle(0, 735, 480, 85, Color{ 255, 255, 255, 245 });
    DrawLine(0, 735, 480, 735, Color{ 225, 220, 210, 255 });

    messageInput.draw();
    sendBtn.draw();
}
