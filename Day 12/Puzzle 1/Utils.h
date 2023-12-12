#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> ParseString(const std::string& line, const std::string& delimiters) {
	std::vector<std::string> chunks;
	size_t start_pos = 0;
	size_t delim_pos = line.find_first_of(delimiters);

	while (delim_pos != std::string::npos) {
		std::string chunk = line.substr(start_pos, delim_pos - start_pos);
		if (chunk.length() > 0) {
			chunks.push_back(line.substr(start_pos, delim_pos - start_pos));
		}
		start_pos = delim_pos + 1;
		delim_pos = line.find_first_of(delimiters, start_pos);
	}

	if (start_pos < line.size()) {
		std::string chunk = line.substr(start_pos);
		if (chunk.length() > 0) {
			chunks.push_back(line.substr(start_pos));
		}
	}

	return chunks;
}

std::vector<int> FindAllNumbersInString(const std::string& line, char delimiter = ',') {
	std::istringstream iss(line);
	std::vector<int> numbers;

	std::string token;
	while (std::getline(iss, token, delimiter)) {
		int num = std::stoi(token);
		numbers.push_back(num);
	}

	return numbers;
}