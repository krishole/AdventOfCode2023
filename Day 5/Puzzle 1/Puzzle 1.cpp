#include <iostream>
#include <fstream>
#include <limits>
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


static std::vector<unsigned long long > s_Seeds;
static std::vector<SMapData> s_Maps;

static int s_RegionCounter = -1;

void LoadRegion(const std::vector<std::string>& lines, int region_start, int region_end) {
	if (s_RegionCounter == -1) {
		s_Seeds = ParseNumbersFromString(ParseString(lines[region_start], ":")[1]);
	} else {
		SMapData data;
		data.m_Type = static_cast<EMapType>(s_RegionCounter);

		for (int i = region_start + 1; i <= region_end; ++i) {
			const std::vector<unsigned long long > numbers = ParseNumbersFromString(lines[i]);
			data.m_DestStart = numbers[0];
			data.m_SourceStart = numbers[1];
			data.m_Range = numbers[2];
			s_Maps.push_back(data);
		}
	}

	++s_RegionCounter;
}

void ProcessLines(const std::vector<std::string>& lines) {
	int region_start = 0;
	for (int i = 0; i < static_cast<int>(lines.size()); i++) {
		if (lines[i].length() == 0 || i == lines.size() - 1) {
			LoadRegion(lines, region_start, i - 1);

			region_start = i+1;
		}
	}
}

const unsigned long long  FindDestinationForSource(unsigned long long source, EMapType type) {
	for (const SMapData& map : s_Maps) {
		if (map.m_Type == static_cast<EMapType>(type)) {
			if (source >= map.m_SourceStart && source <= map.m_SourceStart + map.m_Range) {
				unsigned long long offset = source - map.m_SourceStart;
				unsigned long long source = map.m_DestStart + offset;
				return source;
			}
		}
	}
	return source;
}

void MapSeeds() {
	unsigned long long  lowest = std::numeric_limits<unsigned long long>::max();
	for (unsigned long long  seed : s_Seeds) {
		unsigned long long  source = seed;
		for (int map_type = 0; map_type < static_cast<int>(EMapType::E_MAX); ++map_type) {
			source = FindDestinationForSource(source, static_cast<EMapType>(map_type));
		}

		if (source < lowest) {
			lowest = source;
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
	} else {
		std::cout << "Unable to open file" << std::endl;
	}

	return 0;
}
