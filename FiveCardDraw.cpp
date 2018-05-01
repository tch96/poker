#include "stdafx.h"
#include "FiveCardDraw.h"
#include "card.h"
#include "utility.h"
#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

const int hand_size = 5;

FiveCardDraw::FiveCardDraw() {
	dealer = 0;
	pot = 0;
	current_bid = 0;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 13; ++j) {
			Card card = createCard(Card::Rank(j + 2), Card::Suit(i));
			main_deck.add_card(card);
		}
	}
}

int FiveCardDraw::before_turn(Player &player) {
	cout << "*** " << player.player_name << "'s turn! ***" << endl;
	cout << "Hand: " << player.hand;
	cout << endl;

	vector<int> discard_indices = {};
	bool valid = false;

	// check if user input is valid, otherwise, reset discard_indices vector & ask for input again
	while (!valid) {
		valid = true;
		discard_indices = {};

		cout << "What cards do you want to discard? Enter space-delimited indices [1,5], Sample Input: 1 4 5" << endl;
		cout << "Enter 'none' to not discard" << endl;

		string input;
		getline(cin >> ws, input);
		
		if (input == "none") {
			//do nothing
		}
		else {
			int numRemoved = 0;
			istringstream iss(input);
			string c_index;
			while (iss >> c_index) {
				char *p;
				strtol(c_index.c_str(), &p, 10);
				if (*p != 0) {
					cout << "Invalid Input. Enter proper int values as indices" << endl;
					valid = 0;
					break;
				}

				if (stoi(c_index) < 0 || stoi(c_index) >= hand_size || numRemoved > hand_size) {
					cout << "Invalid Input. Enter at most " << hand_size << " index values between [0,4]" << endl;
					valid = 0;
					break;
				}
				else {
					discard_indices.push_back(stoi(c_index));
					++numRemoved;
				}
			}
		}
	}


	// discard cards if vector has some values
	if (discard_indices.size() > 0) {
		for (size_t i = 0; i < discard_indices.size(); ++i) {
			int index = discard_indices.at(i) - i;
			Card discarded_card = player.hand.remove_card(index);
			discard_deck.add_card(discarded_card);
		}
	}
	
	cout << endl << endl;
	return Usage::success;
}

int FiveCardDraw::turn(Player &player) {
	int numCards = player.hand.size();
	int numCards2Deal = hand_size - numCards;

	if (main_deck.size() + discard_deck.size() < numCards2Deal) {
		return Usage::no_cards_in_deck;
	}

	for (int i = 0; i < numCards2Deal; ++i) {
		if (main_deck.size() == 0) { 
			main_deck = discard_deck;
			discard_deck.clear();
		}
		player.hand << main_deck;
	}
	return Usage::success;
}

int FiveCardDraw::after_turn(Player &player) {
	cout << "Player: " << player.player_name << endl;
	cout << "New Hand: " << player.hand;
	cout << endl;
	return Usage::success;
}

int FiveCardDraw::prebet_summarize(Player &p) {
	cout << "*** " << p.player_name << "'s turn to bet! ***" << endl;
	cout << "Hand: " << p.hand;
	cout << endl;
	cout << "$$$ Pot: " << pot << " $$$" << endl;
	cout << "Current Bid: " << current_bid << "   ||   Your Previous Bid: " << p.bid << "   ||   Chips available: " << p.chips << endl;
	cout << endl;
	return Usage::success;
}

int FiveCardDraw::before_round() {
	check_chips();

	if (players.size() < 2) {
		throw Usage::not_enough_players;
	}

	main_deck.shuffle();
	cout << endl << "Current dealer: " << (*players.at(dealer)).player_name << endl << endl;
	
	if (players.size() == 0) {
		throw Usage::not_enough_players;
	}


	for (int i = 0; i < hand_size; ++i) {
		for (size_t j = dealer + 1; j < dealer + 1 + players.size(); ++j) {
			(*players.at(j%players.size())).hand << main_deck;
		}
	}

	collect_ante();

	cout << "%%% BETTING PHASE 1 %%%" << endl << endl;
	int betting_result = betting_phase();
	if (betting_result == betStatus::one_player_left) {
		return Usage::success;
	}
	else if (betting_result == betStatus::error) {
		throw Usage::betting_error;
	}
	else {
		for (size_t i = dealer + 1; i < dealer + 1 + players.size(); ++i) {
			Player &p = *players.at(i%players.size());
			if (!p.folded) {
				before_turn(p);
				clear_screen();
			}
		}
	}
	return Usage::success;
}

int FiveCardDraw::round() {
	int result = 0;
	if (players.size() == 0) {
		throw Usage::not_enough_players;
	}

	int numAlive = players.size();
	for (size_t i = 0; i < players.size(); ++i) {
		if ((*players.at(i%players.size())).folded) {
			numAlive--;
		}
	}

	if (numAlive == 1) {
		return Usage::success;
	}
	else {
		for (size_t i = dealer + 1; i < dealer + 1 + players.size(); ++i) {
			Player &p = *players.at(i%players.size());
			result = turn(p);
			if (result) {
				return result;
			}
			/*cout << "*** NEW HANDS ***" << endl;
			after_turn(p);*/
		}

		cout << "%%% BETTING PHASE 2 %%%" << endl << endl;
		betting_phase();
	}

	return Usage::success;
}

int FiveCardDraw::find_best_hand(Player &p) {
		p.max_hand = p.hand;
		return Usage::success;
}

int FiveCardDraw::compare_summarize(shared_ptr<Player> p) {
	cout << *p << endl;
	cout << "Hand: " << (*p).hand;
	cout << "Best Hand: " << (*p).max_hand;
	cout << endl << endl;
	return Usage::success;
}

int FiveCardDraw::after_round() {
	compare_players();
	summarize();

	reset_round();

	check_chips();
	add_n_remove();

	return Usage::success;
}
