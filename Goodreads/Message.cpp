#include "Message.h"

Message::Message()
    : fromUsername(""),
    content(""),
    read(false),
    type(MessageType::General)
{
}

Message::Message(const std::string& fromUsername,
    const std::string& content,
    MessageType type)
    : fromUsername(fromUsername),
    content(content),
    read(false),
    type(type)
{
}

const std::string& Message::getFromUsername() const
{
    return fromUsername;
}

const std::string& Message::getContent() const
{
    return content;
}

MessageType Message::getType() const
{
    return type;
}

bool Message::isRead() const
{
    return read;
}

void Message::markAsRead()
{
    read = true;
}

std::string Message::getTypeName() const
{
    switch (type)
    {
    case MessageType::General:
        return "general";

    case MessageType::FollowNotification:
        return "follow notification";

    case MessageType::JobOffer:
        return "job offer";

    case MessageType::BookNotification:
        return "book notification";

    default:
        return "unknown";
    }
}

MessageType Message::typeFromString(const std::string& text)
{
    if (text == "general")
    {
        return MessageType::General;
    }

    if (text == "follow notification")
    {
        return MessageType::FollowNotification;
    }

    if (text == "job offer")
    {
        return MessageType::JobOffer;
    }

    if (text == "book notification")
    {
        return MessageType::BookNotification;
    }

    return MessageType::General;
}