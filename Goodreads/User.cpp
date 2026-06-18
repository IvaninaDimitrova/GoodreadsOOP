#include "User.h"

#include <algorithm>
#include <cctype>

User::User()
    : username(""), password(""), registrationDate(), followers(), following()
{
}

User::User(const std::string& username, const std::string& password, const Date& registrationDate)
    : username(username), password(password), registrationDate(registrationDate), followers(), following()
{
}

const std::string& User::getUsername() const
{
    return username;
}

const std::string& User::getPasswordForSaving() const
{
    return password;
}

const Date& User::getRegistrationDate() const
{
    return registrationDate;
}

bool User::checkPassword(const std::string& password) const
{
    return this->password == password;
}

const std::vector<std::string>& User::getFollowers() const
{
    return followers;
}

const std::vector<std::string>& User::getFollowing() const
{
    return following;
}

void User::addFollower(const std::string& username)
{
    if (!hasFollower(username))
    {
        followers.push_back(username);
    }
}

bool User::hasFollower(const std::string& username) const
{
    return std::find(followers.begin(), followers.end(), username) != followers.end();
}

bool User::follow(const std::string& username)
{
    if (isFollowing(username))
    {
        return false;
    }

    following.push_back(username);
    return true;
}

bool User::isFollowing(const std::string& username) const
{
    return std::find(following.begin(), following.end(), username) != following.end();
}

bool User::isValidUsername(const std::string& username)
{
    return username.length() >= 6 && username.length() <= 24;
}

bool User::isValidPassword(const std::string& password)
{
    if (password.length() < 12 || password.length() > 36)
    {
        return false;
    }

    bool hasLowercase = false;
    bool hasUppercase = false;
    bool hasNonLetter = false;

    for (char symbol : password)
    {
        unsigned char current = static_cast<unsigned char>(symbol);

        if (std::islower(current))
        {
            hasLowercase = true;
        }
        else if (std::isupper(current))
        {
            hasUppercase = true;
        }
        else if (!std::isalpha(current))
        {
            hasNonLetter = true;
        }
    }

    return hasLowercase && hasUppercase && hasNonLetter;
}