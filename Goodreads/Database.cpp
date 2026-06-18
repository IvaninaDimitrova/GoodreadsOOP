#include "Database.h"

Database::Database()
    : users(), books()
{
}

bool Database::registerUser(const std::string& username,
    const std::string& password,
    const std::string& role)
{
    if (!User::isValidUsername(username))
    {
        return false;
    }

    if (!User::isValidPassword(password))
    {
        return false;
    }

    if (findUser(username) != nullptr)
    {
        return false;
    }

    if (role == "reader")
    {
        users.push_back(std::unique_ptr<User>(
            new Reader(username, password, Date::today())
        ));

        return true;
    }

    if (role == "author")
    {
        users.push_back(std::unique_ptr<User>(
            new Author(username, password, Date::today())
        ));

        return true;
    }

    if (role == "publisher")
    {
        users.push_back(std::unique_ptr<User>(
            new Publisher(username, password, Date::today())
        ));

        return true;
    }

    return false;
}

User* Database::findUser(const std::string& username)
{
    for (std::unique_ptr<User>& user : users)
    {
        if (user->getUsername() == username)
        {
            return user.get();
        }
    }

    return nullptr;
}

const User* Database::findUser(const std::string& username) const
{
    for (const std::unique_ptr<User>& user : users)
    {
        if (user->getUsername() == username)
        {
            return user.get();
        }
    }

    return nullptr;
}

Reader* Database::findReader(const std::string& username)
{
    User* user = findUser(username);

    if (user == nullptr)
    {
        return nullptr;
    }

    return dynamic_cast<Reader*>(user);
}

Author* Database::findAuthor(const std::string& username)
{
    User* user = findUser(username);

    if (user == nullptr)
    {
        return nullptr;
    }

    return dynamic_cast<Author*>(user);
}

Publisher* Database::findPublisher(const std::string& username)
{
    User* user = findUser(username);

    if (user == nullptr)
    {
        return nullptr;
    }

    return dynamic_cast<Publisher*>(user);
}

bool Database::addBook(const Book& book)
{
    if (findBook(book.getTitle()) != nullptr)
    {
        return false;
    }

    books.push_back(book);
    return true;
}

Book* Database::findBook(const std::string& title)
{
    for (Book& book : books)
    {
        if (book.getTitle() == title)
        {
            return &book;
        }
    }

    return nullptr;
}

const Book* Database::findBook(const std::string& title) const
{
    for (const Book& book : books)
    {
        if (book.getTitle() == title)
        {
            return &book;
        }
    }

    return nullptr;
}

const std::vector<Book>& Database::getBooks() const
{
    return books;
}

int Database::getUserCount() const
{
    return static_cast<int>(users.size());
}

int Database::getBookCount() const
{
    return static_cast<int>(books.size());
}