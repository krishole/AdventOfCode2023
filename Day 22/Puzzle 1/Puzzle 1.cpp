#include <iostream>
#include <fstream>
#include "Utils.h"

void ProcessLine(const std::string& line) {
	
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
	} else {
		std::cout << "Unable to open file" << std::endl;
	}


	std::cout << "Output" << std::endl;

	return 0;
}
