#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_set>

std::vector<std::string> s_Grid;
static int s_Steps = 64;

struct SVec2 {
	int x;
	int y;

	bool operator==(const SVec2& other) const {
		return x == other.x && y == other.y;
	}
};

namespace std {
	template <>
	struct hash<SVec2> {
		size_t operator()(const SVec2& c) const {
			return hash<int>()(c.x) ^ (hash<int>()(c.y) << 1);
		}
	};
}

static SVec2 s_Directions[4] = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };

bool IsValidCoord(SVec2 c) {
	return (c.y >= 0 && c.y < s_Grid.size() && c.x >= 0 && c.x < s_Grid[0].size());
}

std::vector<SVec2> GetNeighbours(const SVec2& c) {
	std::vector<SVec2> neighbors;
	for (SVec2& dir : s_Directions) {
		SVec2 next = { c.x + dir.x, c.y + dir.y };
		if (IsValidCoord(next) && s_Grid[next.y][next.x] != '#') {
			neighbors.push_back(next);
		}
	}
	return neighbors;
}

size_t Run() {
	std::unordered_set<SVec2> queue;

	for (int y = 0; y < s_Grid.size(); ++y) {
		for (int x = 0; x < s_Grid.size(); ++x) {
			SVec2 c{ x, y };
			if (s_Grid[c.y][c.x] == 'S') {
				queue.insert(c);
			}
		}
	}

	std::unordered_set<SVec2> next;
	for (int i = 0; i < s_Steps; ++i) {
		for (const SVec2& p : queue) {
			std::vector<SVec2> neighbours = GetNeighbours(p);
			next.insert(neighbours.begin(), neighbours.end());
		}
		queue = next;
		next.clear();
	}

	std::cout << "Gardens to Visit: " << queue.size() << std::endl;
	return queue.size();
}

std::vector<size_t> SimplifiedLagrange(std::vector<unsigned int>& values) {
	size_t a = (values[0] / 2) - values[1] + (values[2] / 2);
	size_t b = (-3 * (values[0] / 2)) + (2 * values[1]) - (values[2] / 2);
	size_t c = values[0];

	return std::vector<size_t>{a, b, c};
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

	std::vector<std::string> original_grid = s_Grid;
	s_Steps = 65;
	std::vector<unsigned int> values;
	values.push_back(Run());

	s_Grid = original_grid;
	s_Steps = 65 + 131;
	values.push_back(Run());

	s_Grid = original_grid;
	s_Steps = (65 + 131) * 2;
	values.push_back(Run());

	std::vector<size_t> poly = SimplifiedLagrange(values);
	unsigned int target = 26501365 / 131;

	size_t output = poly[0] * target * target + poly[1] * target + poly[2];
	std::cout << "Output: " << output << std::endl;

	return 0;
}
