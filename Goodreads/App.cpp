#include "App.h"
#include "Utils.h"
#include "Reader.h"
#include "Date.h"

#include <iostream>
#include <string>
#include <vector>

void App::run() {
    std::cout << "Goodreads started. Type help or exit.\n";

    std::string line;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);

        if (line == "exit") {
            std::cout << "Exiting Goodreads.\n";
            break;
        }

        executeCommand(line);
    }
}

void App::executeCommand(const std::string& line) {
    std::vector<std::string> args = splitCommand(line);

    if (args.empty()) {
        return;
    }

    std::string command = args[0];

    if (command == "help") {
        printHelp();
    }
    else if (command == "reader-test") {
        Reader reader("ivanina123", "ValidPassword1", Date::today());

        std::cout << "Username: " << reader.getUsername() << "\n";
        std::cout << "Role: " << reader.getRoleName() << "\n";
        std::cout << "Registered: " << reader.getRegistrationDate().toString() << "\n";

        if (reader.checkPassword("ValidPassword1")) {
            std::cout << "Password check works.\n";
        }
        else {
            std::cout << "Password check failed.\n";
        }
    }
    else {
        std::cout << "Unknown command: " << command << "\n";
    }
}

void App::printHelp() const {
    std::cout << "Available commands:\n";
    std::cout << "help\n";
    std::cout << "reader-test\n";
    std::cout << "register <username> <password> <role>\n";
    std::cout << "login <username> <password>\n";
    std::cout << "logout\n";
    std::cout << "exit\n";

    std::cout << "\nRegistration rules:\n";
    std::cout << "username: 6-24 characters\n";
    std::cout << "password: 12-36 characters, at least one lowercase letter, one uppercase letter, and one non-letter character\n";
    std::cout << "role: reader, author, or publisher\n";
}