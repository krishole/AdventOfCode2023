#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

typedef unsigned long long ull;
struct SRaceData {
	ull m_Time = 0;
	ull m_Distance = 0;
};

static SRaceData s_RaceData;

void ProcessLine(std::string& line) {
	line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
	ull num = std::stoull(line.substr(line.find(':') + 1));

	if (s_RaceData.m_Time == 0) {
		s_RaceData.m_Time = num;
	} else {
		s_RaceData.m_Distance = num;
	}
}

void CheckRace() {
	std::vector<ull> hold_times;

	for (ull i = 1; i < s_RaceData.m_Time; ++i) {
		ull rem_time = s_RaceData.m_Time - i;
		if (i * rem_time > s_RaceData.m_Distance) {
			hold_times.push_back(i);
		}
	}

	std::cout << "Total: " << hold_times.size() << std::endl;
}


int main() {
	std::ifstream file("races.txt");

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			ProcessLine(line);
		}
		CheckRace();
		file.close();
	}
	else {
		std::cout << "Unable to open file" << std::endl;
	}

	return 0;
}
