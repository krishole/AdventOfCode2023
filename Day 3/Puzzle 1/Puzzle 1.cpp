#include <iostream>
#include <fstream>
#include <string>
#include <vector>

static std::vector<std::string> s_Engine;

bool HasAdjacentSymbol(int line, int char_pos, int length) {
	const int start_line = std::max(line - 1, 0);
	const int end_line = std::min(line + 1, static_cast<int>(s_Engine.size() - 1));

	for (int y = start_line; y <= end_line; ++y) {
		int start_pos = std::max(char_pos - 1, 0);
		int end_pos = std::min(char_pos + length, static_cast<int>(s_Engine[y].length() - 1));

		for (int x = start_pos; x <= end_pos; ++x) {
			unsigned char c = s_Engine[y].at(x);
			if (!std::isdigit(c) && c != '.') {
				return true;
			}
		}
	}

	return false;
}

void CheckEngine() {
	int total = 0;

	for (unsigned int y = 0; y < s_Engine.size(); ++y) {
		int found_number = false;
		int number = 0;
		int number_start_x = 0;
		int number_length = 0;

		for (unsigned int x = 0; x < s_Engine[y].length(); ++x) {

			unsigned char c = s_Engine[y].at(x);

			if (std::isdigit(c)) {
				if (!found_number) {
					number_start_x = x;
				}
				found_number = true;
				number = number * 10 + (c - '0');
				++number_length;
			}
			
			if(!std::isdigit(c) || x == s_Engine[y].length() - 1) {
				if (found_number) {
					//map symbols
					if (HasAdjacentSymbol(y, number_start_x, number_length)) {
						total += number;
					}

					found_number = false;
					number = 0;
					number_start_x = 0;
					number_length = 0;
				}
			}
		}

		std::cout << std::endl << std::endl;
	}

	std::cout << "Total: " << total << std::endl;
}

int main() {
	std::ifstream file("engine.txt");
	int total = 0;

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			s_Engine.push_back(line);
		}
		file.close();
	} else {
		std::cout << "Unable to open file" << std::endl;
	}

	CheckEngine();

	return 0;
}
