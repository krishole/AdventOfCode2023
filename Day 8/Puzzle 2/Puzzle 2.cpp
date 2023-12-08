#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>

typedef unsigned long long ull;

struct SMap {
	std::string m_Label = "";
	std::vector<std::string> m_Destinations;
};

std::vector<std::string> s_Starts;
std::vector<SMap> s_Maps;
std::vector<int> s_Directions;

void ProcessLine(const std::string& line) {
	if (s_Directions.size() == 0) {
		for (const char& c : line) {
			if (c == 'L') {
				s_Directions.push_back(0);
			}
			else {
				s_Directions.push_back(1);
			}
		}
	} else {
		if (line.size() > 0) {
			SMap map;
			map.m_Label = line.substr(0, 3);
			map.m_Destinations.push_back(line.substr(7, 3));
			map.m_Destinations.push_back(line.substr(12, 3));

			s_Maps.push_back(map);

			if (map.m_Label.at(2) == 'A') {
				s_Starts.push_back(map.m_Label);
			}
		}
	}
}

ull CalculateLCM(ull a, ull b) {
	return (a * b) / std::gcd(a, b);
}

ull FindLCM(const std::vector<ull>& numbers) {
	return std::accumulate(numbers.begin(), numbers.end(), ull(1), CalculateLCM);
}

void FindExit() {
	int direction = 0;
	std::vector<ull> step_counts;

	for (std::string& step : s_Starts) {
		int steps = 0;
		while (step.at(2) != 'Z') {
			for (const SMap& map : s_Maps) {
				if (map.m_Label == step) {
					step = map.m_Destinations[s_Directions[direction]];
					++steps;
					break;
				}
			}

			++direction;
			if (direction == s_Directions.size()) {
				direction = 0;
			}
		}
		step_counts.push_back(steps);
	}

	ull steps = FindLCM(step_counts);
	std::cout << "Steps: " << steps << std::endl;
}

int main() {
	std::ifstream file("map.txt");
	int total = 0;

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			ProcessLine(line);
		}
		file.close();
		FindExit();
	}
	else {
		std::cout << "Unable to open file" << std::endl;
	}

	return 0;
}
