#pragma once

#include "Date.h"

#include <string>
#include <vector>

class Shelf
{
private:
    std::string name;
    std::vector<std::string> bookTitles;
    Date createdOn;

public:
    Shelf();
    Shelf(const std::string& name, const Date& createdOn);

    const std::string& getName() const;
    const std::vector<std::string>& getBookTitles() const;
    const Date& getCreatedOn() const;

    int getBookCount() const;

    bool hasBook(const std::string& bookTitle) const;
    bool addBook(const std::string& bookTitle);
    bool removeBook(const std::string& bookTitle);
};