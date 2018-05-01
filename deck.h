#pragma once
#include "card.h"
#include "ranksuitMaps.h"
#include "hand.h"
#include "utility.h"
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class Deck {

public:
	friend void operator<<(ostream &os, const Deck &deck);
	friend void operator<<(Hand&, Deck&);
	Deck() { cards = {}; };
	Deck(string & filename);
	void add_card(Card card);
	void load(string & filename);
	void shuffle();
	Card pop();
	void clear();
	const int size() { return cards.size(); };

private:
	vector<Card> cards;
};

void operator<<(ostream &os, const Deck &deck);