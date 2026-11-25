#pragma once
#include "models/Message.h"
#include <vector>
#include <string>

class ChatManager {
private:
    std::vector<Message> messages;
    int nextMessageId;

    void seedMockMessages();

public:
    ChatManager();

    void sendMessage(const std::string& senderRoll, const std::string& receiverRoll, 
                     const std::string& text, const std::string& time = "Just now");

    std::vector<Message> getConversation(const std::string& user1, const std::string& user2) const;
    std::vector<std::string> getChatPartners(const std::string& userRoll) const;
    int getUnreadCount(const std::string& userRoll) const;

    const std::vector<Message>& getAllMessages() const { return messages; }
    void clear() { messages.clear(); }
    void addMessage(const Message& msg) { messages.push_back(msg); }
};
