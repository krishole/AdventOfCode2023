#include <string>
#include <vector>

//find all numbers regardless of delimiter
std::vector<int> FindAllNumbersInString(const std::string& line) {
	std::vector<int> numbers;

	int number = 0;
	bool foundNumber = false;

	for (char c : line) {
		if (std::isdigit(c)) {
			foundNumber = true;
			number = number * 10 + (c - '0');
		}
		else if (foundNumber) {
			foundNumber = false;
			numbers.push_back(number);
			number = 0;
		}
	}

	if (foundNumber) {
		numbers.push_back(number);
		number = 0;
	}

	return numbers;
}