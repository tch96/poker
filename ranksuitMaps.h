#pragma once
#include <unordered_map>
#include "card.h"
#include "hand.h"

using namespace std;

static unordered_map<string, Card::Rank> rankChar2Enum =
{ { "2", Card::Rank::two },{ "3", Card::Rank::three },{ "4", Card::Rank::four },{ "5", Card::Rank::five },
{ "6", Card::Rank::six },{ "7", Card::Rank::seven },{ "8", Card::Rank::eight },{ "9", Card::Rank::nine },
{ "10", Card::Rank::ten },{ "j", Card::Rank::jack },{ "J", Card::Rank::jack },{ "q", Card::Rank::queen },
{ "Q", Card::Rank::queen },{ "k", Card::Rank::king },{ "K", Card::Rank::king },{ "a", Card::Rank::ace },
{ "A", Card::Rank::ace } };

static unordered_map<char, Card::Suit> suitChar2Enum =
{ { 's', Card::Suit::spades },{ 'S', Card::Suit::spades },{ 'h', Card::Suit::hearts },{ 'H', Card::Suit::hearts },
{ 'd', Card::Suit::diamonds },{ 'D', Card::Suit::diamonds },{ 'c', Card::Suit::clubs },{ 'C', Card::Suit::clubs } };

static unordered_map<int, string> rankEnum2Char =
{ { 14, "A" },{ 2,"2" },{ 3,"3" },{ 4, "4" },{ 5, "5" },
{ 6, "6" },{ 7,"7" },{ 8,"8" },{ 9, "9" },
{ 10, "10" },{ 11,"J" },{ 12,"Q" },{ 13, "K" } };

static unordered_map<int, char> suitEnum2Char = { { 0,'C' },{ 1,'D' },{ 2, 'H' },{ 3, 'S' } };

static unordered_map<int, string> trickEnum2Str{ {Trick_Rank::flush, "flush"},
{Trick_Rank::four_kind, "four of a kind"}, {Trick_Rank::full_house, "full house"}, {Trick_Rank::one_pair, "pair"},
{Trick_Rank::straight, "straight"}, {Trick_Rank::straight_flush, "straight flush"}, {Trick_Rank::three_kind, "three of a kind"},
{Trick_Rank::two_pair, "two pair"}, {Trick_Rank::unranked, "unranked"} };

