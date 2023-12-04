#include <iostream>
#include <fstream>
#include <string>
#include <vector>

static int s_Total = 0;

int DoubleNTimes(int N) {
	int result = 1;

	for (int i = 0; i < N; ++i) {
		result *= 2;
	}

	return result;
}

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

	const std::vector<int> winningNumbers = ParseNumbersFromString(parts[1]);
	const std::vector<int> elfsNumbers = ParseNumbersFromString(parts[2]);

	int matching_numbers = 0;

	for (int win : winningNumbers) {
		for (int num : elfsNumbers) {
			if (win == num) {
				++matching_numbers;
			}
		}
	}

	if (matching_numbers > 0) {
		int points = DoubleNTimes(matching_numbers - 1);
		s_Total += points;
	}
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
	} else {
		std::cout << "Unable to open file" << std::endl;
	}

	std::cout << "Total " << s_Total << std::endl;

	return 0;
}
