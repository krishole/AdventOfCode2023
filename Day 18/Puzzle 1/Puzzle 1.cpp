#include <iostream>
#include <fstream>
#include <queue>
#include "Utils.h"

std::vector<std::pair<int, int>> s_Directions = { { 0,-1}, {0, 1},{-1, 0}, {1,0} };

struct SPoint {
	int x, y;
};
static std::vector<SPoint> s_Polygon;
static int s_TotalDistance = 0;

void ProcessLine(const std::string& line) {
	std::vector<std::string> chunks = ParseString(line, " ");

	int dir = 0;
	char d = chunks[0][0];
	if (d == 'U') {
		dir = 0;
	} else if (d == 'D') {
		dir = 1;
	} else if (d == 'L') {
		dir = 2;
	} else if (d == 'R') {
		dir = 3;
	}

	int steps = std::stoi(chunks[1]);
	const SPoint last = s_Polygon.back();
	int x = last.x + (s_Directions[dir].first * steps);
	int y = last.y + (s_Directions[dir].second * steps);

	s_TotalDistance += steps;
	s_Polygon.push_back({ x, y });
}

void Dig() {
	//shoelace
	int area = 0;
	size_t n = s_Polygon.size();
	for (size_t i = 0; i < n; ++i) {
		int x1 = s_Polygon[i].x;
		int y1 = s_Polygon[i].y;
		int x2 = s_Polygon[(i + 1) % n].x;
		int y2 = s_Polygon[(i + 1) % n].y;

		area += (x1 * y2 - x2 * y1);
	}

	area = std::abs(area) / 2;
	int total = area + 1 + s_TotalDistance / 2;
	std::cout << "Area: " << total << std::endl;
}


int main() {
	std::ifstream file("input.txt");
	s_Polygon.push_back({ 0, 0 });

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
