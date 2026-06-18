#pragma once

#include "Date.h"

#include <string>
#include <vector>

enum class UserRole
{
    Reader,
    Author,
    Publisher
};

class User
{
private:
    std::string username;
    std::string password;
    Date registrationDate;

    std::vector<std::string> followers;
    std::vector<std::string> following;

public:
    User();
    User(const std::string& username, const std::string& password, const Date& registrationDate);

    const std::string& getUsername() const;
    const Date& getRegistrationDate() const;

    bool checkPassword(const std::string& password) const;

    const std::vector<std::string>& getFollowers() const;
    const std::vector<std::string>& getFollowing() const;

    void addFollower(const std::string& username);
    bool hasFollower(const std::string& username) const;

    bool follow(const std::string& username);
    bool isFollowing(const std::string& username) const;

    static bool isValidUsername(const std::string& username);
    static bool isValidPassword(const std::string& password);

    virtual UserRole getRole() const = 0;
    virtual std::string getRoleName() const = 0;

    virtual ~User() = default;
};