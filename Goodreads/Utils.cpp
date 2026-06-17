#include "Utils.h"

std::vector<std::string> splitCommand(const std::string& line)
{
	std::vector<std::string> result;
	std::string current;
	bool insideQuotes = false;

	for (size_t i = 0; i < line.length(); i++) {
		char ch = line[i];

		if (ch == '"') {
			insideQuotes = !insideQuotes;
		}
		else if (ch == ' ' && !insideQuotes) {
			if (!current.empty()) {
				result.push_back(current);
				current.clear();
			}
		}
		else {
			current += ch;
		}
	}

	if (!current.empty()) {
		result.push_back(current);
	}

	return result;
}
