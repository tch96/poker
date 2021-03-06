#include "stdafx.h"
#include "utility.h"
#include "deck.h"
#include "hand.h"
#include "Game.h"
#include "FiveCardDraw.h"
#include <iostream>
#include <string>
#include <algorithm>
#include "ranksuitMaps.h"
#include "player.h"

using namespace std;

int new_game_prompt();

int play(shared_ptr<Game> game) {

	bool no_more_adds = 0;
	string player_input;
	while (!no_more_adds) {
		cout << "Add Players? Enter exact player name ('no' to continue to next round)" << endl;
		cin >> player_input;
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (player_input == "no") {
			no_more_adds = 1;
		}
		else {
			try {
				game->add_player(player_input);
				cout << "Player Joined" << endl << endl;
			}
			catch (Usage e) {
				print_error(e);
			}
		}
	}

	int status = 0;
	while (game->num_players() >= 2 && status == 0) {
		status = game->before_round();
		status = game->round();
		status = game->after_round();
	}
	game->stop_game();
	cout << "GAME END" << endl;
	new_game_prompt();
	return Usage::success;
}

int new_game_prompt() {
	bool input_valid = false;

	while (!input_valid) {
		input_valid = true;

		cout << "Play Again? Enter 'Y' or 'N'" << endl;

		string input;
		cin >> input;
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (input == "Y") {
			cout << "Which Game?" << endl;
			string game_name;
			cin >> game_name;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			try {
				Game::start_game(game_name);
			}
			catch (Usage) {
				cout << "Unknown Game. Try Again" << endl << endl;
				input_valid = false;
			}

			if (input_valid) {
				shared_ptr<Game> game = Game::instance();
				play(game);
			}
		}
		else if (input == "N") {
			cout << "Thanks for playing." << endl;
		}
		else {
			input_valid = false;
		}
	}
	return Usage::success;
}


int main(int argc, char * argv[])
{
	try {
		if (argc < min_args) {
			throw Usage::not_enough_args;
		}

		string game_input = argv[Input::game_type];
		Game::start_game(game_input);
		shared_ptr<Game> game = Game::instance();

		for (int i = 2; i < argc; i++) {
			game->add_player(argv[i]);
		}

		int status = 0;
		while (game->num_players() >= 2 && status == 0) {
			status = game->before_round();
			status = game->round();
			status = game->after_round();
		}
		game->stop_game();
		cout << "GAME END" << endl;
		new_game_prompt();

	}
	catch (Usage e) {
		// check if game is in play
		try {
			Game::stop_game();
		}
		catch (Usage) {}

		print_error(e);
		return e;
	}
	catch (exception &e) {
		cout << e.what() << endl;
	}

	cout << "success";
	return Usage::success;
}

