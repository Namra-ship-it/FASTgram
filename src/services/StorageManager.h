#pragma once
#include "services/UserManager.h"
#include "services/PostManager.h"
#include "services/ChatManager.h"
#include <string>

class StorageManager {
private:
    std::string dataDir;
    std::string configPath;
    std::string usersDatPath;
    std::string postsDatPath;
    std::string messagesDatPath;

public:
    StorageManager(const std::string& directory = "data");

    // Config & Session Persistence
    bool loadConfig(std::string& activeSessionRoll, bool& isDarkMode);
    bool saveConfig(const std::string& activeSessionRoll, bool isDarkMode);

    // Binary File Handling (.dat with FTG1 Magic Header)
    bool saveBinaryData(const UserManager& userManager, 
                        const PostManager& postManager, 
                        const ChatManager& chatManager);

    bool loadBinaryData(UserManager& userManager, 
                        PostManager& postManager, 
                        ChatManager& chatManager);

    // Helpers
    bool saveAll(const UserManager& userManager, const PostManager& postManager, const ChatManager& chatManager) {
        return saveBinaryData(userManager, postManager, chatManager);
    }
    bool loadAll(UserManager& userManager, PostManager& postManager, ChatManager& chatManager) {
        return loadBinaryData(userManager, postManager, chatManager);
    }
};
