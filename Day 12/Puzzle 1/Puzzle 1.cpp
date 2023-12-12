#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "Utils.h"

static int s_TotalArrangements = 0;

void ProcessLine(const std::string& line) {
	std::vector<std::string> chunks = ParseString(line, " ");
	std::vector<int> groups = FindAllNumbersInString(chunks[1], ',');

	int unknown_count = std::count(chunks[0].begin(), chunks[0].end(), '?');

	int possible_arrangements = 0;
	int max_combinations = std::pow(2, unknown_count);
	std::vector<int> temp_groups;

	for (int i = 0; i < max_combinations; ++i) {
		std::string temp = chunks[0];
		int idx = 0;

		for (char& c : temp) {
			if (c == '?') {
				bool broken = i & (1 << idx);
				c = broken ? '#' : '.';
				++idx;
			}
		}

		temp_groups.clear();
		int count = 0;
		for (char c : temp) {
			if (c == '#') {
				++count;
			} else {
				if (count > 0) {
					temp_groups.push_back(count);
					count = 0;
				}
			}
		}

		if (count > 0) {
			temp_groups.push_back(count);
		}

		if (temp_groups == groups) {
			++possible_arrangements;
		}
	}

	s_TotalArrangements += possible_arrangements;
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
	} else {
		std::cout << "Unable to open file" << std::endl;
	}

	std::cout << "Total Arrangements: " << s_TotalArrangements << std::endl;

	return 0;
}
