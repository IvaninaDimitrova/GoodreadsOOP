#include "User.h"

#include <algorithm>
#include <cctype>

User::User()
    : username(""), password(""), registrationDate()
{
}

User::User(const std::string& username, const std::string& password, const Date& registrationDate)
    : username(username), password(password), registrationDate(registrationDate)
{
}

const std::string& User::getUsername() const
{
    return username;
}

const Date& User::getRegistrationDate() const
{
    return registrationDate;
}

const std::vector<std::string>& User::getFollowers() const
{
    return followers;
}

bool User::checkPassword(const std::string& password) const
{
    return this->password == password;
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