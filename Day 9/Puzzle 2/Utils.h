#include <string>
#include <sstream>
#include <vector>

//find all numbers regardless of delimiter
std::vector<long long> FindAllNumbersInString(const std::string& line) {
	std::istringstream iss(line);
	std::vector<long long> numbers;

	int num;
	while (iss >> num) {
		numbers.push_back(num);
	}

	return numbers;
}