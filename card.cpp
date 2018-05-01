#include "stdafx.h"
#include "card.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include "ranksuitMaps.h"

using namespace std;

Card createCard(Card::Rank rank, Card::Suit suit) {
	Card newCard;
	newCard.suit = suit;
	newCard.rank = rank;

	return newCard;
}

void operator<<(ostream &os, const Card &card) {
	unordered_map<int, char>::const_iterator suitIt = suitEnum2Char.find(card.suit);
	unordered_map<int, string>::const_iterator rankIt = rankEnum2Char.find(card.rank);
	os << rankIt->second << suitIt->second << ' ';
}