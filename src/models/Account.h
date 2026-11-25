#pragma once
#include "Serializable.h"
#include <string>

enum AccountType {
    ACCOUNT_STUDENT,
    ACCOUNT_FACULTY,
    ACCOUNT_SOCIETY,
    ACCOUNT_ADMIN,
    ACCOUNT_BOT
};

class Account : public Serializable {
protected:
    int id;
    std::string username;
    std::string displayName;
    std::string passwordHash;
    std::string bio;
    std::string avatarPath;
    AccountType type;
    bool isPrivate;

public:
    Account(int id, const std::string& username, const std::string& displayName,
            const std::string& password, AccountType type = ACCOUNT_STUDENT);

    virtual ~Account() = default;

    bool checkPassword(const std::string& inputPassword) const;

    int getId() const { return id; }
    std::string getUsername() const { return username; }
    std::string getDisplayName() const { return displayName; }
    std::string getBio() const { return bio; }
    std::string getAvatarPath() const { return avatarPath; }
    AccountType getType() const { return type; }
    bool getIsPrivate() const { return isPrivate; }

    void setDisplayName(const std::string& name) { displayName = name; }
    void setBio(const std::string& newBio) { bio = newBio; }
    void setAvatarPath(const std::string& path) { avatarPath = path; }
    void setIsPrivate(bool priv) { isPrivate = priv; }

    void write(std::ostream& out) const override;
    void read(std::istream& in) override;
};
