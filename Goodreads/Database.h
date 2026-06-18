#pragma once

#include "User.h"
#include "Reader.h"
#include "Author.h"
#include "Publisher.h"
#include "Book.h"

#include <memory>
#include <string>
#include <vector>

class Database
{
private:
    std::vector<std::unique_ptr<User>> users;
    std::vector<Book> books;

public:
    Database();

    bool registerUser(const std::string& username,
        const std::string& password,
        const std::string& role);

    User* findUser(const std::string& username);
    const User* findUser(const std::string& username) const;

    Reader* findReader(const std::string& username);
    Author* findAuthor(const std::string& username);
    Publisher* findPublisher(const std::string& username);

    bool addBook(const Book& book);

    Book* findBook(const std::string& title);
    const Book* findBook(const std::string& title) const;

    const std::vector<Book>& getBooks() const;

    int getUserCount() const;
    int getBookCount() const;

    bool saveToFiles() const;
    bool loadFromFiles();
};