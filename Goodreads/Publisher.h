#pragma once

#include "User.h"
#include "Message.h"

#include <string>
#include <vector>

class Publisher : public User
{
private:
    std::vector<std::string> authors;
    std::vector<std::string> publishedBooks;
    std::vector<Message> inbox;

public:
    Publisher();
    Publisher(const std::string& username, const std::string& password, const Date& registrationDate);

    UserRole getRole() const override;
    std::string getRoleName() const override;

    void addAuthor(const std::string& authorUsername);
    bool hasAuthor(const std::string& authorUsername) const;
    const std::vector<std::string>& getAuthors() const;

    void addPublishedBook(const std::string& bookTitle);
    const std::vector<std::string>& getPublishedBooks() const;

    void receiveMessage(const Message& message);
    const std::vector<Message>& getInbox() const;
    std::vector<Message>& getInbox();

    bool readMessage(size_t index);
    bool deleteMessage(size_t index);
};