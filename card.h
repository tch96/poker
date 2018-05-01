#pragma once
#include <iostream>

using namespace std;

struct Card {
	enum Rank { two=2, three=3, four=4, five=5, six=6, seven=7,eight=8, nine=9, ten=10, jack=11, queen=12, king=13, ace=14 } rank;
	enum Suit { clubs, diamonds, hearts, spades } suit;
	bool face_up = true;

	const bool operator<(const Card &card2)  {
		/*if (rank < card2.rank) { return 1; }
		else if (rank == card2.rank) { 
			if (suit < card2.suit) { return 1; }
			else { return 0; }
		}
		else { return 0; }*/

		if (rank < card2.rank) { return 1; }
		else {
			return 0;
		}
	}
	const bool operator==(const Card &card2) {
		if (rank == card2.rank && suit == card2.suit) {
			return true;
		}
		else {
			return false;
		}
	}
};

Card createCard(Card::Rank rank, Card::Suit suit);
void operator<<(ostream &os, const Card &card);

