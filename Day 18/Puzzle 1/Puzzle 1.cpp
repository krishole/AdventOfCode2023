#include <iostream>
#include <fstream>
#include <queue>
#include "Utils.h"

std::vector<std::pair<int, int>> s_Directions = { { 0,-1}, {0, 1},{-1, 0}, {1,0} };

struct SDig {
	int m_Direction;
	int m_Steps;
};

std::vector<SDig> s_Data;
std::vector<std::string> s_Grid;

void ProcessLine(const std::string& line) {
	std::vector<std::string> chunks = ParseString(line, " ");

	SDig dig;
	char d = chunks[0][0];
	if (d == 'U') {
		dig.m_Direction = 0;
	} else if (d == 'D') {
		dig.m_Direction = 1;
	} else if (d == 'L') {
		dig.m_Direction = 2;
	} else if (d == 'R') {
		dig.m_Direction = 3;
	}

	dig.m_Steps = std::stoi(chunks[1]);
	s_Data.push_back(dig);
}

void FloodFill(int x, int y) {
	std::queue<std::pair<int, int>> queue;
	bool enclosed = true;

	queue.push({x, y});

	std::vector<std::pair<int, int>> checked;

	while (!queue.empty()) {
		std::pair<int, int> start_pos = queue.front();
		queue.pop();
		checked.push_back(start_pos);

		for (const auto& s : s_Directions) {
			std::pair<int, int> pos = { start_pos.first + s.first, start_pos.second + s.second };

			bool within_bounds = (pos.second >= 0 && pos.second < static_cast<int>(s_Grid.size()) && pos.first >= 0 && pos.first < static_cast<int>(s_Grid[pos.second].size()));
			if (!within_bounds) {
				enclosed = false;
			}
			else {
				char c = s_Grid[pos.second][pos.first];
				if (c != '#' && c != '.') {
					queue.push(pos);
					s_Grid[pos.second][pos.first] = '#';
				}
			}
		}
	}

	if (!enclosed) {
		for (const auto& p : checked) {
			s_Grid[p.second][p.first] = '.';
		}
	}
}

void Dig() {
	//kinda wasteful but whatever
	int x = 0;
	int min_x = 0;
	int max_x = 0;
	int y = 0;
	int min_y = 0;
	int max_y = 0;

	//find bounds of grid
	for (const auto& dig : s_Data) {
		x += s_Directions[dig.m_Direction].first * dig.m_Steps;
		y += s_Directions[dig.m_Direction].second * dig.m_Steps;
		
		if (x > max_x) {
			max_x = x;
		}
		if (x < min_x) {
			min_x = x;
		}
		if (y > max_y) {
			max_y = y;
		}
		if (y < min_y) {
			min_y = y;
		}
	}

	int diffx = max_x - min_x;
	int diffy = max_y - min_y;

	//allocate the grid
	for (int y = 0; y < diffy + 1; ++y) {
		s_Grid.push_back(std::string(diffx + 1, '*'));
	}

	//paint edges
	x = std::abs(min_x);
	y = std::abs(min_y);
	for (const auto& dig : s_Data) {
		int next_x = x + s_Directions[dig.m_Direction].first * dig.m_Steps;
		int next_y = y + s_Directions[dig.m_Direction].second * dig.m_Steps;

		bool less_y = y < next_y;
		bool less_x = x < next_x;
		for (int start_y = y; less_y ? start_y < next_y : start_y >= next_y; less_y ? ++start_y : --start_y) {
			for (int start_x = x; less_x ? start_x < next_x : start_x >= next_x; less_x ? ++start_x : --start_x) {
				s_Grid[start_y][start_x] = '#';
			}
		}
		x = next_x; y = next_y;
	}

	//fill in the gaps
	for (y = 0; y < s_Grid.size(); ++y) {
		for (x = 0; x < s_Grid[0].size(); ++x) {
			if (s_Grid[y][x] == '*') {
				FloodFill(x, y);
			}
		}
	}

	size_t total = 0;
	for (const auto& l : s_Grid) {
		total += std::count(l.begin(), l.end(), '#');
	}
	std::cout << total << std::endl;
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
	Dig();

	return 0;
}
