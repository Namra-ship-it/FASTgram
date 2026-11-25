#include "UserManager.h"
#include <fstream>
#include <cctype>

static std::string toLowerString(const std::string& str) {
    std::string res = str;
    for (size_t i = 0; i < res.length(); ++i) {
        if (res[i] >= 'A' && res[i] <= 'Z') {
            res[i] = (char)(res[i] + ('a' - 'A'));
        }
    }
    return res;
}

UserManager::UserManager() : nextUserId(101) {
    registerUser("i261234", "Sara Ahmed", "fast123", ACCOUNT_STUDENT);
    registerUser("i265544", "Ali Khan", "fast123", ACCOUNT_STUDENT);
    registerUser("i267788", "Zainab Malik", "fast123", ACCOUNT_STUDENT);
    registerUser("i269900", "Hamza Tariq", "fast123", ACCOUNT_STUDENT);
    registerUser("dr.usman", "Dr. Usman", "faculty123", ACCOUNT_FACULTY);
    registerUser("acm_fast", "ACM Society", "society123", ACCOUNT_SOCIETY);

    // Initial follow graph connections
    followGraph.push_back({ "i265544", "i261234" });
    followGraph.push_back({ "i267788", "i261234" });
    followGraph.push_back({ "i261234", "i265544" });
}

UserManager::~UserManager() {
    for (Account* acc : accounts) {
        delete acc;
    }
    accounts.clear();
}

bool UserManager::registerUser(const std::string& username, const std::string& displayName,
                               const std::string& password, AccountType type) {
    if (username.empty() || password.length() < 4) return false;
    if (findByUsername(username) != nullptr) return false;

    Account* newAcc = new Account(nextUserId++, username, displayName, password, type);
    accounts.push_back(newAcc);
    return true;
}

Account* UserManager::authenticate(const std::string& username, const std::string& password) {
    Account* acc = findByUsername(username);
    if (acc && acc->checkPassword(password)) {
        return acc;
    }
    return nullptr;
}

Account* UserManager::findById(int id) const {
    for (Account* acc : accounts) {
        if (acc->getId() == id) return acc;
    }
    return nullptr;
}

Account* UserManager::findByUsername(const std::string& username) const {
    for (Account* acc : accounts) {
        if (acc->getUsername() == username) return acc;
    }
    return nullptr;
}

std::vector<Account*> UserManager::searchUsers(const std::string& query) const {
    std::vector<Account*> results;
    if (query.empty()) {
        return accounts;
    }

    std::string lowerQuery = toLowerString(query);

    for (size_t i = 0; i < accounts.size(); ++i) {
        Account* acc = accounts[i];
        std::string lowerUser = toLowerString(acc->getUsername());
        std::string lowerName = toLowerString(acc->getDisplayName());

        if (lowerUser.find(lowerQuery) != std::string::npos || 
            lowerName.find(lowerQuery) != std::string::npos) 
        {
            results.push_back(acc);
        }
    }
    return results;
}

bool UserManager::toggleFollow(const std::string& followerRoll, const std::string& followedRoll) {
    if (followerRoll == followedRoll) return false;

    for (size_t i = 0; i < followGraph.size(); ++i) {
        if (followGraph[i].first == followerRoll && followGraph[i].second == followedRoll) {
            for (size_t j = i; j + 1 < followGraph.size(); ++j) {
                followGraph[j] = followGraph[j + 1];
            }
            followGraph.pop_back();
            return false; // un-followed
        }
    }

    followGraph.push_back({ followerRoll, followedRoll });
    return true; // followed
}

bool UserManager::isFollowing(const std::string& followerRoll, const std::string& followedRoll) const {
    for (size_t i = 0; i < followGraph.size(); ++i) {
        if (followGraph[i].first == followerRoll && followGraph[i].second == followedRoll) {
            return true;
        }
    }
    return false;
}

int UserManager::getFollowersCount(const std::string& roll) const {
    int count = 0;
    for (const auto& pair : followGraph) {
        if (pair.second == roll) count++;
    }
    return count;
}

int UserManager::getFollowingCount(const std::string& roll) const {
    int count = 0;
    for (const auto& pair : followGraph) {
        if (pair.first == roll) count++;
    }
    return count;
}

bool UserManager::saveToFile(const std::string& filepath) {
    std::ofstream out(filepath);
    if (!out.is_open()) return false;

    out << accounts.size() << "\n";
    for (const Account* acc : accounts) {
        out << acc->getId() << "\n"
            << acc->getUsername() << "\n"
            << acc->getDisplayName() << "\n"
            << acc->getBio() << "\n";
    }
    return true;
}

bool UserManager::loadFromFile(const std::string& filepath) {
    std::ifstream in(filepath);
    if (!in.is_open()) return false;
    return true;
}
