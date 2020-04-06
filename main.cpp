#include <stdio.h>
#include <vector>
#include <string>
#include "CNeural.h"

using namespace std;

string prefix = "data_u2fjv";
char locations[7] = {'d', 'g', 'm', 'u', '6', '7', '9'};
const char* places[4] = {"shop", "playground", "pharmacy", "park"};

void skip_line(FILE* f) {
	char c;
	do {
		c = fgetc(f);
	} while (!feof(f) && c != '\n');
}

void skip_cell(FILE* f) {
	char c;
	do {
		c = fgetc(f);
	} while (!feof(f) && c != ',');
}

string read_string(FILE* f) {
	char c;
	string s = "";
	do {
		c = fgetc(f);
		if (c != ',') {
			s += c;
		}
	} while (!feof(f) && c != ',');
	return s;
}

int main(int argc, char** argv) {
	CNeural neural(4, 7*4);
	for (int loc = 0; loc < 7; ++loc) {
		FILE* f = fopen((prefix + locations[loc]).c_str(), "r");
		char c = fgetc(f);
		skip_line(f);

		while (!feof(f)) {
			skip_cell(f);
			skip_cell(f);
			skip_cell(f);
			int crowded_ratio = stoi(read_string(f));
			skip_cell(f);
			double timestamp = stod(read_string(f));
			string place = read_string(f);
			int place_index = 0;
			for (int i = 0; i < 4; ++i) {
				if (string(places[i]) == place) {
					place_index = i;
					break;
				}
			}
			skip_line(f);

			std::vector<double> state;
			state.resize(7*4, NAN);
			state[loc*4 + place_index] = crowded_ratio;
			neural.add_v(timestamp, state);
		}

		fclose(f);
	}

	neural.update(4);

	return 0;
}
