#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct SMap {
	std::string m_Label = "";
	std::vector<std::string> m_Destinations;
};

std::vector<SMap> s_Maps;
std::vector<int> s_Directions;

void ProcessLine(const std::string& line) {
	if (s_Directions.size() == 0) {
		for (const char& c : line) {
			if (c == 'L') {
				s_Directions.push_back(0);
			} else {
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
		}
	}
}

void FindExit() {
	int steps = 0;
	std::string step = "AAA";
	int direction = 0;

	while (step != "ZZZ") {
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
	} else {
		std::cout << "Unable to open file" << std::endl;
	}


	std::cout << "Output" << std::endl;

	return 0;
}
