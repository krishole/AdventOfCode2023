#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>

struct SHandsData {
	int red = 0;
	int blue = 0;
	int green = 0;
};

struct SGameData {
	int id = 0;
	std::vector<SHandsData> hands;
};

static std::vector<SGameData> s_Games;

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
		} else if (foundNumber) {
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
	SGameData game;

	size_t delim_pos = line.find(':');
	if (delim_pos != std::string::npos) {
		std::string left = line.substr(0, delim_pos);
		std::string right = line.substr(delim_pos + 1);

		game.id = FindNumberInString(left);


		std::vector<std::string> hands = ParseString(right, ";");

		for (const std::string& hand : hands) {
			std::vector<std::string> marbles = ParseString(hand, ",");
			SHandsData hand;
			for (const std::string& marble : marbles) {
				int count = FindNumberInString(marble);

				if (StringContains(marble, "red")) {
					hand.red += count;
				}
				else if (StringContains(marble, "green")) {
					hand.green += count;
				}
				else if (StringContains(marble, "blue")) {
					hand.blue += count;
				}
			}
			game.hands.push_back(hand);
		}

		s_Games.push_back(game);
	}	
}

const std::vector<SGameData> FindPossibleGames(int red, int green, int blue) {
	std::vector<SGameData> games;

	for (const SGameData& game : s_Games) {
		bool possible = true;
		for (const SHandsData& hand : game.hands) {
			if (hand.red > red || hand.green > green || hand.blue > blue) {
				possible = false;
			}
		}
		if (possible) {
			games.push_back(game);
		}
	}

	return games;
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
	} else {
		std::cout << "Unable to open file" << std::endl;
	}

	const std::vector<SGameData> games = FindPossibleGames(12, 13, 14);

	std::cout << "Possible games: " << games.size() << std::endl;

	int possible_id_sum_total = 0;
	for (const SGameData& game : games) {
		possible_id_sum_total += game.id;
	}

	std::cout << "Sum Total: " << possible_id_sum_total << std::endl;

	return 0;
}
