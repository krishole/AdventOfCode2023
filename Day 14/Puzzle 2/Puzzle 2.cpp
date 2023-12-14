#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

std::vector<std::string> s_Grid;
static const int s_Cycles[4][2] = { { 0, -1 }, { -1, 0 }, { 0, 1 }, { 1, 0 } };
static long long s_Cycle = 0;

std::unordered_map<size_t, long long> s_Memo;

bool IsValid(int x, int y) {
	return (y >= 0 && y < s_Grid.size() && x >= 0 && x < s_Grid[0].size());
}

void Tilt(const int dir[2]) {
	int start_y = dir[1] == 1 ? static_cast<int>(s_Grid.size()) - 1 : 0;
	int start_x = dir[0] == 1 ? static_cast<int>(s_Grid[0].size()) - 1 : 0;

	int last_x = start_x == 0 ? static_cast<int>(s_Grid.size()) : -1;
	int last_y = start_y == 0 ? static_cast<int>(s_Grid[0].size()) : -1;

	for (int y = start_y; y != last_y; dir[1] == 1 ?--y : ++y) {
		for (int x = start_x; x != last_x; dir[0] == 1 ? --x : ++x) {
			if (s_Grid[y][x] == 'O') {

				int next_x = x;
				int next_y = y;
				bool valid = false;

				while (IsValid(next_x += dir[0], next_y += dir[1]) && s_Grid[next_y][next_x] == '.') {
					valid = true;
				}

				if (valid) {
					s_Grid[y][x] = '.';
					s_Grid[next_y - dir[1]][next_x - dir[0]] = 'O';
				}
			}
		}
	}
}

void Cycle() {
	for (int j = 0; j < 4; ++j) {
		Tilt(s_Cycles[j]);
	}
}

void CountLoad() {
	int total = 0;
	for (int y = 0; y < s_Grid.size(); ++y) {
		for (int x = 0; x < s_Grid.size(); ++x) {
			if (s_Grid[y][x] == 'O') {
				total += static_cast<int>(s_Grid.size()) - y;
			}
		}
	}

	std::cout << "Total: " << total << std::endl;
}

bool CheckGrid() {
	size_t hash = std::hash<std::string>{}(s_Grid[0]);
	for (int i = 1; i < s_Grid.size(); ++i) {
		hash ^= std::hash<std::string>{}(s_Grid[i]);
	}

	if (s_Memo.contains(hash))
	{
		long long pattern_start = s_Memo.at(hash);
		long long pattern_length = s_Cycle - pattern_start;

		long long cycles_left = 1000000000 - s_Cycle;
		long long cycles_to_target = cycles_left % pattern_length;

		for (long long i = 0; i < cycles_to_target; ++i) {
			Cycle();
		}

		return true;
	}
	else
	{
		s_Memo[hash] = s_Cycle;
	}
	return false;
}

int main() {
	std::ifstream file("input.txt");
	int total = 0;

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			s_Grid.push_back(line);
		}
		file.close();
	}
	else {
		std::cout << "Unable to open file" << std::endl;
	}

	for (s_Cycle = 1; s_Cycle < 1000000001; ++s_Cycle) {
		Cycle();
		if (CheckGrid()) {
			break;
		}
	}
	CountLoad();

	return 0;
}
