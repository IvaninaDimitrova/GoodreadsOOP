#include "Database.h"

#include <fstream>
#include <sstream>

Database::Database()
    : users(), books()
{
}

bool Database::registerUser(const std::string& username,
    const std::string& password,
    const std::string& role)
{
    if (!User::isValidUsername(username))
    {
        return false;
    }

    if (!User::isValidPassword(password))
    {
        return false;
    }

    if (findUser(username) != nullptr)
    {
        return false;
    }

    if (role == "reader")
    {
        users.push_back(std::unique_ptr<User>(
            new Reader(username, password, Date::today())
        ));

        return true;
    }

    if (role == "author")
    {
        users.push_back(std::unique_ptr<User>(
            new Author(username, password, Date::today())
        ));

        return true;
    }

    if (role == "publisher")
    {
        users.push_back(std::unique_ptr<User>(
            new Publisher(username, password, Date::today())
        ));

        return true;
    }

    return false;
}

User* Database::findUser(const std::string& username)
{
    for (std::unique_ptr<User>& user : users)
    {
        if (user->getUsername() == username)
        {
            return user.get();
        }
    }

    return nullptr;
}

const User* Database::findUser(const std::string& username) const
{
    for (const std::unique_ptr<User>& user : users)
    {
        if (user->getUsername() == username)
        {
            return user.get();
        }
    }

    return nullptr;
}

Reader* Database::findReader(const std::string& username)
{
    User* user = findUser(username);

    if (user == nullptr)
    {
        return nullptr;
    }

    return dynamic_cast<Reader*>(user);
}

Author* Database::findAuthor(const std::string& username)
{
    User* user = findUser(username);

    if (user == nullptr)
    {
        return nullptr;
    }

    return dynamic_cast<Author*>(user);
}

Publisher* Database::findPublisher(const std::string& username)
{
    User* user = findUser(username);

    if (user == nullptr)
    {
        return nullptr;
    }

    return dynamic_cast<Publisher*>(user);
}

bool Database::addBook(const Book& book)
{
    if (findBook(book.getTitle()) != nullptr)
    {
        return false;
    }

    books.push_back(book);
    return true;
}

Book* Database::findBook(const std::string& title)
{
    for (Book& book : books)
    {
        if (book.getTitle() == title)
        {
            return &book;
        }
    }

    return nullptr;
}

const Book* Database::findBook(const std::string& title) const
{
    for (const Book& book : books)
    {
        if (book.getTitle() == title)
        {
            return &book;
        }
    }

    return nullptr;
}

const std::vector<Book>& Database::getBooks() const
{
    return books;
}

int Database::getUserCount() const
{
    return static_cast<int>(users.size());
}

int Database::getBookCount() const
{
    return static_cast<int>(books.size());
}

bool Database::saveToFiles() const
{
    std::ofstream usersFile("users.txt");

    if (!usersFile.is_open())
    {
        return false;
    }

    for (const std::unique_ptr<User>& user : users)
    {
        usersFile << user->getRoleName() << "|"
            << user->getUsername() << "|"
            << user->getPasswordForSaving() << "|"
            << user->getRegistrationDate().toString()
            << "\n";
    }

    usersFile.close();

    std::ofstream booksFile("books.txt");

    if (!booksFile.is_open())
    {
        return false;
    }

    for (const Book& book : books)
    {
        booksFile << book.getTitle() << "|"
            << book.getAuthorUsername() << "|"
            << book.getPublisherUsername() << "|"
            << book.getReleaseDate().toString() << "|"
            << book.getPageCount() << "|"
            << book.getTotalRating() << "|"
            << book.getRatingCount() << "|";

        const std::vector<std::string>& genres = book.getGenres();

        for (size_t i = 0; i < genres.size(); i++)
        {
            if (i > 0)
            {
                booksFile << ",";
            }

            booksFile << genres[i];
        }

        booksFile << "|"
            << book.getSynopsis()
            << "\n";
    }

    booksFile.close();

    std::ofstream readerBooksFile("reader_books.txt");

    if (!readerBooksFile.is_open())
    {
        return false;
    }

    for (const std::unique_ptr<User>& user : users)
    {
        const Reader* reader = dynamic_cast<const Reader*>(user.get());

        if (reader == nullptr)
        {
            continue;
        }

        const std::vector<BookEntry>& bookEntries = reader->getBookEntries();

        for (const BookEntry& entry : bookEntries)
        {
            readerBooksFile << reader->getUsername() << "|"
                << entry.bookTitle << "|"
                << Reader::statusToString(entry.status) << "|"
                << entry.rating << "|"
                << (entry.hasRating ? "1" : "0")
                << "\n";
        }
    }

    readerBooksFile.close();

    std::ofstream shelvesFile("shelves.txt");

    if (!shelvesFile.is_open())
    {
        return false;
    }

    for (const std::unique_ptr<User>& user : users)
    {
        const Reader* reader = dynamic_cast<const Reader*>(user.get());

        if (reader == nullptr)
        {
            continue;
        }

        const std::vector<Shelf>& shelves = reader->getShelves();

        for (const Shelf& shelf : shelves)
        {
            shelvesFile << reader->getUsername() << "|"
                << shelf.getName() << "|"
                << shelf.getCreatedOn().toString()
                << "\n";
        }
    }

    shelvesFile.close();

    std::ofstream shelfBooksFile("shelf_books.txt");

    if (!shelfBooksFile.is_open())
    {
        return false;
    }

    for (const std::unique_ptr<User>& user : users)
    {
        const Reader* reader = dynamic_cast<const Reader*>(user.get());

        if (reader == nullptr)
        {
            continue;
        }

        const std::vector<Shelf>& shelves = reader->getShelves();

        for (const Shelf& shelf : shelves)
        {
            const std::vector<std::string>& bookTitles = shelf.getBookTitles();

            for (const std::string& bookTitle : bookTitles)
            {
                shelfBooksFile << reader->getUsername() << "|"
                    << shelf.getName() << "|"
                    << bookTitle
                    << "\n";
            }
        }
    }

    shelfBooksFile.close();

    std::ofstream followsFile("follows.txt");

    if (!followsFile.is_open())
    {
        return false;
    }

    for (const std::unique_ptr<User>& user : users)
    {
        const std::vector<std::string>& following = user->getFollowing();

        for (const std::string& followedUsername : following)
        {
            followsFile << user->getUsername() << "|"
                << followedUsername
                << "\n";
        }
    }

    followsFile.close();

    std::ofstream messagesFile("messages.txt");

    if (!messagesFile.is_open())
    {
        return false;
    }

    for (const std::unique_ptr<User>& user : users)
    {
        const Reader* reader = dynamic_cast<const Reader*>(user.get());

        if (reader != nullptr)
        {
            const std::vector<Message>& inbox = reader->getInbox();

            for (const Message& message : inbox)
            {
                messagesFile << reader->getUsername() << "|"
                    << message.getFromUsername() << "|"
                    << message.getTypeName() << "|"
                    << (message.isRead() ? "1" : "0") << "|"
                    << message.getContent()
                    << "\n";
            }

            continue;
        }

        const Publisher* publisher = dynamic_cast<const Publisher*>(user.get());

        if (publisher != nullptr)
        {
            const std::vector<Message>& inbox = publisher->getInbox();

            for (const Message& message : inbox)
            {
                messagesFile << publisher->getUsername() << "|"
                    << message.getFromUsername() << "|"
                    << message.getTypeName() << "|"
                    << (message.isRead() ? "1" : "0") << "|"
                    << message.getContent()
                    << "\n";
            }
        }
    }

    messagesFile.close();

    std::ofstream birthdaysFile("birthdays.txt");

    if (!birthdaysFile.is_open())
    {
        return false;
    }

    for (const std::unique_ptr<User>& user : users)
    {
        const Reader* reader = dynamic_cast<const Reader*>(user.get());

        if (reader == nullptr)
        {
            continue;
        }

        Date birthday;

        if (reader->getBirthday(birthday))
        {
            birthdaysFile << reader->getUsername() << "|"
                << birthday.toString()
                << "\n";
        }
    }

    birthdaysFile.close();

    return true;
}

bool Database::loadFromFiles()
{
    users.clear();
    books.clear();

    std::ifstream usersFile("users.txt");

    if (usersFile.is_open())
    {
        std::string line;

        while (std::getline(usersFile, line))
        {
            std::stringstream stream(line);

            std::string role;
            std::string username;
            std::string password;
            std::string registrationDateText;

            std::getline(stream, role, '|');
            std::getline(stream, username, '|');
            std::getline(stream, password, '|');
            std::getline(stream, registrationDateText, '|');

            Date registrationDate = Date::fromString(registrationDateText);

            if (!registrationDate.isValid())
            {
                registrationDate = Date::today();
            }

            if (role == "reader")
            {
                users.push_back(std::unique_ptr<User>(
                    new Reader(username, password, registrationDate)
                ));
            }
            else if (role == "author")
            {
                users.push_back(std::unique_ptr<User>(
                    new Author(username, password, registrationDate)
                ));
            }
            else if (role == "publisher")
            {
                users.push_back(std::unique_ptr<User>(
                    new Publisher(username, password, registrationDate)
                ));
            }
        }

        usersFile.close();
    }

    std::ifstream booksFile("books.txt");

    if (booksFile.is_open())
    {
        std::string line;

        while (std::getline(booksFile, line))
        {
            std::stringstream stream(line);

            std::string title;
            std::string authorUsername;
            std::string publisherUsername;
            std::string releaseDateText;
            std::string pageCountText;
            std::string totalRatingText;
            std::string ratingCountText;
            std::string genresText;
            std::string synopsis;

            std::getline(stream, title, '|');
            std::getline(stream, authorUsername, '|');
            std::getline(stream, publisherUsername, '|');
            std::getline(stream, releaseDateText, '|');
            std::getline(stream, pageCountText, '|');
            std::getline(stream, totalRatingText, '|');
            std::getline(stream, ratingCountText, '|');
            std::getline(stream, genresText, '|');
            std::getline(stream, synopsis, '|');

            Date releaseDate = Date::fromString(releaseDateText);

            if (!releaseDate.isValid())
            {
                continue;
            }

            int pageCount = 0;
            double totalRating = 0.0;
            int ratingCount = 0;

            try
            {
                pageCount = std::stoi(pageCountText);
                totalRating = std::stod(totalRatingText);
                ratingCount = std::stoi(ratingCountText);
            }
            catch (...)
            {
                continue;
            }

            std::vector<std::string> genres;
            std::stringstream genreStream(genresText);
            std::string genre;

            while (std::getline(genreStream, genre, ','))
            {
                if (!genre.empty())
                {
                    genres.push_back(genre);
                }
            }

            Book book(
                title,
                authorUsername,
                publisherUsername,
                releaseDate,
                pageCount,
                genres
            );

            book.setSynopsis(synopsis);
            book.setRatingData(totalRating, ratingCount);

            books.push_back(book);
        }

        booksFile.close();
    }

    std::ifstream readerBooksFile("reader_books.txt");

    if (readerBooksFile.is_open())
    {
        std::string line;

        while (std::getline(readerBooksFile, line))
        {
            std::stringstream stream(line);

            std::string username;
            std::string bookTitle;
            std::string statusText;
            std::string ratingText;
            std::string hasRatingText;

            std::getline(stream, username, '|');
            std::getline(stream, bookTitle, '|');
            std::getline(stream, statusText, '|');
            std::getline(stream, ratingText, '|');
            std::getline(stream, hasRatingText, '|');

            Reader* reader = findReader(username);

            if (reader == nullptr)
            {
                continue;
            }

            if (findBook(bookTitle) == nullptr)
            {
                continue;
            }

            ReadStatus status;

            try
            {
                status = Reader::statusFromString(statusText);
            }
            catch (...)
            {
                continue;
            }

            double rating = 0.0;
            bool hasRating = hasRatingText == "1";

            if (hasRating)
            {
                try
                {
                    rating = std::stod(ratingText);
                }
                catch (...)
                {
                    continue;
                }
            }

            reader->addBook(bookTitle, status, rating, hasRating);
        }

        readerBooksFile.close();
    }

    std::ifstream shelvesFile("shelves.txt");

    if (shelvesFile.is_open())
    {
        std::string line;

        while (std::getline(shelvesFile, line))
        {
            std::stringstream stream(line);

            std::string username;
            std::string shelfName;
            std::string createdOnText;

            std::getline(stream, username, '|');
            std::getline(stream, shelfName, '|');
            std::getline(stream, createdOnText, '|');

            Reader* reader = findReader(username);

            if (reader == nullptr)
            {
                continue;
            }

            Date createdOn = Date::fromString(createdOnText);

            if (!createdOn.isValid())
            {
                createdOn = Date::today();
            }

            reader->createShelf(shelfName, createdOn);
        }

        shelvesFile.close();
    }

    std::ifstream shelfBooksFile("shelf_books.txt");

    if (shelfBooksFile.is_open())
    {
        std::string line;

        while (std::getline(shelfBooksFile, line))
        {
            std::stringstream stream(line);

            std::string username;
            std::string shelfName;
            std::string bookTitle;

            std::getline(stream, username, '|');
            std::getline(stream, shelfName, '|');
            std::getline(stream, bookTitle, '|');

            Reader* reader = findReader(username);

            if (reader == nullptr)
            {
                continue;
            }

            if (findBook(bookTitle) == nullptr)
            {
                continue;
            }

            reader->addToShelf(bookTitle, shelfName);
        }

        shelfBooksFile.close();
    }

    std::ifstream followsFile("follows.txt");

    if (followsFile.is_open())
    {
        std::string line;

        while (std::getline(followsFile, line))
        {
            std::stringstream stream(line);

            std::string followerUsername;
            std::string followedUsername;

            std::getline(stream, followerUsername, '|');
            std::getline(stream, followedUsername, '|');

            User* follower = findUser(followerUsername);
            User* followed = findUser(followedUsername);

            if (follower == nullptr || followed == nullptr)
            {
                continue;
            }

            if (follower->getUsername() == followed->getUsername())
            {
                continue;
            }

            bool success = follower->follow(followedUsername);

            if (success)
            {
                followed->addFollower(followerUsername);
            }
        }

        followsFile.close();
    }

    std::ifstream messagesFile("messages.txt");

    if (messagesFile.is_open())
    {
        std::string line;

        while (std::getline(messagesFile, line))
        {
            std::stringstream stream(line);

            std::string receiverUsername;
            std::string fromUsername;
            std::string typeText;
            std::string readText;
            std::string content;

            std::getline(stream, receiverUsername, '|');
            std::getline(stream, fromUsername, '|');
            std::getline(stream, typeText, '|');
            std::getline(stream, readText, '|');
            std::getline(stream, content);

            User* receiver = findUser(receiverUsername);

            if (receiver == nullptr)
            {
                continue;
            }

            Message message(
                fromUsername,
                content,
                Message::typeFromString(typeText)
            );

            if (readText == "1")
            {
                message.markAsRead();
            }

            Reader* reader = dynamic_cast<Reader*>(receiver);

            if (reader != nullptr)
            {
                reader->receiveMessage(message);
                continue;
            }

            Publisher* publisher = dynamic_cast<Publisher*>(receiver);

            if (publisher != nullptr)
            {
                publisher->receiveMessage(message);
            }
        }

        messagesFile.close();
    }

    std::ifstream birthdaysFile("birthdays.txt");

    if (birthdaysFile.is_open())
    {
        std::string line;

        while (std::getline(birthdaysFile, line))
        {
            std::stringstream stream(line);

            std::string username;
            std::string birthdayText;

            std::getline(stream, username, '|');
            std::getline(stream, birthdayText, '|');

            Reader* reader = findReader(username);

            if (reader == nullptr)
            {
                continue;
            }

            Date birthday = Date::fromString(birthdayText);

            if (!birthday.isValid())
            {
                continue;
            }

            reader->setBirthday(birthday);
        }

        birthdaysFile.close();
    }

    return true;
}