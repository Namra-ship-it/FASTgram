#pragma once
#include "Serializable.h"
#include "Comment.h"
#include "raylib.h"
#include <string>
#include <vector>

enum PostKind {
    POST_KIND_PHOTO = 1,
    POST_KIND_TEXT = 2,
    POST_KIND_POLL = 3,
    POST_KIND_EVENT = 4,
    POST_KIND_NOTICE = 5
};

class Post : public Serializable {
private:
    int id;
    std::string authorRoll;
    std::string authorName;
    std::string content;
    std::string timestamp;
    std::vector<std::string> likedByRolls;
    std::vector<Comment> comments;
    Color cardColor;

    PostKind kind;
    std::string imagePath;
    std::string filterName;

    // Poll fields
    std::string pollOption1;
    std::string pollOption2;
    int pollVotes1;
    int pollVotes2;
    std::vector<std::string> pollVoters;

    // Event fields
    std::string eventDate;
    std::string eventVenue;
    int rsvpGoing;
    int rsvpMaybe;
    int rsvpCant;
    std::vector<std::string> rsvpUsers;

public:
    Post();
    Post(int id, const std::string& authorRoll, const std::string& authorName, 
         const std::string& content, const std::string& time = "10m ago",
         Color color = Color{ 255, 255, 255, 255 });

    // Factory methods matching PostFactory pattern (Section 6.4 C1)
    static Post createPhotoPost(int id, const std::string& authorRoll, const std::string& authorName,
                                const std::string& content, const std::string& imgPath, 
                                const std::string& filter, const std::string& time);

    static Post createPollPost(int id, const std::string& authorRoll, const std::string& authorName,
                               const std::string& question, const std::string& opt1, const std::string& opt2,
                               const std::string& time);

    static Post createEventPost(int id, const std::string& authorRoll, const std::string& authorName,
                                const std::string& title, const std::string& date, const std::string& venue,
                                const std::string& details, const std::string& time);

    static Post createNotice(int id, const std::string& authorRoll, const std::string& authorName,
                             const std::string& content, const std::string& time);

    int getId() const { return id; }
    const std::string& getAuthorRoll() const { return authorRoll; }
    const std::string& getAuthorName() const { return authorName; }
    const std::string& getContent() const { return content; }
    const std::string& getTimestamp() const { return timestamp; }
    Color getCardColor() const { return cardColor; }

    PostKind getKind() const { return kind; }
    void setKind(PostKind k) { kind = k; }

    const std::string& getImagePath() const { return imagePath; }
    void setImagePath(const std::string& path) { imagePath = path; }

    const std::string& getFilterName() const { return filterName; }
    void setFilterName(const std::string& filter) { filterName = filter; }

    // Poll accessors & actions
    const std::string& getPollOption1() const { return pollOption1; }
    const std::string& getPollOption2() const { return pollOption2; }
    int getPollVotes1() const { return pollVotes1; }
    int getPollVotes2() const { return pollVotes2; }
    bool votePoll(int option, const std::string& userRoll);
    bool hasVotedPoll(const std::string& userRoll) const;

    // Event accessors & actions
    const std::string& getEventDate() const { return eventDate; }
    const std::string& getEventVenue() const { return eventVenue; }
    int getRsvpGoing() const { return rsvpGoing; }
    int getRsvpMaybe() const { return rsvpMaybe; }
    int getRsvpCant() const { return rsvpCant; }
    bool submitRsvp(int choice, const std::string& userRoll); // 1=Going, 2=Maybe, 3=Can't

    bool toggleLike(const std::string& userRoll);
    bool isLikedBy(const std::string& userRoll) const;
    int getLikeCount() const { return (int)likedByRolls.size(); }

    void addComment(const Comment& comment);
    const std::vector<Comment>& getComments() const { return comments; }
    int getCommentCount() const { return (int)comments.size(); }

    void write(std::ostream& out) const override;
    void read(std::istream& in) override;
};
