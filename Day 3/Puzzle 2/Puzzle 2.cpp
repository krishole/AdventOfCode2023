#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct SBaseData {
	int start_pos = 0;
	int line = 0;
};

struct SNumberData : SBaseData {
	int number = 0;	
	int length = 0;
};

static std::vector<SBaseData> s_Gears;
static std::vector<SNumberData> s_Numbers;

void ProcessLine(const std::string& line, int line_idx) {
	SNumberData data;

	for (int i = 0; i < (int)line.length(); i++) {
		unsigned char c = line.at(i);

		if (std::isdigit(c)) {
			if (data.length == 0) {
				data.start_pos = i;
				data.line = line_idx;
			}
			++data.length;
			data.number = data.number * 10 + (c - '0');
		} else if (c == '*') {
			s_Gears.push_back({ i, line_idx });
		}

		if (!std::isdigit(c) || i == line.length() - 1) {
			if (data.length > 0) {
				s_Numbers.push_back(data);
				data = SNumberData();
			}
		}
	}
}

bool IsNumberTouchingGear(const SNumberData& number, const SBaseData& gear) {
	for (int y = number.line - 1; y <= number.line + 1; ++y) {
		for (int x = number.start_pos - 1; x <= number.start_pos + number.length; ++x) {
			if (x == gear.start_pos && y == gear.line) {
				return true;
			}
		}
	}
	return false;
}

void CheckEngine() {
	int total = 0;

	for (const SBaseData& gear : s_Gears) {
		std::vector<int> touching;
		for (const SNumberData& number : s_Numbers) {
			if (IsNumberTouchingGear(number, gear)) {
				touching.push_back(number.number);
			}
		}

		if (touching.size() == 2) {
			total += (touching[0] * touching[1]);
		}
	}

	std::cout << "Total: " << total << std::endl;
}

int main() {
	std::ifstream file("engine.txt");
	int total = 0;

	if (file.is_open()) {
		std::string line;
		int line_idx = 0;
		while (std::getline(file, line)) {
			ProcessLine(line, line_idx);
			++line_idx;
		}
		file.close();
	}
	else {
		std::cout << "Unable to open file" << std::endl;
	}

	CheckEngine();

	return 0;
}
