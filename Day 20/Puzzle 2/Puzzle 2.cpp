#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <numeric>
#include "Utils.h"

struct SModule {
	char m_Prefix;
	std::vector<std::string> m_Destinations;
};

struct SFlipFlop : SModule {
	bool m_State = false;
};

struct SConjunction : SModule {
	std::unordered_map<SFlipFlop*, bool> m_Callers;
};

std::unordered_map<std::string, SModule*> s_Modules;

std::unordered_map<std::string, int> s_RmCallers;

int s_Presses = 0;
bool s_RxPressed = false;

void ProcessLine(const std::string& line) {
	SModule* new_module = nullptr;
	std::string id;

	if (line[0] == '%') {
		new_module = new SFlipFlop;
		new_module->m_Prefix = '%';
		id = ParseString(line, " ")[0].substr(1);
	} else if (line[0] == '&') {
		new_module = new SConjunction;
		new_module->m_Prefix = '&';
		id = ParseString(line, " ")[0].substr(1);
	} else {
		new_module = new SModule;
		new_module->m_Prefix = 'B';
		id = "broadcaster";
	}

	auto chunks = ParseString(line, "> ,");
	for (int i = 2; i < chunks.size(); ++i) {
		new_module->m_Destinations.push_back(chunks[i]);
		if (chunks[i] == "rm") {
			s_RmCallers[id] = 0;
		}
	}

	s_Modules[id] = new_module;
}

void UpdateConModules() {
	for (std::pair<const std::string, SModule*>& pair : s_Modules) {
		if (pair.second->m_Prefix == '&') {
			SConjunction* mod = static_cast<SConjunction*>(pair.second);

			for (std::pair<const std::string, SModule*>& pair2 : s_Modules) {
				for (std::string dest : pair2.second->m_Destinations) {
					if (dest == pair.first) {
						SFlipFlop* caller = static_cast<SFlipFlop*>(pair2.second);
						mod->m_Callers[caller] = false;
					}
				}
			}
		}
	}
}

bool PushButton() {
	//pulse, destination
	std::queue<std::tuple<bool, std::string, std::string>> queue;
	++s_Presses;

	for (const auto& dest : s_Modules["broadcaster"]->m_Destinations) {
		queue.push(std::make_tuple(0, "broadcaster", dest));
	}

	while (!queue.empty()) {
		bool pulse;
		std::string source;
		std::string destination;
		std::tie(pulse, source, destination) = queue.front();
		queue.pop();

		if (destination == "rx" && !pulse) {
			s_RxPressed = true;
			return true;
		}

		if (s_Modules.find(destination) != s_Modules.end()) {
			if (s_Modules[destination]->m_Prefix == '&') {
				SConjunction* mod = static_cast<SConjunction*>(s_Modules[destination]);

				SFlipFlop* caller = static_cast<SFlipFlop*>(s_Modules[source]);
				mod->m_Callers[caller] = pulse;

				bool all_on = true;
				for (const std::pair<SFlipFlop*, bool>& call : mod->m_Callers) {
					if (!call.second) {
						all_on = false;
						break;
					}
				}

				for (std::string& dest : mod->m_Destinations) {
					queue.push(std::make_tuple(!all_on, destination, dest));
					if (dest == "rm" && !all_on) {
						s_RmCallers[destination] = s_Presses;

						bool has_all = true;
						for (auto& c : s_RmCallers) {
							if (c.second == 0) {
								has_all = false;
							}
						}

						if (has_all) {
							return true;
						}
					}
				}

			} else if (s_Modules[destination]->m_Prefix == '%') {
				SFlipFlop* mod = static_cast<SFlipFlop*>(s_Modules[destination]);

				if (!pulse) {
					mod->m_State = !mod->m_State;
					for (std::string& dest : mod->m_Destinations) {
						queue.push(std::make_tuple(mod->m_State, destination, dest));
					}
				}
			}
		}
	}
	return false;
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

	UpdateConModules();

	while(!s_RxPressed) {
		if (PushButton()) {
			break;
		}
	}

	//dp, dh, qd, bb
	unsigned long long output = s_RmCallers["dp"];
	output = std::lcm(output, s_RmCallers["dh"]);
	output = std::lcm(output, s_RmCallers["qd"]);
	output = std::lcm(output, s_RmCallers["bb"]);

	std::cout << "Button presses: " << output << std::endl;

	return 0;
}
