#include "stdafx.h"
#include "player.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

Player::Player(string name) {
	player_name = name;
	wins = 0; losses = 0;
	chips = 20;
	hand = {};
	max_hand = {};
	bid = 0;
	folded = false;

	ifstream ifs(name+".txt");

	if (ifs.is_open()) {
		string str1;
		int val1, val2, val3;

		if (ifs >> str1 >> val1 >> val2 >> val3) {
			cout << "Name: " << str1 << endl;
			player_name = str1;

			cout << "wins: " << val1 << endl;
			wins = val2;

			cout << "losses: " << val2 << endl; 
			losses = val2;

			cout << "chips: " << val3 << endl;
			chips = val3;
			
			cout << endl;
		}
		else {
			wins = 0, losses = 0; chips = 20;
		}
	}	
}

ostream& operator<<(ostream &os, const Player &player) {
	os << player.player_name << ' ' << player.wins << ' ' << player.losses << ' ' << player.chips;
	return os;
}