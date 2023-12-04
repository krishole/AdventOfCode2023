#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct SScratchCard {
	int m_NumOfWinners = 0;
	int m_Copies = 1;
};

static std::vector<SScratchCard> s_ScratchCards;

std::vector<std::string> ParseString(const std::string& line, const std::string& delimiters) {
	std::vector<std::string> chunks;
	size_t start_pos = 0;
	size_t delim_pos = line.find_first_of(delimiters);

	while (delim_pos != std::string::npos) {
		std::string chunk = line.substr(start_pos, delim_pos - start_pos);
		if (chunk.length() > 0) {
			chunks.push_back(line.substr(start_pos, delim_pos - start_pos));
		}
		start_pos = delim_pos + 1;
		delim_pos = line.find_first_of(delimiters, start_pos);
	}

	if (start_pos < line.size()) {
		std::string chunk = line.substr(start_pos);
		if (chunk.length() > 0) {
			chunks.push_back(line.substr(start_pos));
		}
	}

	return chunks;
}

std::vector<int> ParseNumbersFromString(const std::string& line) {
	std::vector<std::string> strings = ParseString(line, " ");
	std::vector<int> numbers;
	for (const std::string& string : strings) {
		numbers.push_back(std::stoi(string));
	}
	return numbers;
}

void ProcessLine(const std::string& line) {
	const std::vector<std::string> parts = ParseString(line, ":|");

	const std::vector<int> winning_numbers = ParseNumbersFromString(parts[1]);
	const std::vector<int> your_numbers = ParseNumbersFromString(parts[2]);

	int winners = 0;

	for (int win : winning_numbers) {
		for (int num : your_numbers) {
			if (win == num) {
				++winners;
			}
		}
	}

	s_ScratchCards.push_back({ winners });
}

void CheckCards() {
	int total_original = s_ScratchCards.size();
	int total = 0;

	for (int i = 0; i < static_cast<int>(s_ScratchCards.size()); i++) {
		for (int copy = 0; copy < s_ScratchCards[i].m_Copies; ++copy) {
			for (int win = 1; win <= s_ScratchCards[i].m_NumOfWinners; ++win) {
				++s_ScratchCards[i + win].m_Copies;
			}
		}
	}

	for (const SScratchCard& card : s_ScratchCards) {
		total += card.m_Copies;
	}

	std::cout << "Total: " << total << std::endl;
}

int main() {
	std::ifstream file("scratchcards.txt");
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

	CheckCards();

	return 0;
}
