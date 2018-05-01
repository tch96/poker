#include "stdafx.h"
#include "SevenCardStud.h"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

SevenCardStud::SevenCardStud() {
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


int SevenCardStud::prebet_summarize(Player &p) {
	cout << "*** " << p.player_name << "'s turn to bet! ***" << endl;
	press_to_continue();
	cout << "Hand: " << p.hand;

	//search for current player's index
	int p_index = 0;
	while ((*players.at(p_index)).player_name != p.player_name) {
		p_index++;
	}

	// print other hands
	for (size_t i = p_index + 1; i < p_index + players.size(); ++i) {
		Player &other = *players.at(i%players.size());
		cout << other.player_name << "'s hand: ";
		if (other.folded) {
			cout << "Folded" << endl;
		}
		else {
			other.hand.print_facedown();
		}
	}

	cout << endl;
	cout << "$$$ Pot: " << pot << " $$$" << endl;
	cout << "Current Bid: " << current_bid << "   ||   Your Previous Bid: " << p.bid << "   ||   Chips available: " << p.chips << endl;
	cout << endl;	
	return Usage::success;
}

int SevenCardStud::before_turn(Player &p) {
	return Usage::success;
}

int SevenCardStud::turn(Player &p) {
	if (!p.folded) {
		switch (turn_num) {
		case 1:
			// deal two face down cards
			for (int i = 0; i < 2; ++i) {
				Card c = main_deck.pop();
				c.face_up = false;
				p.hand << c;

			}
			p.hand << main_deck;
			break;
		case 2:
			p.hand << main_deck;
			break;
		case 3:
			p.hand << main_deck;
			break;
		case 4:
			p.hand << main_deck;
			break;
		case 5:
			Card c = main_deck.pop();
			c.face_up = false;
			p.hand << c;
			break;
		}
	}
	return Usage::success;
}

int SevenCardStud::after_turn(Player &p) {
	return Usage::success;
}

int SevenCardStud::before_round() {
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

	for (int i = 0; i < numRounds_stud; ++i) {
		turn_num = i + 1;
		// deal cards to each player
		for (size_t j = dealer + 1; j < dealer + 1 + players.size(); ++j) {
			Player &p = *players.at(j%players.size());
			turn(p);
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

int SevenCardStud::round() {
	return Usage::success;
 }

Hand SevenCardStud::best_combination(Player &p) {
	Hand max = Hand();
	int combArr[] = { 0,0,0,0,0,1,1 };
	do {
		Hand temp = Hand();
		for (int i = 0; i < p.hand.size(); ++i) {
			if (!combArr[i]) {
				temp << p.hand[i];
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

	} while (next_permutation(combArr, combArr+7));
	
	return max;
}

int SevenCardStud::find_best_hand(Player &p) {
	p.max_hand = best_combination(p);
	return Usage::success;
}

int SevenCardStud::compare_summarize(shared_ptr<Player> p) {
	cout << *p << endl;
	cout << "Hand: " << (*p).hand;
	cout << "Best Hand: " << (*p).max_hand;
	cout << endl << endl;
	return Usage::success;
}

int SevenCardStud::after_round() {
	compare_players();
	summarize();

	reset_round();

	check_chips();
	add_n_remove();

	return Usage::success;
}