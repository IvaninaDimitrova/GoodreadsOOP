#include "App.h"
#include "Utils.h"
#include "Message.h"
#include "FuzzySearch.h"

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

App::App()
    : database(), currentUser(nullptr)
{
    database.loadFromFiles();
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
            database.saveToFiles();
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
    else if (command == "create-shelf")
    {
        handleCreateShelf(args);
    }
    else if (command == "delete-shelf")
    {
        handleDeleteShelf(args);
    }
    else if (command == "show-shelves")
    {
        handleShowShelves();
    }
    else if (command == "add-to-shelf")
    {
        handleAddToShelf(args);
    }
    else if (command == "remove-from-shelf")
    {
        handleRemoveFromShelf(args);
    }
    else if (command == "shelf-info")
    {
        handleShelfInfo(args);
    }
    else if (command == "follow")
    {
        handleFollow(args);
    }
    else if (command == "profile")
    {
        handleProfile(args);
    }
    else if (command == "inbox")
    {
        handleInbox();
    }
    else if (command == "read-message")
    {
        handleReadMessage(args);
    }
    else if (command == "delete-message")
    {
        handleDeleteMessage(args);
    }
    else if (command == "set-birthday")
    {
        handleSetBirthday(args);
    }
    else if (command == "clear-birthday")
    {
        handleClearBirthday();
    }
    else
    {
        std::cout << "Unknown command: " << command << "\n";
    }
}

void App::printHelp() const
{
    std::cout << "Available commands:\n";

    std::cout << "\nAccount:\n";
    std::cout << "help\n";
    std::cout << "register <username> <password> <role>\n";
    std::cout << "login <username> <password>\n";
    std::cout << "logout\n";
    std::cout << "whoami\n";
    std::cout << "exit\n";

    std::cout << "\nBooks:\n";
    std::cout << "publish <title> <authorUsername> <releaseDate> <pageCount> <genre1> [genre2] ...\n";
    std::cout << "search [text]\n";
    std::cout << "book-info <title>\n";
    std::cout << "add-book <title> <status> [rating]\n";
    std::cout << "my-books\n";
    std::cout << "delete-book <title>\n";

    std::cout << "\nShelves:\n";
    std::cout << "create-shelf <name>\n";
    std::cout << "delete-shelf <name>\n";
    std::cout << "show-shelves\n";
    std::cout << "add-to-shelf <bookTitle> <shelfName>\n";
    std::cout << "remove-from-shelf <bookTitle> <shelfName>\n";
    std::cout << "shelf-info <shelfName>\n";

    std::cout << "\nSocial:\n";
    std::cout << "follow <username>\n";
    std::cout << "profile <username>\n";

    std::cout << "\nMessages:\n";
    std::cout << "inbox\n";
    std::cout << "read-message <index>\n";
    std::cout << "delete-message <index>\n";

    std::cout << "\nProfile settings:\n";
    std::cout << "set-birthday <dd/mm/yyyy>\n";
    std::cout << "clear-birthday\n";

    std::cout << "\nRegistration rules:\n";
    std::cout << "username: 6-24 characters\n";
    std::cout << "password: 12-36 characters, at least one lowercase letter, one uppercase letter, and one non-letter character\n";
    std::cout << "role: reader, author, or publisher\n";

    std::cout << "\nBook status values:\n";
    std::cout << "plan-to-read, reading, paused, dropped\n";

    std::cout << "\nNotes:\n";
    std::cout << "Use quotes for titles or shelf names with spaces, for example: \"Dune Messiah\".\n";
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

bool App::wordsMatchFuzzy(const std::string& text, const std::string& searchedText) const
{
    if (FuzzySearch::areSimilar(text, searchedText))
    {
        return true;
    }

    std::string currentWord;

    for (char symbol : text)
    {
        if (symbol == ' ' || symbol == '-' || symbol == '_' || symbol == ',' || symbol == '.')
        {
            if (!currentWord.empty())
            {
                if (FuzzySearch::areSimilar(currentWord, searchedText))
                {
                    return true;
                }

                currentWord.clear();
            }
        }
        else
        {
            currentWord += symbol;
        }
    }

    if (!currentWord.empty())
    {
        if (FuzzySearch::areSimilar(currentWord, searchedText))
        {
            return true;
        }
    }

    return false;
}

bool App::bookMatchesSearch(const Book& book, const std::string& searchedText) const
{
    if (containsIgnoreCase(book.getTitle(), searchedText) ||
        wordsMatchFuzzy(book.getTitle(), searchedText))
    {
        return true;
    }

    if (containsIgnoreCase(book.getAuthorUsername(), searchedText) ||
        wordsMatchFuzzy(book.getAuthorUsername(), searchedText))
    {
        return true;
    }

    if (containsIgnoreCase(book.getPublisherUsername(), searchedText) ||
        wordsMatchFuzzy(book.getPublisherUsername(), searchedText))
    {
        return true;
    }

    for (const std::string& genre : book.getGenres())
    {
        if (containsIgnoreCase(genre, searchedText) ||
            wordsMatchFuzzy(genre, searchedText))
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

void App::handleCreateShelf(const std::vector<std::string>& args)
{
    if (args.size() != 2)
    {
        std::cout << "Usage: create-shelf <name>\n";
        return;
    }

    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to create a shelf.\n";
        return;
    }

    Reader* reader = dynamic_cast<Reader*>(currentUser);

    if (reader == nullptr)
    {
        std::cout << "Only readers and authors can create shelves.\n";
        return;
    }

    const std::string& shelfName = args[1];

    bool success = reader->createShelf(shelfName, Date::today());

    if (!success)
    {
        std::cout << "Shelf with this name already exists.\n";
        return;
    }

    std::cout << "Shelf created successfully.\n";
}

void App::handleDeleteShelf(const std::vector<std::string>& args)
{
    if (args.size() != 2)
    {
        std::cout << "Usage: delete-shelf <name>\n";
        return;
    }

    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to delete a shelf.\n";
        return;
    }

    Reader* reader = dynamic_cast<Reader*>(currentUser);

    if (reader == nullptr)
    {
        std::cout << "Only readers and authors can delete shelves.\n";
        return;
    }

    const std::string& shelfName = args[1];

    bool success = reader->deleteShelf(shelfName);

    if (!success)
    {
        std::cout << "Shelf not found.\n";
        return;
    }

    std::cout << "Shelf deleted successfully.\n";
}

void App::handleShowShelves() const
{
    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to view your shelves.\n";
        return;
    }

    const Reader* reader = dynamic_cast<const Reader*>(currentUser);

    if (reader == nullptr)
    {
        std::cout << "Only readers and authors have shelves.\n";
        return;
    }

    const std::vector<Shelf>& shelves = reader->getShelves();

    if (shelves.empty())
    {
        std::cout << "You do not have any shelves yet.\n";
        return;
    }

    std::cout << "Your shelves:\n";

    for (const Shelf& shelf : shelves)
    {
        std::cout << "- " << shelf.getName()
            << " | created on: " << shelf.getCreatedOn().toString()
            << " | books: " << shelf.getBookCount() << "\n";

        const std::vector<std::string>& bookTitles = shelf.getBookTitles();

        for (const std::string& bookTitle : bookTitles)
        {
            std::cout << "  * " << bookTitle << "\n";
        }
    }
}

void App::handleAddToShelf(const std::vector<std::string>& args)
{
    if (args.size() != 3)
    {
        std::cout << "Usage: add-to-shelf <bookTitle> <shelfName>\n";
        return;
    }

    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to add a book to a shelf.\n";
        return;
    }

    Reader* reader = dynamic_cast<Reader*>(currentUser);

    if (reader == nullptr)
    {
        std::cout << "Only readers and authors have shelves.\n";
        return;
    }

    const std::string& bookTitle = args[1];
    const std::string& shelfName = args[2];

    if (database.findBook(bookTitle) == nullptr)
    {
        std::cout << "Book not found.\n";
        return;
    }

    if (!reader->hasBook(bookTitle))
    {
        std::cout << "This book is not in your book list. Use add-book first.\n";
        return;
    }

    bool success = reader->addToShelf(bookTitle, shelfName);

    if (!success)
    {
        std::cout << "Could not add book to shelf. Check if the shelf exists or if the book is already there.\n";
        return;
    }

    std::cout << "Book added to shelf successfully.\n";
}

void App::handleRemoveFromShelf(const std::vector<std::string>& args)
{
    if (args.size() != 3)
    {
        std::cout << "Usage: remove-from-shelf <bookTitle> <shelfName>\n";
        return;
    }

    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to remove a book from a shelf.\n";
        return;
    }

    Reader* reader = dynamic_cast<Reader*>(currentUser);

    if (reader == nullptr)
    {
        std::cout << "Only readers and authors have shelves.\n";
        return;
    }

    const std::string& bookTitle = args[1];
    const std::string& shelfName = args[2];

    bool success = reader->removeFromShelf(bookTitle, shelfName);

    if (!success)
    {
        std::cout << "Could not remove book from shelf. Check if the shelf and book exist.\n";
        return;
    }

    std::cout << "Book removed from shelf successfully.\n";
}

void App::handleShelfInfo(const std::vector<std::string>& args) const
{
    if (args.size() != 2)
    {
        std::cout << "Usage: shelf-info <shelfName>\n";
        return;
    }

    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to view shelf info.\n";
        return;
    }

    const Reader* reader = dynamic_cast<const Reader*>(currentUser);

    if (reader == nullptr)
    {
        std::cout << "Only readers and authors have shelves.\n";
        return;
    }

    const std::string& shelfName = args[1];

    const Shelf* shelf = reader->findShelf(shelfName);

    if (shelf == nullptr)
    {
        std::cout << "Shelf not found.\n";
        return;
    }

    std::cout << "Shelf: " << shelf->getName() << "\n";
    std::cout << "Created on: " << shelf->getCreatedOn().toString() << "\n";
    std::cout << "Books count: " << shelf->getBookCount() << "\n";

    const std::vector<std::string>& bookTitles = shelf->getBookTitles();

    if (bookTitles.empty())
    {
        std::cout << "This shelf is empty.\n";
        return;
    }

    std::cout << "Books:\n";

    for (const std::string& bookTitle : bookTitles)
    {
        std::cout << "- " << bookTitle << "\n";
    }
}

void App::handleFollow(const std::vector<std::string>& args)
{
    if (args.size() != 2)
    {
        std::cout << "Usage: follow <username>\n";
        return;
    }

    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to follow someone.\n";
        return;
    }

    const std::string& targetUsername = args[1];

    if (currentUser->getUsername() == targetUsername)
    {
        std::cout << "You cannot follow yourself.\n";
        return;
    }

    User* targetUser = database.findUser(targetUsername);

    if (targetUser == nullptr)
    {
        std::cout << "User not found.\n";
        return;
    }

    bool success = currentUser->follow(targetUsername);

    if (!success)
    {
        std::cout << "You already follow this user.\n";
        return;
    }

    targetUser->addFollower(currentUser->getUsername());

    Reader* targetReader = dynamic_cast<Reader*>(targetUser);

    if (targetReader != nullptr)
    {
        targetReader->receiveMessage(Message(
            currentUser->getUsername(),
            currentUser->getUsername() + " started following you.",
            MessageType::FollowNotification
        ));
    }
    else
    {
        Publisher* targetPublisher = dynamic_cast<Publisher*>(targetUser);

        if (targetPublisher != nullptr)
        {
            targetPublisher->receiveMessage(Message(
                currentUser->getUsername(),
                currentUser->getUsername() + " started following you.",
                MessageType::FollowNotification
            ));
        }
    }

    std::cout << "You are now following " << targetUsername << ".\n";
}

void App::handleProfile(const std::vector<std::string>& args) const
{
    if (args.size() != 2)
    {
        std::cout << "Usage: profile <username>\n";
        return;
    }

    const std::string& username = args[1];

    const User* user = database.findUser(username);

    if (user == nullptr)
    {
        std::cout << "User not found.\n";
        return;
    }

    std::cout << "Username: " << user->getUsername() << "\n";
    std::cout << "Role: " << user->getRoleName() << "\n";
    std::cout << "Registered: " << user->getRegistrationDate().toString() << "\n";
    std::cout << "Followers: " << user->getFollowers().size() << "\n";
    std::cout << "Following: " << user->getFollowing().size() << "\n";

    const Reader* reader = dynamic_cast<const Reader*>(user);

    if (reader != nullptr)
    {
        std::cout << "\nReader info:\n";
        std::cout << "Books in list: " << reader->getBookEntries().size() << "\n";
        std::cout << "Shelves: " << reader->getShelves().size() << "\n";

        Date birthday;

        if (reader->getBirthday(birthday))
        {
            std::cout << "Birthday: " << birthday.toString() << "\n";
        }
    }

    const Author* author = dynamic_cast<const Author*>(user);

    if (author != nullptr)
    {
        std::cout << "\nAuthor info:\n";

        const std::vector<std::string>& publishedBooks = author->getPublishedBooks();

        std::cout << "Published books: " << publishedBooks.size() << "\n";

        for (const std::string& bookTitle : publishedBooks)
        {
            std::cout << "- " << bookTitle << "\n";
        }

        const std::vector<std::string>& publishers = author->getPublishers();

        std::cout << "Publishers: " << publishers.size() << "\n";

        for (const std::string& publisherUsername : publishers)
        {
            std::cout << "- " << publisherUsername << "\n";
        }
    }

    const Publisher* publisher = dynamic_cast<const Publisher*>(user);

    if (publisher != nullptr)
    {
        std::cout << "\nPublisher info:\n";

        const std::vector<std::string>& authors = publisher->getAuthors();

        std::cout << "Authors: " << authors.size() << "\n";

        for (const std::string& authorUsername : authors)
        {
            std::cout << "- " << authorUsername << "\n";
        }

        const std::vector<std::string>& publishedBooks = publisher->getPublishedBooks();

        std::cout << "Published books: " << publishedBooks.size() << "\n";

        for (const std::string& bookTitle : publishedBooks)
        {
            std::cout << "- " << bookTitle << "\n";
        }
    }
}

bool App::tryParseIndex(const std::string& text, size_t& index) const
{
    try
    {
        size_t processedCharacters = 0;
        unsigned long value = std::stoul(text, &processedCharacters);

        if (processedCharacters != text.length())
        {
            return false;
        }

        index = static_cast<size_t>(value);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

void App::handleInbox() const
{
    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to view your inbox.\n";
        return;
    }

    const Reader* reader = dynamic_cast<const Reader*>(currentUser);

    if (reader != nullptr)
    {
        const std::vector<Message>& inbox = reader->getInbox();

        if (inbox.empty())
        {
            std::cout << "Your inbox is empty.\n";
            return;
        }

        std::cout << "Inbox:\n";

        for (size_t i = 0; i < inbox.size(); i++)
        {
            std::cout << "[" << i << "] "
                << (inbox[i].isRead() ? "[READ] " : "[NEW] ")
                << inbox[i].getTypeName()
                << " from " << inbox[i].getFromUsername()
                << ": " << inbox[i].getContent() << "\n";
        }

        return;
    }

    const Publisher* publisher = dynamic_cast<const Publisher*>(currentUser);

    if (publisher != nullptr)
    {
        const std::vector<Message>& inbox = publisher->getInbox();

        if (inbox.empty())
        {
            std::cout << "Your inbox is empty.\n";
            return;
        }

        std::cout << "Inbox:\n";

        for (size_t i = 0; i < inbox.size(); i++)
        {
            std::cout << "[" << i << "] "
                << (inbox[i].isRead() ? "[READ] " : "[NEW] ")
                << inbox[i].getTypeName()
                << " from " << inbox[i].getFromUsername()
                << ": " << inbox[i].getContent() << "\n";
        }

        return;
    }

    std::cout << "This user type does not have an inbox.\n";
}

void App::handleReadMessage(const std::vector<std::string>& args)
{
    if (args.size() != 2)
    {
        std::cout << "Usage: read-message <index>\n";
        return;
    }

    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to read messages.\n";
        return;
    }

    size_t index = 0;

    if (!tryParseIndex(args[1], index))
    {
        std::cout << "Message index must be a non-negative number.\n";
        return;
    }

    Reader* reader = dynamic_cast<Reader*>(currentUser);

    if (reader != nullptr)
    {
        bool success = reader->readMessage(index);

        if (!success)
        {
            std::cout << "Message not found.\n";
            return;
        }

        std::cout << "Message marked as read.\n";
        return;
    }

    Publisher* publisher = dynamic_cast<Publisher*>(currentUser);

    if (publisher != nullptr)
    {
        bool success = publisher->readMessage(index);

        if (!success)
        {
            std::cout << "Message not found.\n";
            return;
        }

        std::cout << "Message marked as read.\n";
        return;
    }

    std::cout << "This user type does not have an inbox.\n";
}

void App::handleDeleteMessage(const std::vector<std::string>& args)
{
    if (args.size() != 2)
    {
        std::cout << "Usage: delete-message <index>\n";
        return;
    }

    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to delete messages.\n";
        return;
    }

    size_t index = 0;

    if (!tryParseIndex(args[1], index))
    {
        std::cout << "Message index must be a non-negative number.\n";
        return;
    }

    Reader* reader = dynamic_cast<Reader*>(currentUser);

    if (reader != nullptr)
    {
        bool success = reader->deleteMessage(index);

        if (!success)
        {
            std::cout << "Could not delete message. Check if it exists and has already been read.\n";
            return;
        }

        std::cout << "Message deleted successfully.\n";
        return;
    }

    Publisher* publisher = dynamic_cast<Publisher*>(currentUser);

    if (publisher != nullptr)
    {
        bool success = publisher->deleteMessage(index);

        if (!success)
        {
            std::cout << "Could not delete message. Check if it exists and has already been read.\n";
            return;
        }

        std::cout << "Message deleted successfully.\n";
        return;
    }

    std::cout << "This user type does not have an inbox.\n";
}

void App::handleSetBirthday(const std::vector<std::string>& args)
{
    if (args.size() != 2)
    {
        std::cout << "Usage: set-birthday <dd/mm/yyyy>\n";
        return;
    }

    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to set your birthday.\n";
        return;
    }

    Reader* reader = dynamic_cast<Reader*>(currentUser);

    if (reader == nullptr)
    {
        std::cout << "Only readers and authors can set a birthday.\n";
        return;
    }

    Date birthday = Date::fromString(args[1]);

    if (!birthday.isValid())
    {
        std::cout << "Invalid birthday. Use format dd/mm/yyyy.\n";
        return;
    }

    reader->setBirthday(birthday);

    std::cout << "Birthday set successfully.\n";
}

void App::handleClearBirthday()
{
    if (currentUser == nullptr)
    {
        std::cout << "You must be logged in to clear your birthday.\n";
        return;
    }

    Reader* reader = dynamic_cast<Reader*>(currentUser);

    if (reader == nullptr)
    {
        std::cout << "Only readers and authors can clear a birthday.\n";
        return;
    }

    reader->clearBirthday();

    std::cout << "Birthday cleared successfully.\n";
}