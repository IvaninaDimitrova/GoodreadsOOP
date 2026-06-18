#include "Reader.h"

Reader::Reader()
    : User()
{
}

Reader::Reader(const std::string& username, const std::string& password, const Date& registrationDate)
    : User(username, password, registrationDate)
{
}

UserRole Reader::getRole() const
{
    return UserRole::Reader;
}

std::string Reader::getRoleName() const
{
    return "reader";
}