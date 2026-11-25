#pragma once
#include <string>

class Comment {
private:
    std::string authorRoll;
    std::string authorName;
    std::string text;
    std::string timestamp;

public:
    Comment() = default;
    Comment(const std::string& roll, const std::string& name, const std::string& commentText, const std::string& time = "Just now")
        : authorRoll(roll), authorName(name), text(commentText), timestamp(time) {}

    const std::string& getAuthorRoll() const { return authorRoll; }
    const std::string& getAuthorName() const { return authorName; }
    const std::string& getText() const { return text; }
    const std::string& getTimestamp() const { return timestamp; }
};
