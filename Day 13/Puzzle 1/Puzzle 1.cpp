#include <iostream>
#include <fstream>
#include <string>
#include <vector>

static std::vector<std::vector<std::string>> s_Patterns;

void ProcessLine(const std::string& line) {
	if (line.length() == 0) {
		std::vector<std::string> pattern;
		s_Patterns.push_back(pattern);
		return;
	}

	if (s_Patterns.size() == 0) {
		std::vector<std::string> pattern;
		s_Patterns.push_back(pattern);
	}

	s_Patterns.back().push_back(line);
}

int FindReflection(const std::vector<std::string>& pattern) {
	for (auto& line : pattern) {
		std::cout << line << std::endl;
	}

	int rows = static_cast<int>(pattern.size());
	int cols = static_cast<int>(pattern[0].size());

	int vertical = 0;
	int horizontal = 0;

	for (int x = 0; x < cols - 1; ++x) {
		bool is_reflection = true;

		for (int i = 1; i < cols - x; ++i) {
			if (x + i > cols || x - i + 1 < 0) {
				continue;
			}

			for (int y = 0; y < rows; ++y) {
				if (pattern[y][x - i + 1] != pattern[y][x + i]) {
					is_reflection = false;
					break;
				}
			}
			if (!is_reflection)
				break;
		}

		if (is_reflection) {
			return x + 1;
		}
	}

	//find horizontal
	for(int y = 0; y < rows - 1; ++y){
		bool is_reflection = true;
		for (int i = 1; i < rows - y; ++i) {
			if (y + i > rows || y - i + 1 < 0) {
				continue;
			}

			for (int x = 0; x < cols - 1; ++x) {
				if (pattern[y - i + 1][x] != pattern[y +  i][x]) {
					is_reflection = false;
					break;
				}
			}
			if (!is_reflection)
				break;
		}

		if (is_reflection) {
			return (y + 1) * 100;
		}
	}

	return 0;
}


int main() {
	std::ifstream file("input.txt");

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			ProcessLine(line);
		}
		file.close();
	} else {
		std::cout << "Unable to open file" << std::endl;
	}

	int total = 0;
	for (const auto& pattern : s_Patterns) {
		total += FindReflection(pattern);
	}

	std::cout << "Output: " << total << std::endl;

	return 0;
}
