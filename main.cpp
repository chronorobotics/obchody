#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "CNeural.h"

using namespace std;

string prefix = "data/data_u2fjv";
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
	double min_timestamp = 30000000000;
	double max_timestamp = 0;
	for (int loc = 0; loc < 7; ++loc) {
		string filename = prefix + locations[loc] + ".csv";
		cout << "loading " << filename << endl;
		FILE* f = fopen(filename.c_str(), "r");
		skip_line(f);

		while (!feof(f)) {
			skip_cell(f);
			skip_cell(f);
			skip_cell(f);
			if (feof(f)) {
				break;
			}
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
			if (timestamp < min_timestamp) {
				min_timestamp = timestamp;
			}
			if (timestamp > max_timestamp) {
				max_timestamp = timestamp;
			}
		}

		fclose(f);
	}

	cout << "Started training" << endl;

	neural.update(4);

	cout << "Training finished" << endl;

	FILE* f = fopen("results/predictions.txt", "w");
	for (double time = min_timestamp; time < max_timestamp + 14*86400; time += 3600) {
		std::vector<float> estimation = neural.estimate_v(time);
		fprintf(f, "%ld", long(time));
		for (int i = 0; i < 7*4; ++i) {
			fprintf(f, " %f", estimation[i]);
		}
		fprintf(f, "\n");
	}
	fclose(f);

	return 0;
}
