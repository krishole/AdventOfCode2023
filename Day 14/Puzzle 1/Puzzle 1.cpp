#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> s_Grid;

void Tilt() {

	for (int y = 1; y < s_Grid.size(); ++y) {
		for (int x = 0; x < s_Grid.size(); ++x) {
			if (s_Grid[y][x] == 'O') {
				int next_y = y;
				bool valid = false;
				while ((next_y -= 1) >= 0 && s_Grid[next_y][x] == '.') {
					valid = true;
				}

				if (valid) {
					s_Grid[y][x] = '.';
					s_Grid[next_y + static_cast<int>(1)][x] = 'O';
				}
			}
		}
	}

	int total = 0;
	for (int y = 0; y < s_Grid.size(); ++y) {
		for (int x = 0; x < s_Grid.size(); ++x) {
			if (s_Grid[y][x] == 'O') {
				total += static_cast<int>(s_Grid.size()) - y;
			}
		}
	}

	std::cout << "Total: " << total << std::endl;
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
	} else {
		std::cout << "Unable to open file" << std::endl;
	}
	Tilt();

	return 0;
}
