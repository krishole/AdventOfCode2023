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
	std::cout << line << std::endl;
	
	const auto& winningNumbers = FindAllNumbersInString(parts[1]);
	const auto& elfsNumbers = FindAllNumbersInString(parts[2]);

	int matching_numbers = 0;

	for (int win : winningNumbers) {
		for (int num : elfsNumbers) {
			if (win == num) {
				std::cout << win << std::endl;
				++matching_numbers;
			}
		}
	}

	if (matching_numbers > 0) {
		int points = DoubleNTimes(matching_numbers - 1);
		std::cout << matching_numbers << " " << points << std::endl;
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

	std::cout << "Total" << s_Total << std::endl;

	return 0;
}
