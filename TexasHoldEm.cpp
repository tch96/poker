#include "stdafx.h"
#include "TexasHoldEm.h"
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

TexasHoldEm::TexasHoldEm() {
	dealer = 0;
	pot = 0;
	current_bid = 0;
	turn_num = 1;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 13; ++j) {
			Card card = createCard(Card::Rank(j + 2), Card::Suit(i));
			main_deck.add_card(card);
		}
	}
}

int TexasHoldEm::prebet_summarize(Player &p) {
	cout << "*** " << p.player_name << "'s turn to bet! ***" << endl;
	press_to_continue();
	cout << "Hand: " << p.hand;
	cout << "River: " << river;

	cout << endl;
	cout << "$$$ Pot: " << pot << " $$$" << endl;
	cout << "Current Bid: " << current_bid << "   ||   Your Previous Bid: " << p.bid << "   ||   Chips available: " << p.chips << endl;
	cout << endl;
	return Usage::success;
}

int TexasHoldEm::before_turn(Player &p) {
	return Usage::success;
}

int TexasHoldEm::turn(Player &p) {
	if (!p.folded) {
			// deal two cards
			p.hand << main_deck;
			p.hand << main_deck;
		}
	return Usage::success;
}

int TexasHoldEm::after_turn(Player &p) {
	return Usage::success;
}

int TexasHoldEm::before_round() {
	check_chips();

	if (players.size() < 2) {
		throw Usage::not_enough_players;
	}

	main_deck.shuffle();
	cout << endl << "Current dealer: " << (*players.at(dealer)).player_name << endl << endl;
	

	if (players.size() == 0) {
		throw Usage::not_enough_players;
	}

	collect_ante();

	for (int i = 0; i < numRounds_texas; ++i) {
		turn_num = i + 1;
		// deal cards to each player

		switch (turn_num) {
		case 1:
			for (size_t j = dealer + 1; j < dealer + 1 + players.size(); ++j) {
				Player &p = *players.at(j%players.size());
				turn(p);
			}
			break;
		case 2:
			for (int i = 0; i < 3; ++i) {
				river << main_deck;
			}		
			break;
		case 3:
			river << main_deck;
			break;
		case 4:
			river << main_deck;
			break;
		}
		

		// bet after each turn
		cout << "%%% BETTING ROUND " << turn_num << " %%%" << endl << endl;
		int betting_result = betting_phase();
		if (betting_result == betStatus::one_player_left) {
			return Usage::success;
		}
		else if (betting_result == betStatus::error) {
			throw Usage::betting_error;
		}
		// else continue
	}
	return Usage::success;
}

int TexasHoldEm::round() {
	return Usage::success;
}

Hand TexasHoldEm::best_combination(Player &p) {
	Hand seven_cards = river;
	for (int i = 0; i < p.hand.size(); ++i) {
		seven_cards << p.hand[i];
	}

	Hand max = Hand();
	int combArr[] = { 0,0,0,0,0,1,1 };
	do {
		Hand temp = Hand();
		for (int i = 0; i < seven_cards.size(); ++i) {
			if (!combArr[i]) {
				temp << seven_cards[i];
			}
		}
		// if max is empty, replace max with temp
		if (max.size() == 0) {
			max = temp;
		}
		// if temp is greater than max, replace max
		else if (poker_rank(temp, max)) {
			max = temp;
		}
	} while (next_permutation(combArr, combArr + 7));

	return max;
}

int TexasHoldEm::find_best_hand(Player &p) {
	p.max_hand = best_combination(p);
	return Usage::success;
}

int TexasHoldEm::compare_summarize(shared_ptr<Player> p) {
	cout << *p << endl;
	cout << "Hand: " << (*p).hand;
	cout << "River: " << river;
	cout << "Best Hand: " << (*p).max_hand;
	cout << endl << endl;
	return Usage::success;
}

int TexasHoldEm::after_round() {
	compare_players();
	summarize();

	reset_round();
	river = {};

	check_chips();
	add_n_remove();

	return Usage::success;
}
