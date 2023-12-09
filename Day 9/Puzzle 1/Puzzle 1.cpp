#include <iostream>
#include <fstream>
#include <numeric>
#include "Utils.h"

//jesus christ
std::vector < std::vector<std::vector<long long>> > s_History;

void ProcessLine(const std::string& line) {
	std::vector<std::vector<long long>> history;
	history.push_back(FindAllNumbersInString(line));
	s_History.push_back(history);
}

void Predict() {
	std::vector<long long> predictions;

	for (std::vector<std::vector<long long>>& history : s_History) {
		bool all_zero = false;

		for (unsigned int i = 0; i < history.size(); ++i) {
			all_zero = true;

			const std::vector<long long>& line = history[i];
			std::vector<long long> next_line;

			if (line.size() > 1) {
				for (unsigned int x = 1; x < line.size(); ++x) {
					long long diff = line[x] - line[x - 1];
					next_line.push_back(diff);
					if (diff != 0) {
						all_zero = false;
					}
				}
			} else {
				next_line.push_back(0);
			}

			history.push_back(next_line);
			if (all_zero) {
				break;
			}
		}

		long long last = 0;
		for (int i = history.size() - 2; i >= 0; --i) {
			std::vector<long long>& line = history[i];
			long long next_num = line.back() + last;
			line.push_back(next_num);
			last = next_num;
		}

		long long last_num = history[0].back();
		predictions.push_back(last_num);
	}

	long long output = std::accumulate(predictions.begin(), predictions.end(), static_cast<long long>(0));
	std::cout << "Total: " << output << std::endl;
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
		Predict();
	} else {
		std::cout << "Unable to open file" << std::endl;
	}

	return 0;
}
