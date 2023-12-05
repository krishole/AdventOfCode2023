#include <iostream>
#include <fstream>
#include <limits>
#include <thread>
#include <mutex>
#include "Utils.h"

enum class EMapType {
	E_SEED_TO_SOIL,
	E_SOIL_TO_FERTILIZER,
	E_FERTILIZER_TO_WATER,
	E_WATER_TO_LIGHT,
	E_LIGHT_TO_TEMPERATURE,
	E_TEMPERATURE_TO_HUMIDITY,
	E_HUMIDITY_TO_LOCATION,
	E_MAX
};

struct SMapData {
	EMapType m_Type;
	unsigned long long m_DestStart = 0;
	unsigned long long m_SourceStart = 0;
	unsigned long long m_Range = 0;
};

struct SSeedData {
	unsigned long long m_Start = 0;
	unsigned long long m_Range = 0;
};


static std::vector<SSeedData> s_Seeds;
static std::vector<SMapData> s_Maps;

static int s_RegionCounter = -1;
static std::mutex s_CoutMutex;

void LoadRegion(const std::vector<std::string>& lines, int region_start, int region_end) {
	if (s_RegionCounter == -1) {
		std::vector<unsigned long long> seeds = ParseNumbersFromString(ParseString(lines[region_start], ":")[1]);
		for (int i = 0; i < seeds.size(); i += 2) {
			s_Seeds.push_back({ seeds[i], seeds[i + 1] });
		}
	}
	else {
		EMapType type = static_cast<EMapType>(s_RegionCounter);

		for (int i = region_start + 1; i <= region_end; ++i) {
			const std::vector<unsigned long long > numbers = ParseNumbersFromString(lines[i]);
			s_Maps.push_back({ type, numbers[0], numbers[1], numbers[2] });
		}
	}

	++s_RegionCounter;
}

void ProcessLines(const std::vector<std::string>& lines) {
	int region_start = 0;
	for (int i = 0; i < static_cast<int>(lines.size()); i++) {
		if (lines[i].length() == 0 || i == lines.size() - 1) {
			LoadRegion(lines, region_start, i - 1);

			region_start = i + 1;
		}
	}
}

const unsigned long long  FindDestinationForSource(unsigned long long source, EMapType type) {
	for (const SMapData& map : s_Maps) {
		if (map.m_Type == static_cast<EMapType>(type)) {
			if (source >= map.m_SourceStart && source < map.m_SourceStart + map.m_Range) {
				return map.m_DestStart + (source - map.m_SourceStart);
			}
		}
	}
	return source;
}

void FindLowestLocationForSeedRange(const SSeedData& seed, unsigned long long& result) {
	unsigned long long  lowest = std::numeric_limits<unsigned long long>::max();
	
	{
		std::lock_guard<std::mutex> lock(s_CoutMutex);
		std::cout << "Thread started, seed range: " << seed.m_Start << " " << seed.m_Range << std::endl;
	}
	
	for (unsigned long long seed_num = seed.m_Start; seed_num < seed.m_Start + seed.m_Range; ++seed_num) {
		unsigned long long  source = seed_num;
		for (int map_type = 0; map_type < static_cast<int>(EMapType::E_MAX); ++map_type) {
			source = FindDestinationForSource(source, static_cast<EMapType>(map_type));
		}

		if (source < lowest) {
			lowest = source;
		}
	}

	{
		std::lock_guard<std::mutex> lock(s_CoutMutex);
		std::cout << "Thread Finished, seed range: " << seed.m_Start << " " << seed.m_Range << " Result: " << lowest << std::endl;
	}

	result = lowest;
}

void MapSeeds() {
	std::vector<std::thread> threads;
	std::vector<unsigned long long> results;

	results.resize(s_Seeds.size());

	for (int i = 0; i < s_Seeds.size(); ++i) {
		threads.emplace_back(FindLowestLocationForSeedRange, std::ref(s_Seeds[i]), std::ref(results[i]));
	}

	for (std::thread& t : threads) {
		t.join();
	}

	unsigned long long  lowest = std::numeric_limits<unsigned long long>::max();
	for (int i = 0; i < results.size(); i++) {
		if (results[i] < lowest) {
			lowest = results[i];
		}
	}

	std::cout << "Lowest Destination: " << lowest << std::endl;
}

int main() {
	std::ifstream file("map.txt");

	if (file.is_open()) {
		std::vector<std::string> lines;
		std::string line;
		while (std::getline(file, line)) {
			lines.push_back(line);
		}
		file.close();

		ProcessLines(lines);
		MapSeeds();
	}
	else {
		std::cout << "Unable to open file" << std::endl;
	}

	return 0;
}
