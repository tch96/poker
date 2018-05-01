#pragma once
#include "card.h"
#include <vector>

using namespace std;

enum Trick_Rank { unranked = 0, one_pair = 1, two_pair = 2, three_kind = 3, straight = 4, flush = 5, 
		full_house = 6, four_kind = 7, straight_flush = 8};

class Deck;

class Hand {

public:
	friend void operator<<(ostream &os, const Hand &handObj);
	friend void operator<<(Hand& handObj, Deck& deck);
	friend void operator<<(Hand &handObj, Card card);

	friend Trick_Rank calc_rank(const Hand &handObj);
	friend bool get_hand_high(const Hand &hand1, const Hand &hand2);
	friend bool compare_nonpair(const Hand &hand1, const Hand &hand2, Trick_Rank tr);
	friend bool compare_pair(const Hand &hand1, const Hand &hand2);
	friend bool poker_rank(const Hand &hand1, const Hand &hand2);

	Hand() { hand = {}; };
	Hand(const Hand &);

	void operator=(const Hand &other);
	
	const int size() const;
	
	const bool operator==(const Hand &other);

	const bool operator<(const Hand &other);

	const string as_string();

	Card operator[](size_t position);

	Card remove_card(size_t position);

	void print_facedown();

private:
	vector<Card> hand;
	Trick_Rank trick_rank;
};

void operator<<(ostream &os, const Hand &handObj);
void operator<<(Hand &handObj, Deck &deck);
void operator<<(Hand &handObj, Card card);

Trick_Rank calc_rank(const Hand &handObj);
bool poker_rank(const Hand &hand1, const Hand &hand2);