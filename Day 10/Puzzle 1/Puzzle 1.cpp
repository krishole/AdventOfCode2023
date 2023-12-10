#include <iostream>
#include <fstream>
#include <unordered_map>
#include <array>
#include <string>
#include <vector>

enum EDirection {
	E_NORTH,
	E_EAST,
	E_SOUTH,
	E_WEST
};

struct SVec2 {
	int x;
	int y;

	bool operator==(const SVec2& other) const {
		return (x == other.x) && (y == other.y);
	}
};

static const std::unordered_map<char, std::array<EDirection,2>> s_Pieces = {
	{ '|', {E_NORTH, E_SOUTH} },
	{ '-', {E_EAST, E_WEST} },
	{ 'L', {E_NORTH, E_EAST} },
	{ 'J', {E_NORTH, E_WEST} },
	{ '7', {E_WEST, E_SOUTH} },
	{ 'F', {E_EAST, E_SOUTH} }
};

static constexpr std::array<std::pair<SVec2, EDirection>, 4> s_Search = {
		std::make_pair(SVec2(0, -1), E_NORTH),
		std::make_pair(SVec2(1, 0), E_EAST),
		std::make_pair(SVec2(0, 1), E_SOUTH),
		std::make_pair(SVec2(-1, 0), E_WEST)
};

std::vector<std::string> s_Tiles;
std::unordered_map<EDirection, EDirection> s_OppositeDirection = { {E_NORTH, E_SOUTH}, {E_SOUTH, E_NORTH}, {E_EAST, E_WEST}, {E_WEST, E_EAST} };


void ProcessLine(const std::string& line) {
	s_Tiles.push_back(line);
}

bool ConnectsFrom(EDirection from, const std::array<EDirection, 2>& piece) {
	for (EDirection dir : piece) {
		if (dir == from) {
			return true;
		}
	}

	return false;
}

EDirection ConnectsTo(EDirection from, const std::array<EDirection, 2>& piece) {
	if (piece[0] == from) {
		return piece[1];
	}
	else {
		return piece[0];
	}
}

SVec2 FindStart() {
	for (unsigned int y = 0; y < s_Tiles.size(); ++y) {
		for (unsigned int x = 0; x < s_Tiles[y].size(); ++x) {
			if (s_Tiles[y].at(x) == 'S') {
				return SVec2(x, y);
			}
		}
	}

	return SVec2(-1, -1);
}

SVec2 SearchFromDirection(EDirection dir) {
	for (const auto s : s_Search) {
		if (s.second == dir) {
			return s.first;
		}
	}
	return SVec2( -1, -1 );
}

SVec2 FindFirstConnection(const SVec2& start, EDirection& dir) {
	for (const auto& s : s_Search) {
		unsigned int y = start.y + s.first.y;
		unsigned int x = start.x + s.first.x;

		if (y >= 0 && y <= s_Tiles.size() && x >= 0 && x <= s_Tiles[y].size()) {
			const char c = s_Tiles[y].at(x);

			if (c != '.') {
				dir = s_OppositeDirection[s.second];
				if (ConnectsFrom(dir, s_Pieces.at(c))) {
					return SVec2(x, y);
				}
			}
		}
	}

	return SVec2(-1, -1);
}

void CheckPipes() {
	SVec2 pos = FindStart();
	EDirection from_dir;
	pos = FindFirstConnection(pos, from_dir);

	char next = s_Tiles[pos.y].at(pos.x);
	std::vector<char> pipes = { 'S', next };

	while (next != 'S') {
		EDirection connection_dir = ConnectsTo(from_dir, s_Pieces.at(next));
		const auto search = SearchFromDirection(connection_dir);

		next = s_Tiles[pos.y + search.y].at(pos.x + search.x);

		if (next != 'S' && next != '.') {
			from_dir = s_OppositeDirection[connection_dir];
			pipes.push_back(next);
			pos.x += search.x;
			pos.y += search.y;
		}
	}

	std::cout << pipes.size() / 2 << std::endl;
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

	CheckPipes();
	return 0;
}
