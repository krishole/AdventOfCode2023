#include <iostream>
#include <fstream>
#include "Utils.h"

void ProcessLine(const std::string& line) {
	std::vector<std::string> split = ParseString(line, ",");
	int total = 0;
	for (const std::string& l : split) {
		int value = 0;
		for (const char c : l) {
			value += static_cast<int>(c);
			value *= 17;
			value %= 256;
		}
		total += value;
	}

	std::cout << "Hash: " << total << std::endl;
}

int main() {
	std::ifstream file("input.txt");
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

	return 0;
}
