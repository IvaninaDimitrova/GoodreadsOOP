#include "Publisher.h"

#include <algorithm>

Publisher::Publisher()
    : User(), authors(), publishedBooks(), inbox()
{
}

Publisher::Publisher(const std::string& username, const std::string& password, const Date& registrationDate)
    : User(username, password, registrationDate), authors(), publishedBooks(), inbox()
{
}

UserRole Publisher::getRole() const
{
    return UserRole::Publisher;
}

std::string Publisher::getRoleName() const
{
    return "publisher";
}

void Publisher::addAuthor(const std::string& authorUsername)
{
    if (!hasAuthor(authorUsername))
    {
        authors.push_back(authorUsername);
    }
}

bool Publisher::hasAuthor(const std::string& authorUsername) const
{
    return std::find(authors.begin(), authors.end(), authorUsername) != authors.end();
}

const std::vector<std::string>& Publisher::getAuthors() const
{
    return authors;
}

void Publisher::addPublishedBook(const std::string& bookTitle)
{
    if (std::find(publishedBooks.begin(), publishedBooks.end(), bookTitle) == publishedBooks.end())
    {
        publishedBooks.push_back(bookTitle);
    }
}

const std::vector<std::string>& Publisher::getPublishedBooks() const
{
    return publishedBooks;
}

void Publisher::receiveMessage(const Message& message)
{
    inbox.push_back(message);
}

const std::vector<Message>& Publisher::getInbox() const
{
    return inbox;
}

std::vector<Message>& Publisher::getInbox()
{
    return inbox;
}

bool Publisher::readMessage(size_t index)
{
    if (index >= inbox.size())
    {
        return false;
    }

    inbox[index].markAsRead();
    return true;
}

bool Publisher::deleteMessage(size_t index)
{
    if (index >= inbox.size())
    {
        return false;
    }

    if (!inbox[index].isRead())
    {
        return false;
    }

    inbox.erase(inbox.begin() + index);
    return true;
}