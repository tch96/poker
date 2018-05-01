#include "stdafx.h"
#include "utility.h"
#include <iostream>
#include <string>

int print_error(Usage &e) {
	unordered_map<Usage, string>::const_iterator it = Usage2Str.find(e);
	if (it != Usage2Str.end()) {
		string error = it->second;
		cout << error << endl;
	}
	return Usage::success;
}

void clear_screen() {
	cout << string(50, '\n');
}

void press_to_continue() {
	cout << "Press Enter to show hand..." << endl;
	while (cin.get() != '\n') {
		cout << "Press Enter to show hand..." << endl;
	}
	cout << endl;
}

