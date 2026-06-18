#include "Date.h"

#include <sstream>
#include <iomanip>
#include <ctime>

Date::Date()
    : day(1), month(1), year(2000)
{
}

Date::Date(int day, int month, int year)
    : day(day), month(month), year(year)
{
}

int Date::getDay() const
{
    return day;
}

int Date::getMonth() const
{
    return month;
}

int Date::getYear() const
{
    return year;
}

bool Date::isValid() const
{
    if (year < 1)
    {
        return false;
    }

    if (month < 1 || month > 12)
    {
        return false;
    }

    if (day < 1)
    {
        return false;
    }

    int daysInMonth[] = {
        0,
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

    if (month == 2 && isLeapYear)
    {
        return day <= 29;
    }

    return day <= daysInMonth[month];
}

std::string Date::toString() const
{
    std::ostringstream output;

    output << std::setw(2) << std::setfill('0') << day << "/"
        << std::setw(2) << std::setfill('0') << month << "/"
        << year;

    return output.str();
}

Date Date::fromString(const std::string& text)
{
    std::istringstream input(text);

    int day;
    int month;
    int year;
    char firstSeparator;
    char secondSeparator;

    input >> day >> firstSeparator >> month >> secondSeparator >> year;

    if (!input || firstSeparator != '/' || secondSeparator != '/' || !input.eof())
    {
        return Date(0, 0, 0);
    }

    return Date(day, month, year);
}

Date Date::today()
{
    std::time_t currentTime = std::time(nullptr);
    std::tm localTime;

#ifdef _MSC_VER
    localtime_s(&localTime, &currentTime);
#else
    localTime = *std::localtime(&currentTime);
#endif

    return Date(localTime.tm_mday, localTime.tm_mon + 1, localTime.tm_year + 1900);
}