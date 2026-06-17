#pragma once

#include <string>

class Date
{
private:
    int day;
    int month;
    int year;

public:
    Date();
    Date(int day, int month, int year);

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    bool isValid() const;
    std::string toString() const;

    static Date fromString(const std::string& text);
    static Date today();
};