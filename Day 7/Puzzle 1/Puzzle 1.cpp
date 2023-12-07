#include <iostream>
#include <fstream>
#include <algorithm>
#include "Utils.h"

enum EHandType {
	E_FIVE_KIND,
	E_FOUR_KIND,
	E_FULL_HOUSE,
	E_THREE_KIND,
	E_TWO_PAIR,
	E_ONE_PAIR,
	E_HIGH_CARD
};

struct SHandData {
	int hand[5] = { 0 };
	int bid = 0;
	EHandType type;
};

std::vector<SHandData> s_Hands;

void ProcessLine(const std::string& line) {
	std::vector<std::string> data = ParseString(line, " ");

	SHandData hand;
	hand.bid = std::stoi(data[1]);

	for (int i = 0; i < 5; ++i) {
		const char& c = data[0].at(i);
		if (std::isdigit(c)) {
			hand.hand[i] = c - '0';
		} else if (c == 'T') {
			hand.hand[i] = 10;
		} else if (c == 'J') {
			hand.hand[i] = 11;
		} else if (c == 'Q') {
			hand.hand[i] = 12;
		} else if (c == 'K') {
			hand.hand[i] = 13;
		} else if (c == 'A') {
			hand.hand[i] = 14;
		}
	}

	int sorted_hand[5];
	std::memcpy(sorted_hand, hand.hand, sizeof(int) * 5);

	std::sort(std::begin(sorted_hand), std::end(sorted_hand), std::greater<int>());

	std::vector<int> groups;

	//this is all kind of a mess but whatever.
	int last = sorted_hand[0];
	int count = 1;
	for (int i = 1; i < 5; ++i) {
		if (sorted_hand[i] == last) {
			++count;

			if (i == 4) {
				groups.push_back(count);
			}
		} else {
			if (count > 1) {
				groups.push_back(count);
			}
			count = 1;
			last = sorted_hand[i];
		}
	}

	if (groups.size() == 0) {
		hand.type = E_HIGH_CARD;
	} else if (groups.size() == 1) {
		if (groups[0] == 2) {
			hand.type = E_ONE_PAIR;
		} else if (groups[0] == 3) {
			hand.type = E_THREE_KIND;
		} else if (groups[0] == 4) {
			hand.type = E_FOUR_KIND;
		} else {
			hand.type = E_FIVE_KIND;
		}
	} else if (groups.size() == 2) {
		if ((groups[0] == 2 && groups[1] == 3) || (groups[0] == 3 && groups[1] == 2)) {
			hand.type = E_FULL_HOUSE;
		} else { 
			hand.type = E_TWO_PAIR;
		}
	}

	s_Hands.push_back(hand);
}

void CheckHands() {
	std::sort(s_Hands.begin(), s_Hands.end(), [](const SHandData& a, const SHandData& b) {
		if (a.type != b.type) {
			return a.type < b.type;
		}

		for (int i = 0; i < 5; ++i) {
			if (a.hand[i] != b.hand[i]) {
				return a.hand[i] > b.hand[i];
			}
		}

		return false;
	});

	unsigned long long total = 0;
	for (unsigned int i = 0; i < s_Hands.size(); ++i) {
		int rank = s_Hands.size() - i;
		total += rank * s_Hands[i].bid;
	}

	std::cout << "Total Winnings: " << total << std::endl;
}

int main() {
	std::ifstream file("cards.txt");
	int total = 0;

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			ProcessLine(line);
		}
		file.close();
		CheckHands();
	} else {
		std::cout << "Unable to open file" << std::endl;
	}

	return 0;
}
