#include <iostream>
#include <fstream>
#include "Utils.h"

struct SRaceData {
	int m_Time = 0;
	int m_Distance = 0;
};

static std::vector<SRaceData> s_RaceData;

void ProcessLine(const std::string& line) {
	const std::vector<int> numbers = FindAllNumbersInString(line);

	bool time = false;
	if (s_RaceData.size() == 0) {
		s_RaceData.resize(numbers.size());
		time = true;
	}

	for (int i = 0; i < static_cast<int>(numbers.size()); ++i) {
		if (time) {
			s_RaceData[i].m_Time = numbers[i];
		} else {
			s_RaceData[i].m_Distance = numbers[i];
		}
	}
}

void CheckRaces() {
	int total = 1;
	for (const SRaceData& race : s_RaceData) {
		std::vector<int> hold_times;

		for (int i = 1; i < race.m_Time; ++i) {
			int rem_time = race.m_Time - i;
			if (i * rem_time > race.m_Distance) {
				hold_times.push_back(i);
			}
		}

		total *= hold_times.size();
	}

	std::cout << "Margin: " << total << std::endl;
}


int main() {
	std::ifstream file("races.txt");

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			ProcessLine(line);
		}
		CheckRaces();
		file.close();
	} else {
		std::cout << "Unable to open file" << std::endl;
	}

	return 0;
}
