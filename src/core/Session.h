#pragma once
#include "Account.h"

class Session {
private:
    Account* currentUser;
    Session();

public:
    static Session& getInstance();

    void login(Account* user);
    void logout();
    bool isLoggedIn() const;
    Account* getCurrentUser() const;
    int getCurrentUserId() const;
};
