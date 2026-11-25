#pragma once
#include "models/Post.h"
#include <vector>
#include <string>

class PostManager {
private:
    std::vector<Post> posts;
    int nextPostId;

public:
    PostManager();

    void seedMockPosts();

    Post* createTextPost(const std::string& authorRoll, const std::string& authorName, 
                         const std::string& content, Color cardColor = Color{ 255, 255, 255, 255 });

    Post* createPhotoPost(const std::string& authorRoll, const std::string& authorName,
                          const std::string& caption, const std::string& imgPath, 
                          const std::string& filter = "Normal");

    Post* createPollPost(const std::string& authorRoll, const std::string& authorName,
                         const std::string& question, const std::string& opt1, const std::string& opt2);

    Post* createEventPost(const std::string& authorRoll, const std::string& authorName,
                          const std::string& title, const std::string& date, const std::string& venue,
                          const std::string& details);

    // Backward compatibility helper
    Post* createPost(const std::string& authorRoll, const std::string& authorName, 
                     const std::string& content, Color cardColor = Color{ 255, 255, 255, 255 })
    {
        return createTextPost(authorRoll, authorName, content, cardColor);
    }

    std::vector<Post>& getAllPosts() { return posts; }
    const std::vector<Post>& getAllPosts() const { return posts; }
    Post* getPostById(int id);

    bool toggleLike(int postId, const std::string& userRoll);
    bool addComment(int postId, const Comment& comment);
    bool votePoll(int postId, int option, const std::string& userRoll);
    bool submitRsvp(int postId, int choice, const std::string& userRoll);

    void clear() { posts.clear(); }
    void addPost(const Post& post) { posts.push_back(post); }
};
