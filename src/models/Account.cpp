#include "Account.h"

static void writeString(std::ostream& out, const std::string& str) {
    size_t len = str.size();
    out.write(reinterpret_cast<const char*>(&len), sizeof(len));
    if (len > 0) {
        out.write(str.data(), len);
    }
}

static std::string readString(std::istream& in) {
    size_t len = 0;
    in.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (len == 0) return "";
    std::string str(len, '\0');
    in.read(&str[0], len);
    return str;
}

Account::Account(int id, const std::string& username, const std::string& displayName,
                 const std::string& password, AccountType type)
    : id(id),
      username(username),
      displayName(displayName),
      passwordHash(password),
      bio("Hey there! Using FASTagram."),
      avatarPath("resources/default_avatar.png"),
      type(type),
      isPrivate(false)
{
}

bool Account::checkPassword(const std::string& inputPassword) const {
    return passwordHash == inputPassword;
}

void Account::write(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&id), sizeof(id));
    out.write(reinterpret_cast<const char*>(&type), sizeof(type));
    out.write(reinterpret_cast<const char*>(&isPrivate), sizeof(isPrivate));

    writeString(out, username);
    writeString(out, displayName);
    writeString(out, passwordHash);
    writeString(out, bio);
    writeString(out, avatarPath);
}

void Account::read(std::istream& in) {
    in.read(reinterpret_cast<char*>(&id), sizeof(id));
    in.read(reinterpret_cast<char*>(&type), sizeof(type));
    in.read(reinterpret_cast<char*>(&isPrivate), sizeof(isPrivate));

    username = readString(in);
    displayName = readString(in);
    passwordHash = readString(in);
    bio = readString(in);
    avatarPath = readString(in);
}
