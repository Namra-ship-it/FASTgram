#include "Post.h"

Post::Post()
    : id(0), authorRoll(""), authorName(""), content(""), timestamp(""),
      cardColor(WHITE), kind(POST_KIND_TEXT), imagePath(""), filterName("Normal"),
      pollOption1(""), pollOption2(""), pollVotes1(0), pollVotes2(0),
      eventDate(""), eventVenue(""), rsvpGoing(0), rsvpMaybe(0), rsvpCant(0)
{
}

Post::Post(int id, const std::string& authorRoll, const std::string& authorName, 
           const std::string& content, const std::string& time, Color color)
    : id(id), authorRoll(authorRoll), authorName(authorName), 
      content(content), timestamp(time), cardColor(color),
      kind(POST_KIND_TEXT), imagePath(""), filterName("Normal"),
      pollOption1(""), pollOption2(""), pollVotes1(0), pollVotes2(0),
      eventDate(""), eventVenue(""), rsvpGoing(0), rsvpMaybe(0), rsvpCant(0)
{
}

Post Post::createPhotoPost(int id, const std::string& authorRoll, const std::string& authorName,
                           const std::string& content, const std::string& imgPath, 
                           const std::string& filter, const std::string& time)
{
    Post p(id, authorRoll, authorName, content, time, WHITE);
    p.kind = POST_KIND_PHOTO;
    p.imagePath = imgPath;
    p.filterName = filter;
    return p;
}

Post Post::createPollPost(int id, const std::string& authorRoll, const std::string& authorName,
                          const std::string& question, const std::string& opt1, const std::string& opt2,
                          const std::string& time)
{
    Post p(id, authorRoll, authorName, question, time, Color{ 245, 248, 255, 255 });
    p.kind = POST_KIND_POLL;
    p.pollOption1 = opt1;
    p.pollOption2 = opt2;
    return p;
}

Post Post::createEventPost(int id, const std::string& authorRoll, const std::string& authorName,
                           const std::string& title, const std::string& date, const std::string& venue,
                           const std::string& details, const std::string& time)
{
    Post p(id, authorRoll, authorName, details, time, Color{ 255, 246, 235, 255 });
    p.kind = POST_KIND_EVENT;
    p.eventDate = date;
    p.eventVenue = venue;
    return p;
}

Post Post::createNotice(int id, const std::string& authorRoll, const std::string& authorName,
                         const std::string& content, const std::string& time)
{
    Post p(id, authorRoll, authorName, content, time, Color{ 255, 238, 210, 255 });
    p.kind = POST_KIND_NOTICE;
    return p;
}

bool Post::toggleLike(const std::string& userRoll) {
    for (size_t i = 0; i < likedByRolls.size(); ++i) {
        if (likedByRolls[i] == userRoll) {
            for (size_t j = i; j + 1 < likedByRolls.size(); ++j) {
                likedByRolls[j] = likedByRolls[j + 1];
            }
            likedByRolls.pop_back();
            return false;
        }
    }
    likedByRolls.push_back(userRoll);
    return true;
}

bool Post::isLikedBy(const std::string& userRoll) const {
    for (size_t i = 0; i < likedByRolls.size(); ++i) {
        if (likedByRolls[i] == userRoll) {
            return true;
        }
    }
    return false;
}

void Post::addComment(const Comment& comment) {
    comments.push_back(comment);
}

bool Post::votePoll(int option, const std::string& userRoll) {
    if (hasVotedPoll(userRoll)) return false;
    pollVoters.push_back(userRoll);
    if (option == 1) pollVotes1++;
    else if (option == 2) pollVotes2++;
    return true;
}

bool Post::hasVotedPoll(const std::string& userRoll) const {
    for (size_t i = 0; i < pollVoters.size(); ++i) {
        if (pollVoters[i] == userRoll) return true;
    }
    return false;
}

bool Post::submitRsvp(int choice, const std::string& userRoll) {
    for (size_t i = 0; i < rsvpUsers.size(); ++i) {
        if (rsvpUsers[i] == userRoll) return false;
    }
    rsvpUsers.push_back(userRoll);
    if (choice == 1) rsvpGoing++;
    else if (choice == 2) rsvpMaybe++;
    else if (choice == 3) rsvpCant++;
    return true;
}

void Post::write(std::ostream& out) const {
    out << id << "\n"
        << (int)kind << "\n"
        << authorRoll << "\n"
        << authorName << "\n"
        << content << "\n"
        << timestamp << "\n"
        << imagePath << "\n"
        << filterName << "\n"
        << pollOption1 << "\n"
        << pollOption2 << "\n"
        << pollVotes1 << "\n"
        << pollVotes2 << "\n"
        << eventDate << "\n"
        << eventVenue << "\n"
        << rsvpGoing << "\n"
        << rsvpMaybe << "\n"
        << rsvpCant << "\n";
}

void Post::read(std::istream& in) {
    in >> id;
    int k = 1;
    in >> k;
    kind = (PostKind)k;
    in.ignore();
    std::getline(in, authorRoll);
    std::getline(in, authorName);
    std::getline(in, content);
    std::getline(in, timestamp);
    std::getline(in, imagePath);
    std::getline(in, filterName);
    std::getline(in, pollOption1);
    std::getline(in, pollOption2);
    in >> pollVotes1;
    in >> pollVotes2;
    in.ignore();
    std::getline(in, eventDate);
    std::getline(in, eventVenue);
    in >> rsvpGoing;
    in >> rsvpMaybe;
    in >> rsvpCant;
}
