#pragma once

#include "User.h"
#include "Shelf.h"
#include "Message.h"

#include <string>
#include <vector>

enum class ReadStatus
{
    PlanToRead,
    Reading,
    Paused,
    Dropped
};

struct BookEntry
{
    std::string bookTitle;
    ReadStatus status;
    double rating;
    bool hasRating;
};

class Reader : public User
{
private:
    std::vector<BookEntry> bookEntries;
    std::vector<Shelf> shelves;
    std::vector<Message> inbox;

    Date birthday;
    bool hasBirthday;

public:
    Reader();
    Reader(const std::string& username, const std::string& password, const Date& registrationDate);

    UserRole getRole() const override;
    std::string getRoleName() const override;

    bool addBook(const std::string& bookTitle, ReadStatus status, double rating, bool hasRating);
    bool deleteBook(const std::string& bookTitle);
    bool hasBook(const std::string& bookTitle) const;
    const std::vector<BookEntry>& getBookEntries() const;

    bool createShelf(const std::string& name, const Date& createdOn);
    bool deleteShelf(const std::string& name);

    Shelf* findShelf(const std::string& name);
    const Shelf* findShelf(const std::string& name) const;

    bool addToShelf(const std::string& bookTitle, const std::string& shelfName);
    bool removeFromShelf(const std::string& bookTitle, const std::string& shelfName);

    const std::vector<Shelf>& getShelves() const;

    void receiveMessage(const Message& message);
    const std::vector<Message>& getInbox() const;
    std::vector<Message>& getInbox();

    bool readMessage(size_t index);
    bool deleteMessage(size_t index);

    void setBirthday(const Date& birthday);
    void clearBirthday();
    bool getBirthday(Date& output) const;

    static ReadStatus statusFromString(const std::string& status);
    static std::string statusToString(ReadStatus status);
};