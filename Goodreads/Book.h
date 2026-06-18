#pragma once

#include "Date.h"

#include <string>
#include <vector>

class Book
{
private:
    std::string title;
    std::string authorUsername;
    std::string publisherUsername;
    std::string synopsis;
    std::vector<std::string> genres;
    Date releaseDate;
    int pageCount;
    double totalRating;
    int ratingCount;

public:
    Book();

    Book(const std::string& title,
        const std::string& authorUsername,
        const std::string& publisherUsername,
        const Date& releaseDate,
        int pageCount,
        const std::vector<std::string>& genres);

    const std::string& getTitle() const;
    const std::string& getAuthorUsername() const;
    const std::string& getPublisherUsername() const;
    const std::string& getSynopsis() const;
    const std::vector<std::string>& getGenres() const;
    const Date& getReleaseDate() const;

    int getPageCount() const;
    int getRatingCount() const;
    double getAverageRating() const;

    void setSynopsis(const std::string& synopsis);
    bool addRating(double rating);

    void printShort() const;
    void printDetails() const;
};