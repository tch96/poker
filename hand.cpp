#include "stdafx.h"
#include "hand.h"
#include "deck.h"
#include "ranksuitMaps.h"
#include "utility.h"
#include "player.h"
#include <unordered_map>
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>


using namespace std;

Hand::Hand(const Hand &copy) {
	hand = copy.hand;
}

void Hand::operator=(const Hand &other) {
	if (this != &other) {
		hand = {};
		hand = other.hand;
	}
}


Card Hand::operator[](size_t position) {
	if (position >= hand.size()) {
		throw Usage::index_out_of_range;
	}
	else {
		return hand.at(position);
	}
}

Card Hand::remove_card(size_t position) {
	if (position >= hand.size()) {
		throw Usage::index_out_of_range;
	}
	else {
		Card card = hand.at(position);
		hand.erase(hand.begin() + position);
		return card;
	}
}

const bool Hand::operator==(const Hand &other) {
	if (hand.size() != other.hand.size()) {
		return false;
	}
	else {
		for (unsigned int i = 0; i < hand.size(); ++i) {
			if (!(hand.at(i) == other.hand.at(i))) {
				return false;
			}
		}
		return true;
	}
}

const int Hand::size() const {
	return hand.size();
}

const bool Hand::operator<(const Hand &other) {
	unsigned int i = 0;
	while (i < hand.size() && i < other.hand.size()) {
		if (hand.at(i) < other.hand.at(i)) {
			return true;
		}
		else if (hand.at(i) == other.hand.at(i)) {
			++i;
		}
		else {
			return false;
		}
	}

	return (hand.size() < other.hand.size());
}

const string Hand::as_string() {
	string final_string = "";
	for (vector<Card>::iterator it = hand.begin(); it != hand.end(); ++it) {
		Card card = *it;
		unordered_map<int, char>::const_iterator suitIt = suitEnum2Char.find(card.suit);
		unordered_map<int, string>::const_iterator rankIt = rankEnum2Char.find(card.rank);
		string as_string = rankIt->second + suitIt->second + ' ';
		final_string += as_string;
	}
	return final_string;
}

void operator<<(ostream &os, const Hand &handObj) {
	for (vector<Card>::const_iterator it = handObj.hand.begin(); it != handObj.hand.end(); ++it) {
		Card card = *it;
		unordered_map<int, char>::const_iterator suitIt = suitEnum2Char.find(card.suit);
		unordered_map<int, string>::const_iterator rankIt = rankEnum2Char.find(card.rank);
		os << rankIt->second << suitIt->second << ' ';
	}
	os << endl;
}

void operator<<(Hand &handObj, Card card) {
	handObj.hand.push_back(card);
	sort(handObj.hand.begin(), handObj.hand.end());
}

void Hand::print_facedown() {
	int num_facedowns = 0;
	for (vector<Card>::const_iterator it = hand.begin(); it != hand.end(); ++it) {
		Card card = *it;
		if (card.face_up) {
			unordered_map<int, char>::const_iterator suitIt = suitEnum2Char.find(card.suit);
			unordered_map<int, string>::const_iterator rankIt = rankEnum2Char.find(card.rank);
			cout << rankIt->second << suitIt->second << ' ';
		}
		else {
			num_facedowns++;
		}
	}
	for (int i = 0; i < num_facedowns; ++i) {
		cout << "* ";
	}
	cout << endl;
}

void operator<<(Hand &handObj, Deck &deck) {
	Card card = deck.cards.back();
	handObj.hand.push_back(card);
	deck.cards.pop_back();
	sort(handObj.hand.begin(), handObj.hand.end());
}

Trick_Rank calc_rank(const Hand &handObj) {
	// bins to store how many of each rank and suit
	int rankCount[13] = { 0 };
	int suitCount[4] = { 0 };

	for (Card a : handObj.hand) {
		rankCount[a.rank - 2]++;
		suitCount[a.suit]++;
	}

	// check four of a kind
	int *ptr = find(begin(rankCount), end(rankCount), 4);
	if (ptr != end(rankCount)) {
		return Trick_Rank::four_kind;
	}

	// check three of a kind & full house
	ptr = find(begin(rankCount), end(rankCount), 3);
	if (ptr != end(rankCount)) {
		int *ptr2 = find(begin(rankCount), end(rankCount), 2);
		if (ptr2 != end(rankCount)) {
			return Trick_Rank::full_house;
		}
		else {
			return Trick_Rank::three_kind;
		}
	}

	// check pair & two pair
	ptr = find(begin(rankCount), end(rankCount), 2);
	if (ptr != end(rankCount)) {
		int position = distance(rankCount, ptr);
		int *ptr2 = find(begin(rankCount) + position + 1, end(rankCount), 2);
		if (ptr2 != end(rankCount)) {
			return Trick_Rank::two_pair;
		}
		else {
			return Trick_Rank::one_pair;
		}
	}

	// check flush and set flush boolean
	bool is_flush = 0;
	ptr = find(begin(suitCount), end(suitCount), 5);
	if (ptr != end(suitCount)) {
		is_flush = 1;
	}

	// check straight
	ptr = find(begin(rankCount), end(rankCount), 1);
	if (ptr != end(suitCount)) {
		int position = distance(rankCount, ptr);
		if (rankCount[position + 1] && rankCount[position + 2] && rankCount[position + 3] && rankCount[position + 4]) {
			if (is_flush) {
				return Trick_Rank::straight_flush;
			}
			else {
				return Trick_Rank::straight;
			}
		}
	}

	// finally check flush again
	if (is_flush) {
		return Trick_Rank::flush;
	}

	// last: no poker rank
	return Trick_Rank::unranked;
}

// compare highest card in each hand; return true if hand1 is higher than hand2; otherwise, return false
bool get_hand_high(const Hand &hand1, const Hand &hand2){
	Hand hand1copy = hand1; Hand hand2copy = hand2;


	for (int i = 0; i < hand1.size(); ++i) {

		Card hand1high = hand1copy.hand.back();
		Card hand2high = hand2copy.hand.back();
		if (hand1high == hand2high) {
			hand1copy.hand.pop_back();
			hand2copy.hand.pop_back();
		}
		else {
			return !(hand1high < hand2high);
		}
	}
	return false;
}

// compares non-pair tricks; return true if hand1 is higher than hand2; otherwise, return false
bool compare_nonpair(const Hand &hand1, const Hand &hand2, Trick_Rank tr) {
	int hand1_triple = 0, hand2_triple = 0;
	vector<int> hand1_singles, hand2_singles;
	int hand1_high = 0, hand2_high = 0;


	int iterated_rank = 0;
	int i = 0;

	while (i < hand1.size()) {
		iterated_rank = hand1.hand.at(i).rank;
		if ((iterated_rank == hand1.hand.at(i + 1).rank) && (iterated_rank == hand1.hand.at(i + 2).rank)) {
			hand1_triple = iterated_rank;
			break;
		}
		++i;
	}

	i = 0;
	while (i < hand2.size()) {
		iterated_rank = hand2.hand.at(i).rank;
		if ((iterated_rank == hand2.hand.at(i + 1).rank) && (iterated_rank == hand2.hand.at(i + 2).rank)) {
			hand2_triple = iterated_rank;
			break;
		}
		++i;
	}

	if (tr == Trick_Rank::full_house) {
		int hand1_pair = 0, hand2_pair = 0;

		// check if triples are not equal. If yes, then return higher triple, if no, then compare pair
		if (hand1_triple != hand2_triple) {
			return (hand1_triple > hand2_triple);
		}
		else {
			i = 0;
			while (i < hand1.size()) {
				iterated_rank = hand1.hand.at(i).rank;
				if (iterated_rank != hand1_triple) {
					hand1_pair = iterated_rank;
					break;
				}
				++i;
			}
			i = 0;
			while (i < hand2.size()) {
				iterated_rank = hand2.hand.at(i).rank;
				if (iterated_rank != hand2_triple) {
					hand2_pair = iterated_rank;
					break;
				}
				++i;
			}
			return (hand1_pair > hand2_pair);
		}
	}
	// Not a full house, so four of a kind or three of a kind
	else {

		// if triples or fours are not equal, return whichever is higher
		if (hand1_triple != hand2_triple) {
			return (hand1_triple > hand2_triple);
		}
		//else search for highest non-triple/four card by calling get-hand_high function
		else {
			for (int j = 0; j < hand1.size(); ++j) {
				iterated_rank = hand1.hand.at(j).rank;
				if (iterated_rank != hand1_triple) {
					hand1_singles.push_back(iterated_rank);
				}
			}
			for (int j = 0; j < hand2.size(); ++j) {
				iterated_rank = hand2.hand.at(j).rank;
				if (iterated_rank != hand2_triple) {
					hand2_singles.push_back(iterated_rank);

				}
			}
			hand1_high = hand1_singles.back();
			hand2_high = hand2_singles.back();

			while (hand1_high == hand2_high) {
				hand1_singles.pop_back();
				hand2_singles.pop_back();
				hand1_high = hand1_singles.back();
				hand2_high = hand2_singles.back();
			}

			return (hand1_high > hand2_high);
		}
	}
}

// removes all pairs from the hand, and compare highest card in hand
bool compare_pair(const Hand &hand1, const Hand &hand2) {
	int hand1_pair1 = 0, hand2_pair1 = 0;
	int hand1_pair2 = 0, hand2_pair2 = 0;
	vector<int> hand1_singles, hand2_singles;
	int hand1_high, hand2_high;

	int iterated_rank;

	int i = 0;
	while (i < hand1.size()) {
		iterated_rank = hand1.hand.at(i).rank;
		if (i < hand1.size() - 1) {
			if (iterated_rank == hand1.hand.at(i + 1).rank) {
				if (hand1_pair1 == 0) {
					hand1_pair1 = iterated_rank;
				}
				else if (hand1_pair2 == 0) {
					hand1_pair2 = iterated_rank;
				}
				i += 2;
			}
			else {
				hand1_singles.push_back(iterated_rank);
				i++;
			}
		}
		else {
			if (iterated_rank != hand1.hand.at(i - 1).rank) {
				hand1_singles.push_back(iterated_rank);
				i++;
			}
		}
	}

	i = 0;
	while (i < hand2.size()) {
		iterated_rank = hand2.hand.at(i).rank;
		if (i < hand2.size() - 1) {
			if (iterated_rank == hand2.hand.at(i + 1).rank) {
				if (hand2_pair1 == 0) {
					hand2_pair1 = iterated_rank;
				}
				else if (hand2_pair2 == 0) {
					hand2_pair2 = iterated_rank;
				}
				i += 2;
			}
			else {
				hand2_singles.push_back(iterated_rank);
				i++;
			}
		}
		else {
			if (iterated_rank != hand2.hand.at(i - 1).rank) {
				hand2_singles.push_back(iterated_rank);
				i++;
			}
		}
	}

	bool result;
	if (hand1_pair2 != hand2_pair2) {
		result = (hand1_pair2 > hand2_pair2);
	}
	else if (hand1_pair1 != hand1_pair1) {
		result = (hand1_pair1 > hand2_pair1);
	}
	else {
		hand1_high = hand1_singles.back();
		hand2_high = hand2_singles.back();
		hand1_singles.pop_back();
		hand2_singles.pop_back();


		while ((hand1_high == hand2_high) && !hand1_singles.empty()) {
			hand1_high = hand1_singles.back();
			hand2_high = hand2_singles.back();
			hand1_singles.pop_back();
			hand2_singles.pop_back();
		}

		result = (hand1_high > hand2_high);
	}
	return result;
}

bool poker_rank(const Hand& hand1, const Hand& hand2) {
	if ((hand1.size() != poker_hand_size) || (hand2.size() != poker_hand_size)) {
		throw Usage::incorrect_hand_size;
		return false;
	}

	Trick_Rank hand1_rank = calc_rank(hand1);
	Trick_Rank hand2_rank = calc_rank(hand2);


	if (hand1_rank > hand2_rank) {
		return true;
	}
	else if (hand1_rank < hand2_rank) {
		return false;
	}
	else {

		switch (hand1_rank) {
			
			case Trick_Rank::straight_flush: {
				return get_hand_high(hand1, hand2);
			}

			case Trick_Rank::four_kind: {
				return compare_nonpair(hand1, hand2, hand1_rank);
			}
				
			case Trick_Rank::full_house: {
				return compare_nonpair(hand1, hand2, hand1_rank);
			}

			case Trick_Rank::flush:
				return get_hand_high(hand1, hand2);

			case Trick_Rank::straight:
				return get_hand_high(hand1, hand2);

			case Trick_Rank::three_kind: {
				return compare_nonpair(hand1, hand2, hand1_rank);
			}

			case Trick_Rank::two_pair: {
				return compare_pair(hand1, hand2);
			}

			case Trick_Rank::one_pair: {
				return compare_pair(hand1, hand2);
			}

			case Trick_Rank::unranked:
				return get_hand_high(hand1, hand2);
		}
	}
	return false;
}