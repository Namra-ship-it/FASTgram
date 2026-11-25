#pragma once
#include "raylib.h"
#include "services/ChatManager.h"
#include "models/Account.h"
#include "ui/widgets/Button.h"
#include "ui/widgets/TextBox.h"
#include <string>

class ChatScreen {
private:
    std::string activePartnerRoll;
    std::string activePartnerName;

    Button backBtn;
    TextBox messageInput;
    Button sendBtn;

    Button partner1Btn;
    Button partner2Btn;
    Button partner3Btn;

    float scrollOffset;

public:
    ChatScreen();

    void update(ChatManager& chatManager, const Account* currentUser, bool& shouldGoBack);
    void draw(const ChatManager& chatManager, const Account* currentUser);

    void setActivePartner(const std::string& roll, const std::string& name);
    void reset();
};
