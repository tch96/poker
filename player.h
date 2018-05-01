#pragma once
#include <string>
#include "hand.h"


struct Player {
	string player_name;
	Hand hand;
	Hand max_hand;
	unsigned int wins, losses, chips;

	unsigned int bid;
	bool folded;


	Player(string name);
};

ostream& operator<<(ostream &os, const Player &player);