#pragma once
#include <set>
#include <unordered_map>
using namespace std;

const int poker_hand_size = 5;

// number of arguments accepted (including the program name)
const int min_args = 4;

enum Input {program_name = 0, game_type = 1};


enum Usage { success = 0, not_enough_args, not_enough_players, missing_file_input, more_than_one_file_input, 
	failed_to_open_file, incorrect_hand_size, no_cards_in_deck , index_out_of_range, instance_not_available, 
	game_already_started, unknown_game, no_game_in_progress, betting_error, already_playing,unknown_error = 99};

static unordered_map<Usage, string> Usage2Str{
	{Usage::success, "success"},
{Usage::not_enough_args, "Not enough arguments. Input one game name, followed by two or more players\nSample Input: Lab4.exe FiveCardDraw Ron Dolores"},
{Usage::not_enough_players, "Not enough players."},
{Usage::missing_file_input, "Missing input text file"},
{Usage::more_than_one_file_input, "Too many arguments. Program requires one input text file with a '-shuffle' option"},
{Usage::failed_to_open_file, "Failed to open input file"},
{Usage::incorrect_hand_size, "Hand is not a poker hand"},
{Usage::no_cards_in_deck, "Not enough cards in deck to deal"},
{Usage::unknown_error, "An unknown error occured"},
{Usage::index_out_of_range, "Index is out of range"},
{Usage::instance_not_available, "Instance is not available"},
{Usage::game_already_started, "Game instance already exists"},
{Usage::unknown_game, "Unknown game specified"},
{Usage::no_game_in_progress, "No game currently in progress"},
{Usage::already_playing, "Player is already in play"},
{Usage::betting_error, "Error occured during betting"},
};

int print_error(Usage &e);

void clear_screen();

void press_to_continue();

