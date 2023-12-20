#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
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

int s_LowPulses = 0;
int s_HighPulses = 0;
int s_Presses = 0;

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

void PushButton() {
	//pulse, destination
	std::queue<std::tuple<bool, std::string, std::string>> queue;
	++s_Presses;
	s_LowPulses += 1 + static_cast<int>(s_Modules["broadcaster"]->m_Destinations.size());

	for (const auto& dest : s_Modules["broadcaster"]->m_Destinations) {
		queue.push(std::make_tuple(0, "broadcaster", dest));
	}

	while (!queue.empty()) {
		bool pulse;
		std::string source;
		std::string destination;
		std::tie(pulse, source, destination) = queue.front();
		queue.pop();

		if (s_Modules.find(destination) != s_Modules.end()) {
			//conjunction
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

					if (all_on) {
						++s_LowPulses;
					}
					else {
						++s_HighPulses;
					}
				}

			}
			else if (s_Modules[destination]->m_Prefix == '%') {
				SFlipFlop* mod = static_cast<SFlipFlop*>(s_Modules[destination]);

				if (!pulse) {
					mod->m_State = !mod->m_State;
					for (std::string& dest : mod->m_Destinations) {
						queue.push(std::make_tuple(mod->m_State, destination, dest));

						if (mod->m_State) {
							++s_HighPulses;
						}
						else {
							++s_LowPulses;
						}
					}
				}
			}
		}
	}
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

	for (int i = 0; i < 1000; ++i) {
		PushButton();
	}

	std::cout << "Low: " << s_LowPulses << " High: " << s_HighPulses << " Total: " << s_LowPulses * s_HighPulses << std::endl;

	return 0;
}
