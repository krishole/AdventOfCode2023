#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

struct SVec2 {
	int x;
	int y;
};

std::vector<std::string> s_Universe;

void ExpandUniverse() {
	std::vector<int> missing_cols;
	for (unsigned int y = 0; y < s_Universe.size(); ++y) {
		if (s_Universe[y].find('#') == std::string::npos) {
			missing_cols.push_back(y);
		}
	}

	std::vector<int> missing_rows;
	for (unsigned int x = 0; x < s_Universe[0].size(); ++x) {
		bool found = false;
		for (unsigned int y = 0; y < s_Universe.size(); ++y) {
			if (s_Universe[y][x] == '#') {
				found = true;
				break;
			}
		}
		if (!found) {
			missing_rows.push_back(x);
		}
	}

	std::vector<SVec2> galaxies;
	int y_off = 0;

	for (unsigned int y = 0; y < s_Universe.size(); ++y) {
		for (unsigned int i = 0; i < missing_cols.size(); ++i) {
			if (missing_cols[i] == y) {
				y_off += 1;
			}
		}

		int x_off = 0;
		for (unsigned int x = 0; x < s_Universe[y].size(); ++x) {
			for (unsigned int i = 0; i < missing_rows.size(); ++i) {
				if (missing_rows[i] == x) {
					x_off += 1;
				}
			}

			if (s_Universe[y][x] == '#') {
				galaxies.push_back(SVec2(x + x_off, y + y_off));
			}
		}
	}

	int total = 0;
	for (unsigned int i = 0; i < galaxies.size(); ++i) {
		const SVec2& p1 = galaxies[i];
		for (unsigned int j = i + 1; j < galaxies.size(); ++j) {
			const SVec2& p2 = galaxies[j];
			total += std::abs(p2.y - p1.y) + std::abs(p2.x - p1.x);
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
			s_Universe.push_back(line);
		}
		file.close();
		ExpandUniverse();
	}
	else {
		std::cout << "Unable to open file" << std::endl;
	}
	return 0;
}