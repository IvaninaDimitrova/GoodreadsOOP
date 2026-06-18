#include "FuzzySearch.h"

#include <cctype>

std::string FuzzySearch::toLowercase(const std::string& text)
{
    std::string result = text;

    for (char& symbol : result)
    {
        unsigned char current = static_cast<unsigned char>(symbol);
        symbol = static_cast<char>(std::tolower(current));
    }

    return result;
}

int FuzzySearch::hammingDistance(const std::string& first, const std::string& second)
{
    if (first.length() != second.length())
    {
        return -1;
    }

    int distance = 0;

    for (size_t i = 0; i < first.length(); i++)
    {
        if (first[i] != second[i])
        {
            distance++;
        }
    }

    return distance;
}

bool FuzzySearch::areSimilar(const std::string& first, const std::string& second)
{
    std::string loweredFirst = toLowercase(first);
    std::string loweredSecond = toLowercase(second);

    int distance = hammingDistance(loweredFirst, loweredSecond);

    if (distance == -1)
    {
        return false;
    }

    return distance <= 2;
}