#pragma once
#include <string>

class App
{
private:
	void executeCommand(const std::string& line);
	void printHelp() const;

public:
	void run();
};

