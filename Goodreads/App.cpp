#include "App.h"
#include "Utils.h"

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

App::App()
    : database(), currentUser(nullptr)
{
}

void App::run()
{
    std::cout << "Goodreads started. Type help or exit.\n";

    std::string line;

    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, line);

        if (line == "exit")
        {
            std::cout << "Exiting Goodreads.\n";
            break;
        }

        executeCommand(line);
    }
}

void App::executeCommand(const std::string& line)
{
    std::vector<std::string> args = splitCommand(line);

    if (args.empty())
    {
        return;
    }

    std::string command = args[0];

    if (command == "help")
    {
        printHelp();
    }
    else if (command == "register")
    {
        handleRegister(args);
    }
    else if (command == "login")
    {
        handleLogin(args);
    }
    else if (command == "logout")
    {
        handleLogout();
    }
    else if (command == "whoami")
    {
        handleWhoami();
    }
    else if (command == "publish")
    {
        handlePublish(args);
    }
    else if (command == "search")
    {
        handleSearch(args);
    }
    else if (command == "book-info")
    {
        handleBookInfo(args);
    }
    else if (command == "add-book")
    {
        handleAddBook(args);
    }
    else if (command == "my-books")
    {
        handleMyBooks();
    }
    else if (command == "delete-book")
    {
        handleDeleteBook(args);
    }
    else
    {
        std::cout << "Unknown command: " << command << "\n";
    }
}

void App::printHelp() const
{
    std::cout << "Available commands:\n";
    std::cout << "help\n";
    std::cout << "register <username> <password> <role>\n";
    std::cout << "login <username> <password>\n";
    std::cout << "logout\n";
    std::cout << "whoami\n";
    std::cout << "publish <title> <authorUsername> <releaseDate> <pageCount> <genre1> [genre2] ...\n";
    std::cout << "search [text]\n";
    std::cout << "book-info <title>\n";
    std::cout << "add-book <title> <status> [rating]\n";
    std::cout << "my-books\n";
    std::cout << "delete-book <title>\n";
    std::cout << "exit\n";

    std::cout << "\nRegistration rules:\n";
    std::cout << "username: 6-24 characters\n";
    std::cout << "password: 12-36 characters, at least one lowercase letter, one uppercase letter, and one non-letter character\n";
    std::cout << "role: reader, author, or publisher\n";
}

void App::handleRegister(const std::vector<std::string>& args)
{
    if (args.size() != 4)
    {
        std::cout << "Usage: register <username> <password> <role>\n";
        return;
    }

    const std::string& username = args[1];
    const std::string& password = args[2];
    const std::string& role = args[3];

    bool success = database.registerUser(username, password, role);

    if (!success)
    {
        std::cout << "Registration failed. Check username, password, role, or duplicate username.\n";
        return;
    }

    std::cout << "User registered successfully.\n";
}

void App::handleLogin(const std::vector<std::string>& args)
{
    if (args.size() != 3)
    {
        std::cout << "Usage: login <username> <password>\n";
        return;
    }

    if (currentUser != nullptr)
    {
        std::cout << "You are already logged in as " << currentUser->getUsername() << ".\n";
        return;
    }

    const std::string& username = args[1];
    const std::string& password = args[2];

    User* user = database.findUser(username);

    if (user == nullptr)
    {
        std::cout << "Login failed. User not found.\n";
        return;
    }

    if (!user->checkPassword(password))
    {
        std::cout << "Login failed. Wrong password.\n";
        return;
    }

    currentUser = user;

    std::cout << "Logged in successfully as " << currentUser->getUsername();
    std::cout << " (" << currentUser->getRoleName() << ").\n";
}

void App::handleLogout()
{
    if (currentUser == nullptr)
    {
        std::cout << "You are not logged in.\n";
        return;
    }

    std::cout << "Logged out from " << currentUser->getUsername() << ".\n";
    currentUser = nullptr;
}

void App::handleWhoami() const
{
    if (currentUser == nullptr)
    {
        std::cout << "You are not logged in.\n";
        return;
    }

    std::cout << "Username: " << currentUser->getUsername() << "\n";
    std::cout << "Role: " << currentUser->getRoleName() << "\n";
    std::cout << "Registered: " << currentUser->getRegistrationDate().toString() << "\n";
}

bool App::isPositiveInteger(const std::string& text) const
{
    if (text.empty())
    {
        return false;
    }

    for (char symbol : text)
    {
        unsigned char current = static_cast<unsigned char>(symbol);

        if (!std::isdigit(current))
        {
            return false;
        }
    }

    return true;
}

void App::handlePublish(const std::vector<std::string>& args)
{
    if (args.size() < 6)
    {
        std::cout << "Usage: publish <title> <authorUsername> <releaseDate> <pageCount> <genre1> [genre2] ...\n";
        return;
    }

    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to publish a book.\n";
        return;
    }

    Publisher* publisher = dynamic_cast<Publisher*>(currentUser);

    if (publisher == nullptr)
    {
        std::cout << "Only publishers can publish books.\n";
        return;
    }

    const std::string& title = args[1];
    const std::string& authorUsername = args[2];
    const std::string& releaseDateText = args[3];
    const std::string& pageCountText = args[4];

    Author* author = database.findAuthor(authorUsername);

    if (author == nullptr)
    {
        std::cout << "Author not found.\n";
        return;
    }

    Date releaseDate = Date::fromString(releaseDateText);

    if (!releaseDate.isValid())
    {
        std::cout << "Invalid release date. Use format dd/mm/yyyy.\n";
        return;
    }

    if (!isPositiveInteger(pageCountText))
    {
        std::cout << "Page count must be a positive number.\n";
        return;
    }

    int pageCount = 0;

    try
    {
        pageCount = std::stoi(pageCountText);
    }
    catch (...)
    {
        std::cout << "Page count is too large.\n";
        return;
    }

    if (pageCount <= 0)
    {
        std::cout << "Page count must be greater than zero.\n";
        return;
    }

    std::vector<std::string> genres;

    for (size_t i = 5; i < args.size(); i++)
    {
        genres.push_back(args[i]);
    }

    Book book(
        title,
        authorUsername,
        publisher->getUsername(),
        releaseDate,
        pageCount,
        genres
    );

    bool success = database.addBook(book);

    if (!success)
    {
        std::cout << "A book with this title already exists.\n";
        return;
    }

    publisher->addAuthor(authorUsername);
    publisher->addPublishedBook(title);

    author->addPublisher(publisher->getUsername());
    author->addPublishedBook(title);

    author->receiveMessage(Message(
        publisher->getUsername(),
        "Your book \"" + title + "\" was published by " + publisher->getUsername() + ".",
        MessageType::BookNotification
    ));

    std::cout << "Book published successfully.\n";
}

std::string App::toLowercase(const std::string& text) const
{
    std::string result = text;

    for (char& symbol : result)
    {
        unsigned char current = static_cast<unsigned char>(symbol);
        symbol = static_cast<char>(std::tolower(current));
    }

    return result;
}

bool App::containsIgnoreCase(const std::string& text, const std::string& searchedText) const
{
    std::string loweredText = toLowercase(text);
    std::string loweredSearchedText = toLowercase(searchedText);

    return loweredText.find(loweredSearchedText) != std::string::npos;
}

bool App::bookMatchesSearch(const Book& book, const std::string& searchedText) const
{
    if (containsIgnoreCase(book.getTitle(), searchedText))
    {
        return true;
    }

    if (containsIgnoreCase(book.getAuthorUsername(), searchedText))
    {
        return true;
    }

    if (containsIgnoreCase(book.getPublisherUsername(), searchedText))
    {
        return true;
    }

    for (const std::string& genre : book.getGenres())
    {
        if (containsIgnoreCase(genre, searchedText))
        {
            return true;
        }
    }

    return false;
}

void App::handleSearch(const std::vector<std::string>& args) const
{
    const std::vector<Book>& books = database.getBooks();

    if (books.empty())
    {
        std::cout << "No books have been published yet.\n";
        return;
    }

    if (args.size() == 1)
    {
        std::cout << "All published books:\n";

        for (const Book& book : books)
        {
            book.printShort();
        }

        return;
    }

    if (args.size() != 2)
    {
        std::cout << "Usage: search [text]\n";
        return;
    }

    const std::string& searchedText = args[1];

    bool found = false;

    std::cout << "Search results:\n";

    for (const Book& book : books)
    {
        if (bookMatchesSearch(book, searchedText))
        {
            book.printShort();
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "No books found.\n";
    }
}

void App::handleBookInfo(const std::vector<std::string>& args) const
{
    if (args.size() != 2)
    {
        std::cout << "Usage: book-info <title>\n";
        return;
    }

    const std::string& title = args[1];

    const Book* book = database.findBook(title);

    if (book == nullptr)
    {
        std::cout << "Book not found.\n";
        return;
    }

    book->printDetails();
}

bool App::tryParseRating(const std::string& text, double& rating) const
{
    try
    {
        size_t processedCharacters = 0;
        rating = std::stod(text, &processedCharacters);

        if (processedCharacters != text.length())
        {
            return false;
        }

        return rating >= 0.0 && rating <= 10.0;
    }
    catch (...)
    {
        return false;
    }
}

void App::handleAddBook(const std::vector<std::string>& args)
{
    if (args.size() != 3 && args.size() != 4)
    {
        std::cout << "Usage: add-book <title> <status> [rating]\n";
        return;
    }

    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to add a book.\n";
        return;
    }

    Reader* reader = dynamic_cast<Reader*>(currentUser);

    if (reader == nullptr)
    {
        std::cout << "Only readers and authors can add books.\n";
        return;
    }

    const std::string& title = args[1];
    const std::string& statusText = args[2];

    Book* book = database.findBook(title);

    if (book == nullptr)
    {
        std::cout << "Book not found. You can only add already published books.\n";
        return;
    }

    ReadStatus status;

    try
    {
        status = Reader::statusFromString(statusText);
    }
    catch (...)
    {
        std::cout << "Invalid status. Use: plan-to-read, reading, paused, or dropped.\n";
        return;
    }

    double rating = 0.0;
    bool hasRating = false;

    if (args.size() == 4)
    {
        hasRating = true;

        if (!tryParseRating(args[3], rating))
        {
            std::cout << "Rating must be a number between 0 and 10.\n";
            return;
        }
    }

    bool success = reader->addBook(title, status, rating, hasRating);

    if (!success)
    {
        std::cout << "This book is already in your list.\n";
        return;
    }

    if (hasRating)
    {
        book->addRating(rating);
    }

    std::cout << "Book added successfully.\n";
}
void App::handleMyBooks() const
{
    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to view your books.\n";
        return;
    }

    const Reader* reader = dynamic_cast<const Reader*>(currentUser);

    if (reader == nullptr)
    {
        std::cout << "Only readers and authors have book lists.\n";
        return;
    }

    const std::vector<BookEntry>& books = reader->getBookEntries();

    if (books.empty())
    {
        std::cout << "Your book list is empty.\n";
        return;
    }

    std::cout << "My books:\n";

    for (const BookEntry& entry : books)
    {
        std::cout << "- " << entry.bookTitle;
        std::cout << " | status: " << Reader::statusToString(entry.status);

        if (entry.hasRating)
        {
            std::cout << " | rating: " << entry.rating;
        }

        std::cout << "\n";
    }
}

void App::handleDeleteBook(const std::vector<std::string>& args)
{
    if (args.size() != 2)
    {
        std::cout << "Usage: delete-book <title>\n";
        return;
    }

    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to delete a book from your list.\n";
        return;
    }

    Reader* reader = dynamic_cast<Reader*>(currentUser);

    if (reader == nullptr)
    {
        std::cout << "Only readers and authors have book lists.\n";
        return;
    }

    const std::string& title = args[1];

    bool success = reader->deleteBook(title);

    if (!success)
    {
        std::cout << "This book is not in your list.\n";
        return;
    }

    std::cout << "Book deleted successfully.\n";
}