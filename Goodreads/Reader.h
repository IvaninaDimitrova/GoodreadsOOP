#pragma once

#include "User.h"

#include <string>

class Reader : public User
{
public:
    Reader();
    Reader(const std::string& username, const std::string& password, const Date& registrationDate);

    UserRole getRole() const override;
    std::string getRoleName() const override;
};
