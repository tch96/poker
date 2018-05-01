#pragma once
#include "Game.h"
#include <memory>


class FiveCardDraw : public Game {
public:
	FiveCardDraw();
	virtual int before_turn(Player &player);

	virtual int turn(Player &player);
	virtual int after_turn(Player &player);
	virtual int before_round();
	virtual int round();
	virtual int after_round();
	virtual int prebet_summarize(Player &player);
	virtual int find_best_hand(Player &player);
	virtual int compare_summarize(shared_ptr<Player> p);

};