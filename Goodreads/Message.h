#pragma once

#include <string>

enum class MessageType
{
    General,
    FollowNotification,
    JobOffer,
    BookNotification
};

class Message
{
private:
    std::string fromUsername;
    std::string content;
    bool read;
    MessageType type;

public:
    Message();
    Message(const std::string& fromUsername,
        const std::string& content,
        MessageType type);

    const std::string& getFromUsername() const;
    const std::string& getContent() const;
    MessageType getType() const;

    bool isRead() const;
    void markAsRead();

    std::string getTypeName() const;

    static MessageType typeFromString(const std::string& text);
};
