#include "Reader.h"

#include <algorithm>
#include <stdexcept>

Reader::Reader()
    : User(), bookEntries(), shelves(), inbox(), birthday(), hasBirthday(false)
{
}

Reader::Reader(const std::string& username, const std::string& password, const Date& registrationDate)
    : User(username, password, registrationDate),
    bookEntries(),
    shelves(),
    inbox(),
    birthday(),
    hasBirthday(false)
{
}

UserRole Reader::getRole() const
{
    return UserRole::Reader;
}

std::string Reader::getRoleName() const
{
    return "reader";
}

bool Reader::addBook(const std::string& bookTitle, ReadStatus status, double rating, bool hasRating)
{
    if (hasBook(bookTitle))
    {
        return false;
    }

    BookEntry entry;
    entry.bookTitle = bookTitle;
    entry.status = status;
    entry.rating = rating;
    entry.hasRating = hasRating;

    bookEntries.push_back(entry);
    return true;
}

bool Reader::deleteBook(const std::string& bookTitle)
{
    auto iterator = std::find_if(bookEntries.begin(), bookEntries.end(),
        [&](const BookEntry& entry)
        {
            return entry.bookTitle == bookTitle;
        });

    if (iterator == bookEntries.end())
    {
        return false;
    }

    bookEntries.erase(iterator);

    for (Shelf& shelf : shelves)
    {
        shelf.removeBook(bookTitle);
    }

    return true;
}

bool Reader::hasBook(const std::string& bookTitle) const
{
    return std::any_of(bookEntries.begin(), bookEntries.end(),
        [&](const BookEntry& entry)
        {
            return entry.bookTitle == bookTitle;
        });
}

const std::vector<BookEntry>& Reader::getBookEntries() const
{
    return bookEntries;
}

bool Reader::createShelf(const std::string& name, const Date& createdOn)
{
    if (findShelf(name) != nullptr)
    {
        return false;
    }

    shelves.emplace_back(name, createdOn);
    return true;
}

bool Reader::deleteShelf(const std::string& name)
{
    auto iterator = std::find_if(shelves.begin(), shelves.end(),
        [&](const Shelf& shelf)
        {
            return shelf.getName() == name;
        });

    if (iterator == shelves.end())
    {
        return false;
    }

    shelves.erase(iterator);
    return true;
}

Shelf* Reader::findShelf(const std::string& name)
{
    for (Shelf& shelf : shelves)
    {
        if (shelf.getName() == name)
        {
            return &shelf;
        }
    }

    return nullptr;
}

const Shelf* Reader::findShelf(const std::string& name) const
{
    for (const Shelf& shelf : shelves)
    {
        if (shelf.getName() == name)
        {
            return &shelf;
        }
    }

    return nullptr;
}

bool Reader::addToShelf(const std::string& bookTitle, const std::string& shelfName)
{
    Shelf* shelf = findShelf(shelfName);

    if (shelf == nullptr)
    {
        return false;
    }

    if (!hasBook(bookTitle))
    {
        return false;
    }

    return shelf->addBook(bookTitle);
}

bool Reader::removeFromShelf(const std::string& bookTitle, const std::string& shelfName)
{
    Shelf* shelf = findShelf(shelfName);

    if (shelf == nullptr)
    {
        return false;
    }

    return shelf->removeBook(bookTitle);
}

const std::vector<Shelf>& Reader::getShelves() const
{
    return shelves;
}

void Reader::receiveMessage(const Message& message)
{
    inbox.push_back(message);
}

const std::vector<Message>& Reader::getInbox() const
{
    return inbox;
}

std::vector<Message>& Reader::getInbox()
{
    return inbox;
}

bool Reader::readMessage(size_t index)
{
    if (index >= inbox.size())
    {
        return false;
    }

    inbox[index].markAsRead();
    return true;
}

bool Reader::deleteMessage(size_t index)
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

void Reader::setBirthday(const Date& birthday)
{
    this->birthday = birthday;
    hasBirthday = true;
}

void Reader::clearBirthday()
{
    hasBirthday = false;
}

bool Reader::getBirthday(Date& output) const
{
    if (!hasBirthday)
    {
        return false;
    }

    output = birthday;
    return true;
}

ReadStatus Reader::statusFromString(const std::string& status)
{
    if (status == "plan-to-read")
    {
        return ReadStatus::PlanToRead;
    }

    if (status == "reading")
    {
        return ReadStatus::Reading;
    }

    if (status == "paused")
    {
        return ReadStatus::Paused;
    }

    if (status == "dropped")
    {
        return ReadStatus::Dropped;
    }

    throw std::invalid_argument("Invalid read status.");
}

std::string Reader::statusToString(ReadStatus status)
{
    switch (status)
    {
    case ReadStatus::PlanToRead:
        return "plan-to-read";

    case ReadStatus::Reading:
        return "reading";

    case ReadStatus::Paused:
        return "paused";

    case ReadStatus::Dropped:
        return "dropped";

    default:
        return "unknown";
    }
}