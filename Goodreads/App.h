#pragma once

#include "Database.h"

#include <string>
#include <vector>

class App
{
private:
    Database database;
    User* currentUser;

    void executeCommand(const std::string& line);
    void printHelp() const;

    void handleRegister(const std::vector<std::string>& args);
    void handleLogin(const std::vector<std::string>& args);
    void handleLogout();
    void handleWhoami() const;
    void handlePublish(const std::vector<std::string>& args);
    void handleSearch(const std::vector<std::string>& args) const;
    void handleBookInfo(const std::vector<std::string>& args) const;
    void handleAddBook(const std::vector<std::string>& args);
    void handleMyBooks() const;
    void handleDeleteBook(const std::vector<std::string>& args);

    bool isPositiveInteger(const std::string& text) const;
    bool tryParseRating(const std::string& text, double& rating) const;

    std::string toLowercase(const std::string& text) const;
    bool containsIgnoreCase(const std::string& text, const std::string& searchedText) const;
    bool bookMatchesSearch(const Book& book, const std::string& searchedText) const;

public:
    App();
    void run();
};