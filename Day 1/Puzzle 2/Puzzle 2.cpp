#include <iostream>
#include <fstream>
#include <string>
#include <vector>

static const std::vector<std::string> s_NumberWords = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

int ProcessLine(const std::string& line, bool first = true) {
	int i = first ? 0 : line.size() - 1;
	int max = first ? line.size() : 0;

	for (; first ? i < max : i >= max; first ? ++i : --i) {
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

int main() {
	std::ifstream file("calibration.txt");

	int total = 0;

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			int first = ProcessLine(line);
			int last = ProcessLine(line, false);

			total += (first * 10) + last;
		}
		file.close();
	} else {
		std::cout << "Unable to open file" << std::endl;
	}

	std::cout << total << std::endl;

	return 0;
}
