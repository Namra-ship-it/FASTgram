#pragma once
#include "Serializable.h"
#include <string>

class Message : public Serializable {
private:
    int id;
    std::string senderRoll;
    std::string receiverRoll;
    std::string text;
    std::string timestamp;
    bool isRead;

public:
    Message();
    Message(int id, const std::string& sender, const std::string& receiver, 
            const std::string& text, const std::string& time = "Just now", bool isRead = false);

    int getId() const { return id; }
    const std::string& getSenderRoll() const { return senderRoll; }
    const std::string& getReceiverRoll() const { return receiverRoll; }
    const std::string& getText() const { return text; }
    const std::string& getTimestamp() const { return timestamp; }
    bool getIsRead() const { return isRead; }
    void markAsRead() { isRead = true; }

    void write(std::ostream& out) const override;
    void read(std::istream& in) override;
};
