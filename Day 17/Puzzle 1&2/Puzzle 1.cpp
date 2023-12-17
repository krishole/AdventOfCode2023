#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <string>

std::vector<std::string> s_Grid;

const std::pair<int, int> s_Directions[4] = {
	{0, -1}, {0, 1}, {-1, 0}, {1, 0}
};
const std::pair<int, int> s_OppositeDirections[4] = {
	{0, 1}, {0, -1}, {1, 0}, {-1, 0}
};

struct SPrioritizedItem {
	int m_Priority;
	//Position, Steps, Direction
	std::pair<int, int> m_Position;
	int m_Steps;
	std::pair<int, int> m_Direction;

	bool operator<(const SPrioritizedItem& other) const {
		return m_Priority > other.m_Priority;
	}
};

int DirectionToInt(std::pair<int, int> direction) {
	for (int i = 0; i < 4; ++i) {
		if (s_Directions[i] == direction) {
			return i;
		}
	}
	return -1;
}

int GetBestPathHeat(int min_steps, int max_steps) {
	std::priority_queue<SPrioritizedItem> queue;
	queue.push({ 0,{0, 0}, 0, s_Directions[1]} );
	queue.push({ 0, {0, 0}, 0, s_Directions[3]} );

	//position, direction, steps
	std::map<std::pair<int, int>, std::vector<std::vector<int>>> best_heats;
	for (int y = 0; y < s_Grid.size(); ++y) {
		for (int x = 0; x < s_Grid[0].size(); ++x) {
			best_heats[std::make_pair(x, y)] = std::vector<std::vector<int>>(4, std::vector<int>(max_steps, -1));
		}
	}

	auto IsBestHeat = [&](int steps, std::pair<int, int> position, int cost, std::pair<int, int> direction) {
		int cur_best = best_heats[position][DirectionToInt(direction)][steps - 1];
		if (cost < cur_best || cur_best == -1) {
			best_heats[position][DirectionToInt(direction)][steps - 1] = cost;
			return true;
		}
		return false;
	};

	std::pair<int, int> end_pos = { static_cast<int>(s_Grid.size() - 1), static_cast<int>(s_Grid[0].size() - 1) };
	while (!queue.empty()) {
		SPrioritizedItem item = queue.top();
		queue.pop();

		int cost = item.m_Priority;
		std::pair<int, int> position = item.m_Position;
		int steps = item.m_Steps;
		std::pair<int, int> direction = item.m_Direction;

		if (position == end_pos && steps >= min_steps) {
			return cost;
		}

		if (steps >= min_steps) {
			for(std::pair<int, int> new_dir : s_Directions) {
				if (new_dir != s_OppositeDirections[DirectionToInt(direction)] && new_dir != direction) {
					int x = position.first + new_dir.first;
					int y = position.second + new_dir.second;

					if (x >= 0 && x < s_Grid[0].size() && y >= 0 && y < s_Grid.size()) {
						int next_cost = s_Grid[y][x] - '0';
						if (IsBestHeat(1, { x, y }, cost + next_cost, new_dir)) {
							queue.push({ cost + next_cost, {x, y}, 1, new_dir });
						}
					}
				}
			}
		}

		if (steps < max_steps) {
			int x = position.first + direction.first;
			int y = position.second + direction.second;

			if (x >= 0 && x < s_Grid[0].size() && y >= 0 && y < s_Grid.size()) {
				int next_cost = s_Grid[y][x] - '0';
				if (IsBestHeat(steps + 1, { x, y }, cost + next_cost, direction)) {
					queue.push({ cost + next_cost, {x, y}, steps + 1, direction} );
				}
			}
		}
	}

	return -1;
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
	
	int part1 = GetBestPathHeat(1, 3);
	int part2 = GetBestPathHeat(4, 10);

	std::cout << "Part 1: " << part1 << std::endl;
	std::cout << "Part 2: " << part2 << std::endl;

	return 0;
}
