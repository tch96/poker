#include "stdafx.h"
#include <vector>
#include "card.h"
#include "ranksuitMaps.h"
#include "utility.h"
#include "deck.h"
#include <fstream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <iostream>
#include <ctime>

using namespace std;

Deck::Deck(string & filename) {
	try {
		Deck::load(filename);
	}
	catch (Usage e) {
		throw e;
	}
	catch (...) {
		throw Usage::unknown_error;
	}
}

void Deck::add_card(Card card) {
	cards.push_back(card);
}

void Deck::load(string & filename) {
	ifstream ifs(filename);

	if (ifs.is_open()) {
		string s;
		while (ifs >> s) {
			int strlen = s.length();
			if (strlen == 2 || strlen == 3) {
				char suit = s.at(strlen - 1);
				string rank = s.substr(0, strlen - 1);

				unordered_map<char, Card::Suit>::const_iterator suitIt = suitChar2Enum.find(suit);
				unordered_map<string, Card::Rank>::const_iterator rankIt = rankChar2Enum.find(rank);

				if (suitIt != suitChar2Enum.end() && rankIt != rankChar2Enum.end()) {
					Card card = createCard(rankIt->second, suitIt->second);
					cards.push_back(card);
				}
			}
		}
	}
	else {
		throw Usage::failed_to_open_file;
	}
}
void Deck::shuffle() {
	cout << "shuffle" << endl;
	srand(unsigned(time(NULL)));
	random_shuffle(cards.begin(), cards.end());
}

Card Deck::pop() {
	Card back = cards.back();
	cards.pop_back();
	return back;
}

void Deck::clear() {
	cards = {};
}

void operator<<(ostream &os, const Deck &deck) {
	os << "Deck:" << endl;
	for (vector<Card>::const_iterator it = deck.cards.begin(); it != deck.cards.end(); ++it) {
		Card card = *it;
		unordered_map<int, char>::const_iterator suitIt = suitEnum2Char.find(card.suit);
		unordered_map<int, string>::const_iterator rankIt = rankEnum2Char.find(card.rank);
		os << rankIt->second << suitIt->second << endl;
	}
}