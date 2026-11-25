#include "StorageManager.h"
#include <fstream>
#include <iostream>

StorageManager::StorageManager(const std::string& directory)
    : dataDir(directory),
      configPath("config.txt"),
      usersDatPath("users.dat"),
      postsDatPath("posts.dat"),
      messagesDatPath("messages.dat")
{
}

bool StorageManager::loadConfig(std::string& activeSessionRoll, bool& isDarkMode) {
    activeSessionRoll = "";
    isDarkMode = false;

    std::ifstream in(configPath);
    if (!in.is_open()) return false;

    std::string line;
    while (std::getline(in, line)) {
        if (line.find("active_session_user=") == 0) {
            activeSessionRoll = line.substr(20);
        } else if (line.find("theme=") == 0) {
            std::string theme = line.substr(6);
            isDarkMode = (theme == "DARK");
        }
    }
    return true;
}

bool StorageManager::saveConfig(const std::string& activeSessionRoll, bool isDarkMode) {
    std::ofstream out(configPath);
    if (!out.is_open()) return false;

    out << "# FASTagram Desktop Configuration\n";
    out << "window_width=480\n";
    out << "window_height=820\n";
    out << "theme=" << (isDarkMode ? "DARK" : "LIGHT") << "\n";
    out << "active_session_user=" << activeSessionRoll << "\n";
    return true;
}

static void writeString(std::ostream& out, const std::string& s) {
    uint32_t len = (uint32_t)s.length();
    out.write((char*)&len, sizeof(len));
    if (len > 0) {
        out.write(s.data(), len);
    }
}

static std::string readString(std::istream& in) {
    uint32_t len = 0;
    in.read((char*)&len, sizeof(len));
    if (len == 0 || in.eof()) return "";
    std::string s(len, '\0');
    in.read(&s[0], len);
    return s;
}

bool StorageManager::saveBinaryData(const UserManager& userManager, 
                                   const PostManager& postManager, 
                                   const ChatManager& chatManager)
{
    // --- 1. Save users.dat with FTG1 magic header ---
    {
        std::ofstream out(usersDatPath, std::ios::binary);
        if (out.is_open()) {
            const char magic[4] = { 'F', 'T', 'G', '1' };
            uint32_t version = 1;
            const auto& accounts = userManager.getAllAccounts();
            uint32_t count = (uint32_t)accounts.size();

            out.write(magic, 4);
            out.write((char*)&version, sizeof(version));
            out.write((char*)&count, sizeof(count));

            for (size_t i = 0; i < accounts.size(); ++i) {
                const Account* acc = accounts[i];
                int id = acc->getId();
                int type = (int)acc->getType();
                out.write((char*)&id, sizeof(id));
                out.write((char*)&type, sizeof(type));
                writeString(out, acc->getUsername());
                writeString(out, acc->getDisplayName());
                writeString(out, acc->getBio());
            }
        }
    }

    // --- 2. Save posts.dat with FTG1 magic header ---
    {
        std::ofstream out(postsDatPath, std::ios::binary);
        if (out.is_open()) {
            const char magic[4] = { 'F', 'T', 'G', '1' };
            uint32_t version = 1;
            const auto& posts = postManager.getAllPosts();
            uint32_t count = (uint32_t)posts.size();

            out.write(magic, 4);
            out.write((char*)&version, sizeof(version));
            out.write((char*)&count, sizeof(count));

            for (size_t i = 0; i < posts.size(); ++i) {
                const Post& p = posts[i];
                int id = p.getId();
                int kind = (int)p.getKind();
                int likes = p.getLikeCount();
                int pVotes1 = p.getPollVotes1();
                int pVotes2 = p.getPollVotes2();
                int rGoing = p.getRsvpGoing();
                int rMaybe = p.getRsvpMaybe();
                int rCant = p.getRsvpCant();

                out.write((char*)&id, sizeof(id));
                out.write((char*)&kind, sizeof(kind));
                writeString(out, p.getAuthorRoll());
                writeString(out, p.getAuthorName());
                writeString(out, p.getContent());
                writeString(out, p.getTimestamp());
                writeString(out, p.getImagePath());
                writeString(out, p.getFilterName());
                writeString(out, p.getPollOption1());
                writeString(out, p.getPollOption2());
                out.write((char*)&pVotes1, sizeof(pVotes1));
                out.write((char*)&pVotes2, sizeof(pVotes2));
                writeString(out, p.getEventDate());
                writeString(out, p.getEventVenue());
                out.write((char*)&rGoing, sizeof(rGoing));
                out.write((char*)&rMaybe, sizeof(rMaybe));
                out.write((char*)&rCant, sizeof(rCant));
                out.write((char*)&likes, sizeof(likes));
            }
        }
    }

    // --- 3. Save messages.dat with FTG1 magic header ---
    {
        std::ofstream out(messagesDatPath, std::ios::binary);
        if (out.is_open()) {
            const char magic[4] = { 'F', 'T', 'G', '1' };
            uint32_t version = 1;
            const auto& msgs = chatManager.getAllMessages();
            uint32_t count = (uint32_t)msgs.size();

            out.write(magic, 4);
            out.write((char*)&version, sizeof(version));
            out.write((char*)&count, sizeof(count));

            for (size_t i = 0; i < msgs.size(); ++i) {
                const Message& m = msgs[i];
                int id = m.getId();
                out.write((char*)&id, sizeof(id));
                writeString(out, m.getSenderRoll());
                writeString(out, m.getReceiverRoll());
                writeString(out, m.getText());
                writeString(out, m.getTimestamp());
            }
        }
    }

    return true;
}

bool StorageManager::loadBinaryData(UserManager& userManager, 
                                   PostManager& postManager, 
                                   ChatManager& chatManager)
{
    // --- 1. Load users.dat ---
    std::ifstream userIn(usersDatPath, std::ios::binary);
    if (userIn.is_open()) {
        char magic[4];
        userIn.read(magic, 4);
        if (magic[0] == 'F' && magic[1] == 'T' && magic[2] == 'G' && magic[3] == '1') {
            uint32_t version = 0, count = 0;
            userIn.read((char*)&version, sizeof(version));
            userIn.read((char*)&count, sizeof(count));

            for (uint32_t i = 0; i < count; ++i) {
                int id = 0, type = 0;
                userIn.read((char*)&id, sizeof(id));
                userIn.read((char*)&type, sizeof(type));
                std::string username = readString(userIn);
                std::string displayName = readString(userIn);
                std::string bio = readString(userIn);

                // Register if not already registered
                if (!username.empty() && userManager.findByUsername(username) == nullptr) {
                    userManager.registerUser(username, displayName, "fast123", (AccountType)type);
                    Account* a = userManager.findByUsername(username);
                    if (a) a->setBio(bio);
                }
            }
        }
    }

    // --- 2. Load posts.dat ---
    std::ifstream postIn(postsDatPath, std::ios::binary);
    if (postIn.is_open()) {
        char magic[4];
        postIn.read(magic, 4);
        if (magic[0] == 'F' && magic[1] == 'T' && magic[2] == 'G' && magic[3] == '1') {
            uint32_t version = 0, count = 0;
            postIn.read((char*)&version, sizeof(version));
            postIn.read((char*)&count, sizeof(count));

            if (count > 0) {
                postManager.clear();
                for (uint32_t i = 0; i < count; ++i) {
                    int id = 0, kind = 0;
                    postIn.read((char*)&id, sizeof(id));
                    postIn.read((char*)&kind, sizeof(kind));
                    std::string authorRoll = readString(postIn);
                    std::string authorName = readString(postIn);
                    std::string content = readString(postIn);
                    std::string timestamp = readString(postIn);
                    std::string imagePath = readString(postIn);
                    std::string filterName = readString(postIn);
                    std::string opt1 = readString(postIn);
                    std::string opt2 = readString(postIn);
                    int pVotes1 = 0, pVotes2 = 0;
                    postIn.read((char*)&pVotes1, sizeof(pVotes1));
                    postIn.read((char*)&pVotes2, sizeof(pVotes2));
                    std::string evDate = readString(postIn);
                    std::string evVenue = readString(postIn);
                    int rGoing = 0, rMaybe = 0, rCant = 0, likes = 0;
                    postIn.read((char*)&rGoing, sizeof(rGoing));
                    postIn.read((char*)&rMaybe, sizeof(rMaybe));
                    postIn.read((char*)&rCant, sizeof(rCant));
                    postIn.read((char*)&likes, sizeof(likes));

                    Post p(id, authorRoll, authorName, content, timestamp);
                    p.setKind((PostKind)kind);
                    p.setImagePath(imagePath);
                    p.setFilterName(filterName);
                    postManager.addPost(p);
                }
            }
        }
    }

    return true;
}
