#include <string>
#include <sstream>
#include <vector>

namespace std {
	// Custom hash function for std::vector<int>
	template<>
	struct hash<std::vector<int>> {
		size_t operator()(const std::vector<int>& v) const {
			size_t seed = 0;
			std::hash<int> hasher;

			for (const int& i : v) {
				seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}

			return seed;
		}
	};

	// Custom hash function for std::tuple<std::string, int, std::vector<int>, int, int>
	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	struct hash<std::tuple<T1, T2, T3, T4, T5>> {
		size_t operator()(const std::tuple<T1, T2, T3, T4, T5>& tuple) const {
			size_t seed = 0;
			auto hasher = std::hash<T1>{};

			seed ^= hasher(std::get<0>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<T2>{}(std::get<1>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<T3>{}(std::get<2>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<T4>{}(std::get<3>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<T5>{}(std::get<4>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

			return seed;
		}
	};
}

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
