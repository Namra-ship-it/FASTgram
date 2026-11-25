#include "Message.h"

Message::Message()
    : id(0), senderRoll(""), receiverRoll(""), text(""), timestamp(""), isRead(false)
{
}

Message::Message(int id, const std::string& sender, const std::string& receiver, 
                 const std::string& text, const std::string& time, bool isRead)
    : id(id), senderRoll(sender), receiverRoll(receiver), text(text), timestamp(time), isRead(isRead)
{
}

void Message::write(std::ostream& out) const {
    out << id << "\n"
        << senderRoll << "\n"
        << receiverRoll << "\n"
        << text << "\n"
        << timestamp << "\n"
        << (isRead ? 1 : 0) << "\n";
}

void Message::read(std::istream& in) {
    in >> id;
    in.ignore();
    std::getline(in, senderRoll);
    std::getline(in, receiverRoll);
    std::getline(in, text);
    std::getline(in, timestamp);
    int readVal = 0;
    in >> readVal;
    isRead = (readVal == 1);
}
