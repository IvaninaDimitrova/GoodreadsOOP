#include "Shelf.h"

#include <algorithm>

Shelf::Shelf()
    : name(""), bookTitles(), createdOn()
{
}

Shelf::Shelf(const std::string& name, const Date& createdOn)
    : name(name), bookTitles(), createdOn(createdOn)
{
}

const std::string& Shelf::getName() const
{
    return name;
}

const std::vector<std::string>& Shelf::getBookTitles() const
{
    return bookTitles;
}

const Date& Shelf::getCreatedOn() const
{
    return createdOn;
}

int Shelf::getBookCount() const
{
    return static_cast<int>(bookTitles.size());
}

bool Shelf::hasBook(const std::string& bookTitle) const
{
    return std::find(bookTitles.begin(), bookTitles.end(), bookTitle) != bookTitles.end();
}

bool Shelf::addBook(const std::string& bookTitle)
{
    if (hasBook(bookTitle))
    {
        return false;
    }

    bookTitles.push_back(bookTitle);
    return true;
}

bool Shelf::removeBook(const std::string& bookTitle)
{
    auto iterator = std::find(bookTitles.begin(), bookTitles.end(), bookTitle);

    if (iterator == bookTitles.end())
    {
        return false;
    }

    bookTitles.erase(iterator);
    return true;
}