#pragma once

#include "Reader.h"

#include <string>
#pragma once

#include "Reader.h"

#include <string>
#include <vector>

class Author : public Reader
{
private:
    std::vector<std::string> publishedBooks;
    std::vector<std::string> publishers;

public:
    Author();
    Author(const std::string& username, const std::string& password, const Date& registrationDate);

    UserRole getRole() const override;
    std::string getRoleName() const override;

    void addPublishedBook(const std::string& bookTitle);
    const std::vector<std::string>& getPublishedBooks() const;

    bool addPublisher(const std::string& publisherUsername);
    bool removePublisher(const std::string& publisherUsername);
    bool worksWithPublisher(const std::string& publisherUsername) const;
    const std::vector<std::string>& getPublishers() const;
};