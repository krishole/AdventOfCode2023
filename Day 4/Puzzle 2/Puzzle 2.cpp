#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct SScratchCard {
	int m_NumOfWinners = 0;
	int m_Copies = 1;
};

static std::vector<SScratchCard> s_ScratchCards;

std::vector<int> FindAllNumbersInString(const std::string& line) {
	std::vector<int> numbers;

	int number = 0;
	bool found_number = false;

	for (char c : line) {
		if (std::isdigit(c)) {
			found_number = true;
			number = number * 10 + (c - '0');
		}
		else if (found_number) {
			found_number = false;
			numbers.push_back(number);
			number = 0;
		}
	}

	if (found_number) {
		numbers.push_back(number);
		number = 0;
	}

	return numbers;
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
	const auto& parts = ParseString(line, ":|");

	const auto& winning_numbers = FindAllNumbersInString(parts[1]);
	const auto& your_numbers = FindAllNumbersInString(parts[2]);

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
