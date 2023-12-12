#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "Utils.h"

#include <unordered_map>
#include <tuple>

typedef unsigned long long ull;
static ull s_Total = 0;
static std::unordered_map<std::tuple<std::string, int, std::vector<int>, int, int>, ull> s_Memo;

ull GetPossibileCombinations(const std::string& line, const int& i, const std::vector<int>& groups, const int group_i, const int hash_count) {
	auto key = std::make_tuple(line, i, groups, group_i, hash_count);
	if (s_Memo.find(key) != s_Memo.end()) {
		return s_Memo[key];
	}

	ull result = 0;
	if (i == line.length()) {
		result = group_i == groups.size() ? 1 : 0;
	} else if (line[i] == '#') {
		result = GetPossibileCombinations(line, i + 1, groups, group_i, hash_count + 1);
	} else if (line[i] == '.' || group_i == groups.size()) {
		if (group_i < groups.size() && hash_count == groups[group_i]) {
			result = GetPossibileCombinations(line, i + 1, groups, group_i + 1, 0);
		} else if(hash_count == 0) {
			result = GetPossibileCombinations(line, i + 1, groups, group_i, 0);
		}
	} else {
		ull total_hash = GetPossibileCombinations(line, i + 1, groups, group_i, hash_count + 1);
		ull dot_count = 0;
		if (hash_count == groups[group_i]) {
			dot_count = GetPossibileCombinations(line, i + 1, groups, group_i + 1, 0);
		} else if (hash_count == 0) {
			dot_count = GetPossibileCombinations(line, i + 1, groups, group_i, 0);
		}
		result = total_hash + dot_count;
	}

	s_Memo[key] = result;

	return result;
}

void ProcessLine(const std::string& line) {
	std::vector<std::string> chunks = ParseString(line, " ");
	std::vector<int> folded_groups = FindAllNumbersInString(chunks[1], ',');

	std::string unfolded;
	std::vector<int> unfolded_groups;
	for (int i = 0; i < 5; ++i) {
		unfolded += chunks[0];
		if (i != 4) {
			unfolded += '?';
		}
		unfolded_groups.insert(unfolded_groups.end(), folded_groups.begin(), folded_groups.end());
	}

	unfolded += '.';

	ull total = GetPossibileCombinations(unfolded, 0, unfolded_groups, 0, 0);
	s_Total += total;
	std::cout << s_Total << " " << total << std::endl;
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

	std::cout << s_Total << std::endl;

	return 0;
}
