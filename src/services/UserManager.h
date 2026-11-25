#pragma once
#include "Account.h"
#include <vector>
#include <string>
#include <utility>

class UserManager {
private:
    std::vector<Account*> accounts;
    std::vector<std::pair<std::string, std::string>> followGraph; // pair<followerRoll, followedRoll>
    int nextUserId;

public:
    UserManager();
    ~UserManager();

    bool registerUser(const std::string& username, const std::string& displayName, 
                      const std::string& password, AccountType type = ACCOUNT_STUDENT);

    Account* authenticate(const std::string& username, const std::string& password);

    Account* findById(int id) const;
    Account* findByUsername(const std::string& username) const;

    std::vector<Account*> searchUsers(const std::string& query) const;

    // Follow System
    bool toggleFollow(const std::string& followerRoll, const std::string& followedRoll);
    bool isFollowing(const std::string& followerRoll, const std::string& followedRoll) const;
    int getFollowersCount(const std::string& roll) const;
    int getFollowingCount(const std::string& roll) const;

    bool saveToFile(const std::string& filepath = "users.dat");
    bool loadFromFile(const std::string& filepath = "users.dat");

    const std::vector<Account*>& getAllAccounts() const { return accounts; }
};
