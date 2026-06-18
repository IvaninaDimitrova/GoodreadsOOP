#include "Book.h"

#include <iostream>
#include <iomanip>

Book::Book()
    : title(""),
    authorUsername(""),
    publisherUsername(""),
    synopsis(""),
    genres(),
    releaseDate(),
    pageCount(0),
    totalRating(0.0),
    ratingCount(0)
{
}

Book::Book(const std::string& title,
    const std::string& authorUsername,
    const std::string& publisherUsername,
    const Date& releaseDate,
    int pageCount,
    const std::vector<std::string>& genres)
    : title(title),
    authorUsername(authorUsername),
    publisherUsername(publisherUsername),
    synopsis(""),
    genres(genres),
    releaseDate(releaseDate),
    pageCount(pageCount),
    totalRating(0.0),
    ratingCount(0)
{
}

const std::string& Book::getTitle() const
{
    return title;
}

const std::string& Book::getAuthorUsername() const
{
    return authorUsername;
}

const std::string& Book::getPublisherUsername() const
{
    return publisherUsername;
}

const std::string& Book::getSynopsis() const
{
    return synopsis;
}

const std::vector<std::string>& Book::getGenres() const
{
    return genres;
}

const Date& Book::getReleaseDate() const
{
    return releaseDate;
}

int Book::getPageCount() const
{
    return pageCount;
}

int Book::getRatingCount() const
{
    return ratingCount;
}

double Book::getAverageRating() const
{
    if (ratingCount == 0)
    {
        return 0.0;
    }

    return totalRating / ratingCount;
}

void Book::setSynopsis(const std::string& synopsis)
{
    this->synopsis = synopsis;
}

bool Book::addRating(double rating)
{
    if (rating < 0.0 || rating > 10.0)
    {
        return false;
    }

    totalRating += rating;
    ratingCount++;
    return true;
}

void Book::printShort() const
{
    std::cout << title << " by " << authorUsername;
    std::cout << " | rating: " << std::fixed << std::setprecision(2) << getAverageRating();
    std::cout << " (" << ratingCount << " ratings)\n";
}

void Book::printDetails() const
{
    std::cout << "Title: " << title << "\n";
    std::cout << "Author: " << authorUsername << "\n";
    std::cout << "Publisher: " << publisherUsername << "\n";
    std::cout << "Release date: " << releaseDate.toString() << "\n";
    std::cout << "Pages: " << pageCount << "\n";

    std::cout << "Genres: ";
    if (genres.empty())
    {
        std::cout << "(none)";
    }
    else
    {
        for (size_t i = 0; i < genres.size(); i++)
        {
            if (i > 0)
            {
                std::cout << ", ";
            }

            std::cout << genres[i];
        }
    }
    std::cout << "\n";

    std::cout << "Average rating: " << std::fixed << std::setprecision(2) << getAverageRating();
    std::cout << " (" << ratingCount << " ratings)\n";

    if (!synopsis.empty())
    {
        std::cout << "Synopsis: " << synopsis << "\n";
    }
}