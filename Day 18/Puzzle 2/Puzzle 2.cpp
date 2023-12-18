#include <iostream>
#include <fstream>
#include "Utils.h"

typedef long long ll;
struct SPoint {
	ll x, y;
};

static std::vector<std::pair<ll, ll>> s_Directions = { { 1,0}, {0, 1},{-1, 0}, {0,-1} };
static std::vector<SPoint> s_Polygon;
static int s_TotalDistance = 0;

void ProcessLine(const std::string& line) {
	std::vector<std::string> chunks = ParseString(line, " ");

	std::string hex = chunks[2].substr(2, 5);
	std::string hex2 = chunks[2].substr(7, 1);

	ll direction = std::stoi(hex2);
	ll steps = std::stoll(hex, 0, 16);

	const SPoint last = s_Polygon.back();
	ll x = last.x + (s_Directions[direction].first * steps);
	ll y = last.y + (s_Directions[direction].second * steps);

	s_Polygon.push_back({ x, y });
	s_TotalDistance += steps;
}

void Dig() {
	//shoelace
	ll area = 0;
	size_t n = s_Polygon.size();
	for (size_t i = 0; i < n; ++i) {
		ll x1 = s_Polygon[i].x;
		ll y1 = s_Polygon[i].y;
		ll x2 = s_Polygon[(i + 1) % n].x;
		ll y2 = s_Polygon[(i + 1) % n].y;

		area += (x1 * y2 - x2 * y1);
	}

	area = std::abs(area) / 2;
	ll total = area + 1 + s_TotalDistance / 2;
	std::cout << "Area: " << total << std::endl;
}

int main() {
	std::ifstream file("input.txt");
	s_Polygon.push_back({ 0,0 });

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
	Dig();

	return 0;
}
