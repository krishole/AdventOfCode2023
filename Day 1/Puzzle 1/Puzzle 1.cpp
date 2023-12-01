#include <iostream>
#include <fstream>
#include <string>

int main() {
	std::ifstream file("calibration.txt");
	int total = 0;

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			auto firstIterator = std::find_if(line.begin(), line.end(), [](char c) { return std::isdigit(c); });
			auto lastIterator = std::find_if(line.rbegin(), line.rend(), [](char c) { return std::isdigit(c); });

			if (firstIterator != line.end() && lastIterator != line.rend()) {
				int first = *firstIterator - '0';
				int last = *lastIterator - '0';

				total += (first * 10) + last;
			}
		}
		file.close();
	} else {
		std::cout << "Unable to open file" << std::endl;
	}

	std::cout << total << std::endl;

	return 0;
}
