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
std::map<char, std::pair<int, int>> s_Ranges = { { 'x', {1, 4000}},{ 'm', {1, 4000}} ,{ 'a', {1, 4000}} ,{ 's', {1, 4000}} };
unsigned long long s_Total = 0;

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

				size_t spl = op.find(':');
				new_rule.m_Limit = std::stoi(op.substr(2, spl - 2));
				new_rule.m_Dest = op.substr(spl + 1);

				new_op.m_Rules.push_back(new_rule);
			}
			else {
				new_op.m_ElseDest = op;
			}
		}

		s_Workflows[chunks[0]] = new_op;
	}
}

unsigned long long RangeTotal(std::map<char, std::pair<int, int>>& ranges) {
	unsigned long long x = ranges['x'].second - ranges['x'].first + 1;
	unsigned long long m = ranges['m'].second - ranges['m'].first + 1;
	unsigned long long a = ranges['a'].second - ranges['a'].first + 1;
	unsigned long long s = ranges['s'].second - ranges['s'].first + 1;
	return x * m * a * s;
}

void RunRule(const SRule& rule, std::map<char, std::pair<int, int>>& success_range, std::map<char, std::pair<int, int>>& fail_range) {
	std::pair<int, int>& range = success_range[rule.m_Rule];
	std::pair<int, int>& frange = fail_range[rule.m_Rule];

	if (rule.m_Lt) {
		if (range.second >= rule.m_Limit) {
			range.second = rule.m_Limit - 1;
		}
		if (frange.first < rule.m_Limit) {
			frange.first = rule.m_Limit;
		}
	}
	else {
		if (range.first <= rule.m_Limit + 1) {
			range.first = rule.m_Limit + 1;
		}
		if (frange.second > rule.m_Limit) {
			frange.second = rule.m_Limit;
		}
	}
}

void CheckWorkflow(std::string workflow, std::map<char, std::pair<int, int>>& ranges) {
	if (workflow == "A") {
		s_Total += RangeTotal(ranges);
		return;
	} else if (workflow == "R") {
		return;
	}
	
	std::map<char, std::pair<int, int>> fail_range = ranges;
	for (const auto& rule : s_Workflows[workflow].m_Rules) {
		std::map<char, std::pair<int, int>> pass_range = fail_range;
		RunRule(rule, pass_range, fail_range);
		CheckWorkflow(rule.m_Dest, pass_range);
	}

	CheckWorkflow(s_Workflows[workflow].m_ElseDest, fail_range);
}

void ProcessParts() {
	std::string workflow = "in";
	CheckWorkflow(workflow, s_Ranges);

	std::cout << "Total: " << s_Total << std::endl;
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
	}
	else {
		std::cout << "Unable to open file" << std::endl;
	}
	ProcessParts();

	return 0;
}
