#include <iostream>
#include <fstream>
#include <unordered_map>
#include <array>
#include <string>
#include <vector>
#include <queue>

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

static const std::unordered_map<char, std::array<EDirection, 2>> s_Pieces = {
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
std::vector<SVec2> s_MainLoop;
std::unordered_map<EDirection, EDirection> s_OppositeDirection = { {E_NORTH, E_SOUTH}, {E_SOUTH, E_NORTH}, {E_EAST, E_WEST}, {E_WEST, E_EAST} };

bool ConnectsFrom(EDirection from, const char c) {
	if (c == 'S') {
		return true;
	}
	auto it = s_Pieces.find(c);
	if (it == s_Pieces.end()) {
		return false;
	}

	for (EDirection dir : it->second) {
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
	return SVec2(-1, -1);
}

SVec2 FindFirstConnection(const SVec2& start, EDirection& dir) {
	for (const auto& s : s_Search) {
		unsigned int x = start.x + s.first.x;
		unsigned int y = start.y + s.first.y;

		if (y >= 0 && y <= s_Tiles.size() && x >= 0 && x <= s_Tiles[y].size()) {
			const char c = s_Tiles[y].at(x);

			if (c != '.') {
				dir = s_OppositeDirection[s.second];
				if (ConnectsFrom(dir, c)) {
					return SVec2(x, y);
				}
			}
		}
	}

	return SVec2(-1, -1);
}

//this is awful tbh
static const std::unordered_map<EDirection, std::pair<SVec2, std::vector<std::pair<EDirection, SVec2>>>> s_ConnectionSearch = {
	//from dir  //start   //end points
	{ E_SOUTH, { {0, -1},  {   {E_NORTH, {0, 1}}, {E_EAST, {-1, 0}}, {E_WEST, {1,0}} } } },
	{ E_NORTH, { {0,  1},  {   {E_EAST, {-1, 0}}, {E_WEST, {1, 0}} } } },
	{ E_EAST,  { {-1, 0},  {   {E_WEST, {1, 0}} } } }
};

bool IsValidPosition(std::vector<std::string>& grid, SVec2 pos, char& c) {
	if (pos.y >= 0 && pos.y < static_cast<int>(grid.size()) && pos.x >= 0 && pos.x < static_cast<int>(grid[pos.y].size())) {
		c = grid[pos.y].at(pos.x);
		return true;
	}
	return false;
}

void BuildLargerMaze() {
	std::vector<std::string> maze;

	//Insert empty cells between all existing cells
	for (unsigned int y = 0; y < s_Tiles.size(); ++y) {
		const std::string& line = s_Tiles[y];

		std::string new_line;
		for (unsigned int x = 0; x < line.size(); ++x) {

			new_line += line[x];
			if (x != line.size() - 1) {
				new_line += '+';
			}
		}
		maze.push_back(new_line);

		if (y != s_Tiles.size() - 1) {
			new_line.clear();
			for (unsigned int x = 0; x < line.size(); ++x) {
				if (x != line.size() - 1) {
					new_line += "+.";
				} else {
					new_line += '+';
				}
			}
			maze.push_back(new_line);
		}
	}

	//Replace empty cells with connecting cells if connections exist
	for (unsigned int y = 0; y < maze.size(); ++y) {
		for (unsigned int x = 0; x < maze[y].size(); ++x) {
			char& c = maze[y].at(x);

			if (c == '+') {
				bool found = false;
				//check all connecting directions for pipes
				for (const auto& s : s_ConnectionSearch) {
					char c1 = 0;
;
					if (IsValidPosition(maze, SVec2(x + s.second.first.x, y + s.second.first.y), c1) && ConnectsFrom(s.first, c1)) {

						//check all possible connections
						for (const auto& endpoint : s.second.second) {
							char c2 = 0;
							const SVec2& off = endpoint.second;

							if (IsValidPosition(maze, SVec2(x + off.x, y + off.y), c2) && ConnectsFrom(endpoint.first, c2)) {
								c = '&';
								found = true;
								break;
							}
						}
					}

					if (found)
						break;
				}

				if (!found)
					c = '.';
			}
		}
	}

	//replace main loop with '&'
	for (unsigned int y = 0; y < maze.size(); ++y) {
		for (unsigned int x = 0; x < maze[y].size(); ++x) {
			char& c = maze[y].at(x);
			if (c != '.' && c != '&') {
				c = '&';
			}
		}
	}

	s_Tiles = maze;
}

void ShrinkMaze() {
	std::vector<std::string> board;

	for (unsigned int y = 0; y < s_Tiles.size(); y += 2) {
		std::string line;
		for (unsigned int x = 0; x < s_Tiles[y].size(); x += 2) {
			line += s_Tiles[y].at(x);
		}
		board.push_back(line);
	}

	s_Tiles = board;
}

void FloodFill(SVec2 start) {
	std::queue<SVec2> queue;
	bool enclosed = true;

	queue.push(start);

	std::vector<SVec2> checked;
	//include starting tile
	s_Tiles[start.y].at(start.x) = '#';

	while (!queue.empty()) {
		SVec2 start_pos = queue.front();
		queue.pop();
		checked.push_back(start_pos);

		for (const auto& s : s_Search) {
			SVec2 pos = { start_pos.x + s.first.x, start_pos.y + s.first.y };

			bool within_bounds = (pos.y >= 0 && pos.y < static_cast<int>(s_Tiles.size()) && pos.x >= 0 && pos.x < static_cast<int>(s_Tiles[pos.y].size()));
			if (!within_bounds) {
				enclosed = false;
			} else {
				char c = s_Tiles[pos.y].at(pos.x);
				if (c != '0' && c != '&' && c != 'I') {
					queue.push(pos);
					s_Tiles[pos.y].at(pos.x) = 'I';
				} else if (c == '&') {
					//check if we can squeeze through the pipes


				}
			}
		}
	}

	if (!enclosed) {
		for (const auto& p : checked) {
			s_Tiles[p.y].at(p.x) = '0';
		}
	}
}

void PrintBoard() {
	for (unsigned int y = 0; y < s_Tiles.size(); ++y) {
		std::cout << s_Tiles[y] << std::endl;
	}
	std::cout << std::endl;
}

bool IsInMainLoop(SVec2 pos) {
	for (const SVec2& p : s_MainLoop) {
		if (p == pos) {
			return true;
		}
	}
	return false;
}

int CountTiles() {
	int total = 0;
	for (const std::string& line : s_Tiles) {
		total += static_cast<int>(std::count(line.begin(), line.end(), 'I'));
	}
	return total;
}

void CheckPipes() {
	SVec2 pos = FindStart();
	EDirection from_dir;
	s_MainLoop.push_back(pos);

	
	pos = FindFirstConnection(pos, from_dir);
	s_MainLoop.push_back(pos);

	char next = s_Tiles[pos.y].at(pos.x);

	//Find the main loop
	while (next != 'S') {
		EDirection connection_dir = ConnectsTo(from_dir, s_Pieces.at(next));
		const auto search = SearchFromDirection(connection_dir);

		next = s_Tiles[pos.y + search.y].at(pos.x + search.x);

		if (next != 'S' && next != '.') {
			from_dir = s_OppositeDirection[connection_dir];
			pos.x += search.x;
			pos.y += search.y;
			s_MainLoop.push_back(pos);
		}
	}

	//clean the grid of everything except the loop
	for (unsigned int y = 0; y < s_Tiles.size(); ++y) {
		for (unsigned int x = 0; x < s_Tiles[y].size(); ++x) {
			if (!IsInMainLoop(SVec2(x, y))) {
				s_Tiles[y].at(x) = '.';
			}
		}
	}

	BuildLargerMaze();
	for (unsigned int y = 0; y < s_Tiles.size(); ++y) {
		for (unsigned int x = 0; x < s_Tiles[y].size(); ++x) {
			char c = s_Tiles[y].at(x);
			if (c != '0' && c != '&' && c != 'I') {
				FloodFill(SVec2(x, y));
			}
		}
	}

	ShrinkMaze();
	//std::cout << "Shrunk Board: " << std::endl;
	//PrintBoard();

	std::cout << CountTiles() << std::endl;
}

int main() {
	std::ifstream file("input.txt");
	int total = 0;

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			s_Tiles.push_back(line);
		}
		file.close();
	}
	else {
		std::cout << "Unable to open file" << std::endl;
	}

	CheckPipes();
	return 0;
}
