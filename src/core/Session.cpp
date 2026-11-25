#include "Session.h"

Session::Session() : currentUser(nullptr) {}

Session& Session::getInstance() {
    static Session instance;
    return instance;
}

void Session::login(Account* user) {
    currentUser = user;
}

void Session::logout() {
    currentUser = nullptr;
}

bool Session::isLoggedIn() const {
    return currentUser != nullptr;
}

Account* Session::getCurrentUser() const {
    return currentUser;
}

int Session::getCurrentUserId() const {
    return currentUser ? currentUser->getId() : -1;
}
