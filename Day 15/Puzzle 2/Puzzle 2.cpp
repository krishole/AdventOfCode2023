#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Utils.h"
#include <unordered_map>

struct SLens {
	std::string m_Label;
	int m_Focal_Length;
};

std::vector<std::vector<SLens>> s_Boxes(256);

int hash(std::string& input) {
	int value = 0;
	for (const char c : input) {
		value += static_cast<int>(c);
		value *= 17;
		value %= 256;
	}
	return value;
}

void ProcessLine(const std::string& line) {
	std::vector<std::string> steps = ParseString(line, ",");
	for (const std::string& step : steps) {
		size_t i = 0;
		if ((i = step.find('=')) != std::string::npos) {
			std::string label = step.substr(0, i);
			int length = std::stoi(step.substr(i + 1));
			int box_id = hash(label);

			auto lens = std::ranges::find_if(s_Boxes[box_id], [=](const SLens& lens) {return lens.m_Label == label; });
			if (lens != s_Boxes[box_id].end()) {
				lens->m_Focal_Length = length;
			} else {
				s_Boxes[box_id].push_back({ label, length });
			}
		} else if ((i = step.find('-')) != std::string::npos) {
			std::string label = step.substr(0, i);
			int box_id = hash(label);

			auto lens = std::ranges::find_if(s_Boxes[box_id], [=](const SLens& lens) {return lens.m_Label == label; });
			if (lens != s_Boxes[box_id].end()) {
				s_Boxes[box_id].erase(lens);
			}
		}
	}

	int result = 0;
	for (int i = 0; i < 256; ++i) {
		for (int j = 0; j < s_Boxes[i].size(); ++j) {
			int box = (1 + i) * (j + 1) * (s_Boxes[i][j].m_Focal_Length);
			result += box;
		}
	}

	std::cout << "Hash: " << result << std::endl;
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
	}
	else {
		std::cout << "Unable to open file" << std::endl;
	}

	return 0;
}
