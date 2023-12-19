#include <iostream>
#include <fstream>
#include "Utils.h"
#include <map>

struct SRule {
	bool m_Lt = false;
	int m_Limit = 0;
	char m_Rule = 0;
	std::string m_Dest;
};

struct SOperation {
	std::vector<SRule> m_Rules;
	std::string m_ElseDest;
};

bool s_Workflow = true;
static std::map<std::string, SOperation> s_Workflows;
static std::vector<std::map<char, int>> s_Parts;

void ProcessLine(const std::string& line) {
	if (line.length() == 0) {
		s_Workflow = false;
		return;
	}

	if (s_Workflow) {
		auto chunks = ParseString(line, "{");

		SOperation new_op;
		auto ops = ParseString(chunks[1], ",}");

		for (const auto& op : ops) {
			if (op.find(':') != std::string::npos) {
				SRule new_rule;
				new_rule.m_Rule = op[0];
				new_rule.m_Lt = op[1] == '<';

				int spl = static_cast<int>(op.find(':'));
				new_rule.m_Limit = std::stoi(op.substr(2, spl - 2));
				new_rule.m_Dest = op.substr(spl + 1);

				new_op.m_Rules.push_back(new_rule);
			} else {
				new_op.m_ElseDest = op;
			}
		}

		s_Workflows[chunks[0]] = new_op;
	} else {
		std::map<char, int> new_part;
		auto chunks = ParseString(line, "{,}");

		for (auto& chunk : chunks) {
			new_part[chunk[0]] = std::stoi(chunk.substr(2));
		}

		s_Parts.push_back(new_part);
	}
}

void ProcessParts() {
	std::vector<std::map<char, int>> accepted_parts;

	for (const std::map<char, int>& part : s_Parts) {
		std::string workflow = "in";

		while (true) {
			if (workflow == "A") {
				accepted_parts.push_back(part);
				break;
			} else if (workflow == "R") {
				break;
			}

			bool ruled = false;
			for (const auto& rule : s_Workflows[workflow].m_Rules) {
				int val = part.at(rule.m_Rule);
				if ( rule.m_Lt ? val < rule.m_Limit : val > rule.m_Limit) {
					ruled = true;
					workflow = rule.m_Dest;
					break;
				}
			}

			if (!ruled) {
				workflow = s_Workflows[workflow].m_ElseDest;
			}
		}
	}

	size_t total = 0;
	for (const auto& part : accepted_parts) {
		for (const auto& pair : part) {
			total += pair.second;
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
			ProcessLine(line);
		}
		file.close();
	} else {
		std::cout << "Unable to open file" << std::endl;
	}
	ProcessParts();

	return 0;
}
