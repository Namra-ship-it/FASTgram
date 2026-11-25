#include "ChatManager.h"

ChatManager::ChatManager()
    : nextMessageId(1)
{
    seedMockMessages();
}

void ChatManager::seedMockMessages() {
    // Conversation between Sara (i261234) and Ali (i265544)
    sendMessage("i265544", "i261234", "Hey Sara! Did you finish the raylib OOP assignment?", "10:30 AM");
    sendMessage("i261234", "i265544", "Yes! Just finished building the cute mascot and feed!", "10:32 AM");
    sendMessage("i265544", "i261234", "That looks so cool! Can we test chatting together?", "10:35 AM");

    // Conversation between Sara (i261234) and Zainab (i267788)
    sendMessage("i267788", "i261234", "Are we meeting in the library at 3 PM?", "Yesterday");
    sendMessage("i261234", "i267788", "Yes, see you near the second floor study cubicles!", "Yesterday");
}

void ChatManager::sendMessage(const std::string& senderRoll, const std::string& receiverRoll, 
                             const std::string& text, const std::string& time)
{
    messages.push_back(Message(nextMessageId++, senderRoll, receiverRoll, text, time, false));
}

std::vector<Message> ChatManager::getConversation(const std::string& user1, const std::string& user2) const {
    std::vector<Message> conv;
    for (const auto& msg : messages) {
        if ((msg.getSenderRoll() == user1 && msg.getReceiverRoll() == user2) ||
            (msg.getSenderRoll() == user2 && msg.getReceiverRoll() == user1)) 
        {
            conv.push_back(msg);
        }
    }
    return conv;
}

std::vector<std::string> ChatManager::getChatPartners(const std::string& userRoll) const {
    std::vector<std::string> partners;
    for (size_t i = 0; i < messages.size(); ++i) {
        const Message& msg = messages[i];
        std::string other = "";
        if (msg.getSenderRoll() == userRoll) other = msg.getReceiverRoll();
        else if (msg.getReceiverRoll() == userRoll) other = msg.getSenderRoll();

        if (!other.empty()) {
            bool found = false;
            for (size_t p = 0; p < partners.size(); ++p) {
                if (partners[p] == other) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                partners.push_back(other);
            }
        }
    }
    return partners;
}

int ChatManager::getUnreadCount(const std::string& userRoll) const {
    int count = 0;
    for (const auto& msg : messages) {
        if (msg.getReceiverRoll() == userRoll && !msg.getIsRead()) {
            count++;
        }
    }
    return count;
}
