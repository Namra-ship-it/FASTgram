#include "PostManager.h"

PostManager::PostManager()
    : nextPostId(1)
{
    seedMockPosts();
}

void PostManager::seedMockPosts() {
    posts.clear();

    // 1. Pinned Announcement / Notice Post (Section 6.13 X2)
    posts.push_back(Post::createNotice(nextPostId++, "acm_fast", "ACM Student Chapter", 
        "📢 OFFICIAL NOTICE: NASCON '26 Registration is now LIVE! Project stalls, Auto Show, and Gaming Competitions start tomorrow morning at 9:00 AM! 🚀", 
        "Pinned Notice"));

    // 2. Real Photo Post: NASCON Auto Show in Campus Courtyard (Section 6.2 P1)
    posts.push_back(Post::createPhotoPost(nextPostId++, "i261234", "Sara Ahmed", 
        "NASCON '26 Auto Show in the campus courtyard! Look at these heavy bikes and sports cars today! 🏍️🔥 Best campus vibes ever! #NASCON26 #FASTCampus", 
        "images/posts/nascon_autoshow.jpg", "Normal", "15m ago"));

    // 3. Real Photo Post: FAST NUCES Campus Main Building (Section 6.2 P1)
    posts.push_back(Post::createPhotoPost(nextPostId++, "i269900", "Hamza Tariq", 
        "Bright sunny day at FAST NUCES! Our campus looks magnificent today getting ready for the big event! ☀️🏢 #FASTians #CampusLife", 
        "images/posts/campus_main.jpg", "Normal", "1h ago"));

    // 4. Campus Poll Post (Section 6.2 P4)
    posts.push_back(Post::createPollPost(nextPostId++, "i265544", "Ali Khan",
        "FAST Cafeteria Debate: Which one is the undefeated king of lunch?",
        "1. Biryani 🍛", "2. Shawarma Roll 🌯", "2h ago"));

    // 5. Campus Event Post with RSVP (Section 6.13 X3)
    posts.push_back(Post::createEventPost(nextPostId++, "acm_fast", "ACM Society",
        "FAST Speed Programming Battle '26",
        "14 MAR • 3:00 PM", "CS Lab 3 & 4",
        "2 hours, 6 algorithm problems, cash prizes and internship interviews! Register your teams now.",
        "Yesterday"));

    // Add initial likes and comments
    if (posts.size() >= 2) {
        posts[1].toggleLike("i265544");
        posts[1].toggleLike("i267788");
        posts[1].toggleLike("i269900");
        posts[1].addComment(Comment("i265544", "Ali Khan", "That red sports bike was insane! 🔥"));
        posts[1].addComment(Comment("i267788", "Zainab Malik", "Visiting the stalls right after OOP lecture!"));
    }

    if (posts.size() >= 4) {
        posts[3].votePoll(1, "i261234");
        posts[3].votePoll(1, "i267788");
        posts[3].votePoll(2, "i269900");
    }

    if (posts.size() >= 5) {
        posts[4].submitRsvp(1, "i261234");
        posts[4].submitRsvp(1, "i265544");
    }
}

Post* PostManager::createTextPost(const std::string& authorRoll, const std::string& authorName, 
                                 const std::string& content, Color cardColor)
{
    posts.push_back(Post(nextPostId++, authorRoll, authorName, content, "Just now", cardColor));
    for (int i = (int)posts.size() - 1; i > 0; --i) {
        Post temp = posts[i];
        posts[i] = posts[i - 1];
        posts[i - 1] = temp;
    }
    return &posts[0];
}

Post* PostManager::createPhotoPost(const std::string& authorRoll, const std::string& authorName,
                                  const std::string& caption, const std::string& imgPath, 
                                  const std::string& filter)
{
    posts.push_back(Post::createPhotoPost(nextPostId++, authorRoll, authorName, caption, imgPath, filter, "Just now"));
    for (int i = (int)posts.size() - 1; i > 0; --i) {
        Post temp = posts[i];
        posts[i] = posts[i - 1];
        posts[i - 1] = temp;
    }
    return &posts[0];
}

Post* PostManager::createPollPost(const std::string& authorRoll, const std::string& authorName,
                                 const std::string& question, const std::string& opt1, const std::string& opt2)
{
    posts.push_back(Post::createPollPost(nextPostId++, authorRoll, authorName, question, opt1, opt2, "Just now"));
    for (int i = (int)posts.size() - 1; i > 0; --i) {
        Post temp = posts[i];
        posts[i] = posts[i - 1];
        posts[i - 1] = temp;
    }
    return &posts[0];
}

Post* PostManager::createEventPost(const std::string& authorRoll, const std::string& authorName,
                                  const std::string& title, const std::string& date, const std::string& venue,
                                  const std::string& details)
{
    posts.push_back(Post::createEventPost(nextPostId++, authorRoll, authorName, title, date, venue, details, "Just now"));
    for (int i = (int)posts.size() - 1; i > 0; --i) {
        Post temp = posts[i];
        posts[i] = posts[i - 1];
        posts[i - 1] = temp;
    }
    return &posts[0];
}

Post* PostManager::getPostById(int id) {
    for (size_t i = 0; i < posts.size(); ++i) {
        if (posts[i].getId() == id) {
            return &posts[i];
        }
    }
    return nullptr;
}

bool PostManager::toggleLike(int postId, const std::string& userRoll) {
    Post* post = getPostById(postId);
    if (post) {
        return post->toggleLike(userRoll);
    }
    return false;
}

bool PostManager::addComment(int postId, const Comment& comment) {
    Post* post = getPostById(postId);
    if (post) {
        post->addComment(comment);
        return true;
    }
    return false;
}

bool PostManager::votePoll(int postId, int option, const std::string& userRoll) {
    Post* post = getPostById(postId);
    if (post) {
        return post->votePoll(option, userRoll);
    }
    return false;
}

bool PostManager::submitRsvp(int postId, int choice, const std::string& userRoll) {
    Post* post = getPostById(postId);
    if (post) {
        return post->submitRsvp(choice, userRoll);
    }
    return false;
}
