#pragma once
#include "Game.h"

// number of rounds in seven card stud
const int numRounds_stud = 5;

class SevenCardStud : public Game{
public:
	SevenCardStud();

	virtual int before_turn(Player &player);
	virtual int turn(Player &player);
	virtual int after_turn(Player &player);
	virtual int before_round();
	virtual int round();
	virtual int after_round();
	virtual int prebet_summarize(Player &player);
	virtual int find_best_hand(Player &player);
	virtual int compare_summarize(shared_ptr<Player> p);

	Hand best_combination(Player &p);

protected:
	unsigned int turn_num;
};
