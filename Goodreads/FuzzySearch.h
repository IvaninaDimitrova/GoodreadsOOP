#pragma once

#include <string>

class FuzzySearch
{
public:
    static bool areSimilar(const std::string& first, const std::string& second);

private:
    static std::string toLowercase(const std::string& text);
    static int hammingDistance(const std::string& first, const std::string& second);
};