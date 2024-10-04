#include <string>
#include <sstream>
#include <vector>

static const std::vector<std::string> s_NumberWords = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

int FindDigitInString(const std::string& line, bool search_forward = true) {
	int i = search_forward ? 0 : line.size() - 1;
	int max = search_forward ? line.size() : 0;

	for (; search_forward ? i < max : i >= max; search_forward ? ++i : --i) {
		const char c = line.at(i);
		if (std::isdigit(c)) {
			return c - '0';
		}

		for (unsigned int nIdx = 0; nIdx < s_NumberWords.size(); ++nIdx) {
			std::string substr = line.substr(i, s_NumberWords[nIdx].size());
			if (substr == s_NumberWords[nIdx]) {
				return nIdx;
			}
		}
	}

	return 0;
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

//relies on whitespace
std::vector<int> ParseNumbersFromString(const std::string& line) {
	std::vector<std::string> strings = ParseString(line, " ");
	std::vector<int> numbers;
	for (const std::string& string : strings) {
		numbers.push_back(std::stoi(string));
	}
	return numbers;
}

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

	return -1;
}

//find all numbers regardless of delimiter
std::vector<long long> FindAllNumbersInString(const std::string& line) {
	std::istringstream iss(line);
	std::vector<long long> numbers;

	int num;
	while (iss >> num) {
		numbers.push_back(num);
	}

	return numbers;
}


bool HasAdjacentSymbol(int line, int char_pos, int length, const std::vector<std::string>& schematic) {
	const int start_line = std::max(line - 1, 0);
	const int end_line = std::min(line + 1, (int)(schematic.size()) - 1);

	for (int y = start_line; y <= end_line; ++y) {
		int start_pos = std::max(char_pos - 1, 0);
		int end_pos = std::min(char_pos + length, (int)(schematic[y].length() - 1));

		for (int x = start_pos; x <= end_pos; ++x) {
			unsigned char c = schematic[y].at(x);
			if (!std::isdigit(c) && c != '.') {
				return true;
			}
		}
	}

	return false;
}
