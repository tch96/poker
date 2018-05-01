#include "stdafx.h"
#include "Game.h"
#include "FiveCardDraw.h"
#include "SevenCardStud.h"
#include "TexasHoldEm.h"
#include "hand.h"
#include "utility.h"
#include <memory>
#include <iostream>
#include <fstream>

using namespace std;


string fiveCardDraw = "FiveCardDraw";
string sevenCardStud = "SevenCardStud";
string texasHoldEm = "TexasHoldEm";

shared_ptr<Game> Game::game_sp;

shared_ptr<Game> Game::instance() {
	if (game_sp == NULL) {
		throw Usage::instance_not_available;
	}
	else {
		shared_ptr<Game> game_sp_copy = game_sp;
		return game_sp_copy;
	}
}

void Game::start_game(string& s) {
	if (game_sp != NULL) {
		throw Usage::game_already_started;
	}
	if (s == fiveCardDraw) {
		game_sp = make_shared<FiveCardDraw>(FiveCardDraw());
	}
	else if (s == sevenCardStud) {
		game_sp = make_shared<SevenCardStud>(SevenCardStud());
	}
	else if (s == texasHoldEm) {
		game_sp = make_shared<TexasHoldEm>(TexasHoldEm());
	}
	else {
		throw Usage::unknown_game;
	}
}

void Game::stop_game() {
	//game shared pointer is singular, throw no_game exception
	if (game_sp == NULL) {
		throw Usage::no_game_in_progress;
	}
	else {
		game_sp = NULL;
	}
}

void Game::add_player(const string &s) {
	// check if player already exists
	if (find_player(s) != NULL) {
		throw Usage::already_playing;
		return;
	}
	// player doesn't exist, add player to players vector
	else {
		Player* player_p = new Player(s);
		shared_ptr<Player> player_sp = shared_ptr<Player>(player_p);
		players.push_back(player_sp);
		return;
	}
}

shared_ptr<Player> Game::find_player(const string &s) {
	for (size_t i = 0; i < players.size(); ++i){
		if ((*players[i]).player_name == s) {
			return players[i];
		}
	}
	shared_ptr<Player> sing_ptr = NULL;
	return sing_ptr;
}

bool Game::poker_rank_players(shared_ptr<Player> p1, shared_ptr<Player> p2) {
	if (p1 == NULL) {
		return false;
	}
	if (p2 == NULL) {
		return true;
	}
	if ((*p1).max_hand == (*p2).max_hand) {
		return false;
	}

	bool poker_rank_result = poker_rank((*p1).max_hand, (*p2).max_hand);
	return poker_rank_result;
}

unsigned int Game::num_players() {
	return players.size();
}

void Game::collect_ante() {
	cout << "Collecting Ante from players..." << endl << endl;
	for (size_t i = 0; i < players.size(); ++i) {
		(*players.at(i)).chips--;
		pot++;
	}
}

int Game::ask_raise(Player &player) {
	unsigned int call_margin = current_bid - player.bid;
	bool raise_valid = false;

	while (!raise_valid) {
		cout << "How much would you like to raise by 1 or 2 chips? Enter an '1' or '2'" << endl;

		string raise_input;
		cin >> raise_input;
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		// check if input is an int
		char *c;
		strtol(raise_input.c_str(), &c, 10);

		if (*c != 0) {
			cout << "Invalid input. Please enter integer inputs." << endl << endl;
		}

		// input is an int
		else {
			unsigned int raise_val = stoi(raise_input);

			if (raise_val < 1 || raise_val > 2) {
				cout << "You can either call or raise by 1 or 2 chips." << endl << endl;
			}
			else {
				if (player.chips < call_margin + raise_val) {
					cout << "You do not have enough chips to raise bid by " << raise_val << endl << endl;
				}
				else {
					player.chips -= (call_margin + raise_val);
					player.bid += (call_margin + raise_val);
					pot += (call_margin + raise_val);
					current_bid += raise_val;
					raise_valid = true;
				}
			}
		}
	}
	return Usage::success;
}


betAction Game::bet(Player &p) {
	prebet_summarize(p);

	bool valid = false;
	betAction action;

	while (!valid) {
		valid = true;
		cout << "Would you like to Fold [F], Check [K], Call [C], or Raise [R]? Enter singular characters only." << endl;
		string input;
		cin >> input;
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (input == "F") {
			cout << "Folded" << endl << endl;
			p.folded = true;
			action = fold;
		}
		else if (input == "K") {
			if (p.bid < current_bid) {
				cout << "You must either Fold, Call, or Raise." << endl << endl;
				valid = false;
			}
			else {
				cout << "Checked" << endl << endl;
				action = check;
			}
		}
		else if (input == "C") {
			unsigned int call_margin = current_bid - p.bid;
			if (call_margin == 0) {
				cout << "You must either Fold, Check, or Raise." << endl << endl;
				valid = false;
			}
			else if (p.chips < call_margin) {
				cout << "Not enough chips to make full call. All available chips have been taken." << endl << endl;
				pot += p.chips;
				p.chips = 0;
			}
			else {
				cout << "Called" << endl << endl;
				pot += call_margin;
				p.chips -= call_margin;
				p.bid += call_margin;
			}
			action = call;
		}
		else if (input == "R") {
			unsigned int call_margin = current_bid - p.bid;
			if ((p.chips < call_margin + 1)) {
				cout << "Not enough chips to raise. Try Fold, Check, or Call." << endl << endl;
				valid = false;
			}
			else {
				ask_raise(p);
				cout << "Raised" << endl << endl;
				action = raise;
			}
		}
		else {
			cout << "Invalid Input." << endl << endl;
			valid = false;
		}
	}
	return action;
}

int Game::betting_phase() {

	int numAlive;

	bool hasBet = false;
	bool round_complete = false;
	while (!round_complete) {
		numAlive = players.size();
		round_complete = true;
		for (size_t i = dealer + 1; i < dealer + 1 + players.size(); ++i) {
			Player &p = *players.at(i%players.size());
			if (p.folded) {
				numAlive--;
			}
			if (!p.folded && (p.chips > 0) && (numAlive > 1) && ((p.bid < current_bid) || ((p.bid == current_bid) && !hasBet))) {
				betAction result = bet(p);
				clear_screen();

				if (result == raise) {
					hasBet = true;
					round_complete = false;
				}
				else if (result == fold) {
					numAlive--;
				}
			}
		}
	}

	if (numAlive > 1) {
		return betStatus::few_players_left;
	}
	else if (numAlive == 1) {
		return betStatus::one_player_left;
	}
	else {
		return betStatus::error;
	}
}

int Game::compare_players() {
	vector<shared_ptr<Player>> sorted_players;

	// find all unfolded hands
	for (size_t i = 0; i < players.size(); ++i) {
		shared_ptr<Player> player_sp = players.at(i);
		if (!(*player_sp).folded) {
			sorted_players.push_back(player_sp);
		}
		// increment losses for all folded hands
		else {
			(*player_sp).losses++;
		}
	}

	size_t num_winners = 1;

	// if more than one player still alive, search for possible cowinners
	if (sorted_players.size() > 1) {
		
		for (size_t i = 0; i < sorted_players.size(); ++i) {
			Player &p = *sorted_players.at(i);
			find_best_hand(p);
		}

		sort(sorted_players.begin(), sorted_players.end(), poker_rank_players);

		bool search_cowinner = true;
		shared_ptr<Player> winner_sp = sorted_players.at(0);

		while (search_cowinner) {
			// if player at num_winners is greater than winner_sp, then we know they are equal [strict weak ordering]
			if (poker_rank_players(sorted_players.at(num_winners), winner_sp)) {
				num_winners++;
			}
			else {
				search_cowinner = false;
			}
		}
	}

	// add chips to winners & increment wins
	for (size_t i = 0; i < sorted_players.size(); ++i) {
		Player &p = *sorted_players.at(i);
		p.losses++;
		if (i < num_winners) {
			int pot_split = pot / num_winners;
			p.chips += pot_split;
			pot -= pot_split;
			p.wins++;
		}
		else {
			p.losses++;
		}
	}


	cout << "***COMPARE HANDS***" << endl << endl;

	for (size_t i = 0; i < sorted_players.size(); ++i) {
		shared_ptr<Player> p = sorted_players.at(i);
		/*cout << *p << endl;
		cout << "Hand: " << (*p).hand;
		cout << "Best Hand: " << (*p).max_hand;
		cout << endl << endl;*/
		compare_summarize(p);
	}
	return Usage::success;
}

int Game::summarize() {
	cout << "%%% SUMMARY OF ROUND %%%" << endl << endl;
	for (size_t i = 0; i < players.size();++i) {
		Player &p = *players.at(i);
		cout << p << endl;
		if (p.folded) {
			cout << "Folded" << endl << endl;
		}
		else {
			cout << "Best Hand: " << p.max_hand;
			cout << endl;
		}
	}
	return Usage::success;
}

int Game::reset_round() {
	// reset each player to no hand, bid = 0, folded = false
	for (unsigned int i = 0; i < players.size(); ++i) {
		Player & p = *players.at(i);
		int hand_size = p.hand.size();
		for (int j = 0; j < hand_size; ++j) {
			Card discard_card = p.hand.remove_card(0);
			discard_deck.add_card(discard_card);
		}
		p.bid = 0;
		p.folded = false;
	}

	int discard_deck_size = discard_deck.size();
	for (int i = 0; i < discard_deck_size; ++i) {
		main_deck.add_card(discard_deck.pop());
	}


	current_bid = 0;

	return Usage::success;
}

int Game::check_chips() {
	for (unsigned int i = 0; i < players.size(); ++i) {
		Player & p = *players.at(i);
		if (p.chips == 0) {
			cout << "Player: " << p.player_name << " is out of chips..." << endl;
			bool valid = false;

			while (!valid) {
				valid = true;
				cout << "Reset chips to 20, or Quit? Press 'R' for reset; 'Q' for quit" << endl;

				string input;
				cin >> input;
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				if (input == "R") {
					p.chips = 20;
				}
				else if (input == "Q"){
					string name = p.player_name;
					remove_players(name);
				}
				else {
					cout << "Invalid Input" << endl << endl;
					valid = false;
				}
			}
			
		}
	}
	return Usage::success;
}

void Game::remove_players(const string &input) {
	shared_ptr<Player> player = find_player(input);
	if (player != NULL) {
		string file_name = input + ".txt";
		ofstream ofs(file_name);
		ofs << *player;
		ofs.close();

		for (size_t i = 0; i < players.size(); ++i) {
			if (player == players.at(i)) {
				players.erase(players.begin() + i);
				break;
			}
		}
		cout << "Player " << input << " was removed" << endl;
	}
	else {
		cout << "Player not in game" << endl;
	}
}

int Game::add_n_remove() {
	int no_more_removal = 0;
	string user_input;

	cout << "%%% ADD or REMOVE PLAYERS %%%" << endl;
	while (!no_more_removal) {
		cout << endl << "Remove Players? Enter exact player name ('no' to continue to next round)" << endl;
		cin >> user_input;
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (user_input == "no") {
			no_more_removal = 1;
			cout << endl;
		}
		else {
			remove_players(user_input);
		}
	}

	int no_more_adds = 0;
	while (!no_more_adds) {
		cout << "Add Players? Enter exact player name ('no' to continue to next round)" << endl;
		cin >> user_input;
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (user_input == "no") {
			no_more_adds = 1;
		}
		else {
			try {
				add_player(user_input);
				cout << "Player Joined" << endl << endl;
			}
			catch (Usage e) {
				print_error(e);
			}
		}
	}

	// rotate dealer if there are still players left; otherwise, modular will cause runtime error
	if (players.size() > 0) {
		dealer = (dealer + 1) % players.size();
	}
	return Usage::success;
}