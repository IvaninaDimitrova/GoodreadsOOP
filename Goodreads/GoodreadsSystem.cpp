#include "GoodreadsSystem.h"
#include <iostream>
#include <string>

void GoodreadsSystem::run() {
	std::cout << "Goodreads started. Type help or exit.\n";

	std::string command;

	while (true) {
		std::cout << "> ";
		std::getline(std::cin, command);

		if (command == "exit") {
			std::cout << "Exiting Goodreads.\n";
			break;
		}
		else if (command == "help") {
			std::cout << "Available commands:\n";
			std::cout << "help\n";
			std::cout << "exit\n";
		}
		else {
			std::cout << "Unknown command.\n";
		}
	}
}
