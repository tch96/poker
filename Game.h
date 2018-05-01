#pragma once
#include "deck.h"
#include "player.h"
#include <memory>
#include <vector>
#include <string>

using namespace std;

enum betStatus {
	one_player_left, few_players_left, error
};

enum betAction {
	fold, check, call, raise
};

class Game {
public:
	~Game() {};
	static shared_ptr<Game> instance();
	static void start_game(string& s);
	static void stop_game();
	void add_player(const string &s);
	shared_ptr<Player> find_player(const string &s);
	static bool poker_rank_players(shared_ptr<Player> p1, shared_ptr<Player> p2);
	unsigned int num_players();

	//virtual int betting_phase() = 0;
	virtual int before_turn(Player& player) = 0;
	virtual int turn(Player& player) = 0;
	virtual int after_turn(Player& player) = 0;
	virtual int before_round() = 0;
	virtual int round() = 0;
	virtual int after_round() = 0;
	virtual int prebet_summarize(Player &player) = 0;
	virtual int find_best_hand(Player &player) = 0;
	virtual int compare_summarize(shared_ptr<Player> p) = 0;

	void remove_players(const string &remove_input);
	

protected:
	static shared_ptr<Game> game_sp;
	Deck main_deck;
	vector<shared_ptr<Player>> players;
	size_t dealer;
	Deck discard_deck;


	unsigned int pot;
	unsigned int current_bid;

	void collect_ante();
	betAction bet(Player &p);
	int ask_raise(Player &p);
	int betting_phase();

	int compare_players();
	int summarize();
	int reset_round();
	int check_chips();
	int add_n_remove();
};