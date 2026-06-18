#include "Author.h"

#include <algorithm>

Author::Author()
    : Reader(), publishedBooks(), publishers()
{
}

Author::Author(const std::string& username, const std::string& password, const Date& registrationDate)
    : Reader(username, password, registrationDate), publishedBooks(), publishers()
{
}

UserRole Author::getRole() const
{
    return UserRole::Author;
}

std::string Author::getRoleName() const
{
    return "author";
}

void Author::addPublishedBook(const std::string& bookTitle)
{
    if (std::find(publishedBooks.begin(), publishedBooks.end(), bookTitle) == publishedBooks.end())
    {
        publishedBooks.push_back(bookTitle);
    }
}

const std::vector<std::string>& Author::getPublishedBooks() const
{
    return publishedBooks;
}

bool Author::addPublisher(const std::string& publisherUsername)
{
    if (worksWithPublisher(publisherUsername))
    {
        return false;
    }

    publishers.push_back(publisherUsername);
    return true;
}

bool Author::removePublisher(const std::string& publisherUsername)
{
    auto iterator = std::find(publishers.begin(), publishers.end(), publisherUsername);

    if (iterator == publishers.end())
    {
        return false;
    }

    publishers.erase(iterator);
    return true;
}

bool Author::worksWithPublisher(const std::string& publisherUsername) const
{
    return std::find(publishers.begin(), publishers.end(), publisherUsername) != publishers.end();
}

const std::vector<std::string>& Author::getPublishers() const
{
    return publishers;
}