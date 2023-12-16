#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

std::vector<std::string> s_Grid;
std::unordered_map<size_t, std::vector<std::pair<int, int>>> s_Visited;
std::vector<std::pair<int, int>> s_Tiles;

void PrintGrid() {
	for (const auto& l : s_Grid) {
		std::cout << l << std::endl;
	}
	std::cout << std::endl;
}

char CharFromDir(int dir_x, int dir_y) {
	if (dir_x == -1) {
		return '<';
	} else if (dir_x == 1) {
		return '>';
	} else if (dir_y == -1) {
		return '^';
	} else {
		return 'v';
	}
}

bool IsVisited(int x, int y, int dir_x, int dir_y) {
	const size_t key = (y * s_Grid[0].size()) + x;
	if (s_Visited.find(key) != s_Visited.end()) {
		for (const auto dir : s_Visited[key]) {
			if (dir.first == dir_x && dir.second == dir_y) {
				return true;
			}
		}
	}
	s_Visited[key].push_back(std::make_pair(dir_x, dir_y));
	return false;
}

void Beam(int x, int y, int dir_x, int dir_y) {
	if (x < 0 || x >= s_Grid[0].size() || y < 0 || y >= s_Grid.size()) {
		return;
	}

	char c = s_Grid[y][x];
	if (c != '.' && IsVisited(x, y, dir_x, dir_y)) {
		return;
	}
	
	s_Tiles.push_back(std::make_pair(x, y));

	if (c == '.') {
		Beam(x + dir_x, y + dir_y, dir_x, dir_y);
	} else if (c == '\\') {
		Beam(x + dir_y, y + dir_x, dir_y, dir_x);
	} else if (c == '/' ){
		Beam(x + (dir_y * -1), y + (dir_x * -1), dir_y * -1, dir_x * -1);
	} else if (c == '-') {
		if (dir_x != 0) {
			Beam(x + dir_x, y + dir_y, dir_x, dir_y);
		} else {
			Beam(x - 1, y, -1, 0);
			Beam(x + 1, y, 1, 0);
		}
	} else if (c == '|') {
		if (dir_y != 0) {
			Beam(x + dir_x, y + dir_y, dir_x, dir_y);
		} else {
			Beam(x, y - 1, 0, -1);
			Beam(x, y + 1, 0, 1);
		}
	}
}

void EnergizeTiles() {
	for (const auto&tile : s_Tiles) {
		s_Grid[tile.second][tile.first] = '#';
	}

	PrintGrid();
	size_t total = 0;
	for (const std::string& line : s_Grid) {
		total += std::count(line.begin(), line.end(), '#');
	}

	std::cout << std::endl << "Output: " << total << std::endl;
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
	} else {
		std::cout << "Unable to open file" << std::endl;
	}

	Beam(0, 0, 1, 0);
	EnergizeTiles();

	return 0;
}
