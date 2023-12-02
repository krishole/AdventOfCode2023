#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>

static int s_GamePowersTotal = 0;

bool StringContains(const std::string& input, const std::string& string) {
	return input.find(string) != std::string::npos;
}

int FindNumberInString(const std::string& line) {
	int number = 0;
	bool foundNumber = false;

	for (char c : line) {
		if (std::isdigit(c)) {
			foundNumber = true;
			number = number * 10 + (c - '0');
		}
		else if (foundNumber) {
			return number;
		}
	}

	if (foundNumber) {
		return number;
	}

	assert(false);
	return -1;
}

std::vector<std::string> ParseString(const std::string& line, const std::string& delimiters) {
	std::vector<std::string> chunks;
	size_t start_pos = 0;
	size_t delim_pos = line.find_first_of(delimiters);

	while (delim_pos != std::string::npos) {
		chunks.push_back(line.substr(start_pos, delim_pos - start_pos));
		start_pos = delim_pos + 1;
		delim_pos = line.find_first_of(delimiters, start_pos);
	}

	if (start_pos < line.size()) {
		chunks.push_back(line.substr(start_pos));
	}

	return chunks;
}

void ProcessLine(const std::string& line) {
	int min_red = 0;
	int min_green = 0;
	int min_blue = 0;

	size_t delim_pos = line.find(':');
	if (delim_pos != std::string::npos) {
		std::string right = line.substr(delim_pos + 1);

		std::vector<std::string> hands = ParseString(right, ";");

		for (const std::string& hand : hands) {
			std::vector<std::string> marbles = ParseString(hand, ",");
			for (const std::string& marble : marbles) {
				int count = FindNumberInString(marble);

				if (StringContains(marble, "red") && min_red < count) {
					min_red = count;
				}
				else if (StringContains(marble, "green") && min_green < count) {
					min_green = count;
				}
				else if (StringContains(marble, "blue") && min_blue < count) {
					min_blue = count;
				}
			}
		}
	}

	int power = min_red * min_green * min_blue;
	s_GamePowersTotal += power;
}

int main() {
	std::ifstream file("games.txt");
	int total = 0;

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			ProcessLine(line);
		}
		file.close();
	}
	else {
		std::cout << "Unable to open file" << std::endl;
	}

	std::cout << "Sum Total: " << s_GamePowersTotal << std::endl;

	return 0;
}
