#include <unistd.h>
#include <stdlib.h>

using namespace std;

#include <string>
#include <iostream>
#include <fstream>

static void help(void);
static size_t count_bytes(ifstream &infile);
static size_t count_words(ifstream &infile);
static size_t count_lines(ifstream &infile);

int main(int argc, char* argv[])
{
	bool cflag = false, lflag = false, wflag = false;
	string input_filename = "";
	int c;

	size_t number_of_bytes, number_of_lines, number_of_words;

	ifstream input_file;

	while((c = getopt(argc, argv, "clw")) != -1) {
		switch(c) {
			case 'c':
				cflag = true;
				break;
			case 'l':
				lflag = true;
				break;
			case 'w':
				wflag = true;
				break;
			case '?':
			case ':':
			default:
				help();
				break;
		}
	}

	if (optind == argc-1)
	{
		input_filename.assign(argv[optind]);
	}

	if (input_filename.size() == 0)
	{
		help();
	}

	input_file.open(input_filename);	

	if (cflag) {
		number_of_bytes = count_bytes(input_file);

		cout << number_of_bytes << "\t";
	}

	if (lflag) {
		number_of_lines = count_lines(input_file);

		cout << number_of_lines << "\t";
	}

	if (wflag) {
		number_of_words = count_words(input_file);

		cout << number_of_words << "\t";
	}

	cout << input_filename;

	input_file.close();

	return 0;
}

static void help() {
	exit(1);
}

static size_t count_bytes(ifstream &infile) {
	size_t bytes = 0;

	infile.seekg(0, ios::beg);
	infile.seekg(0, ios::end);
	bytes = infile.tellg();

	return bytes;
}

static size_t count_lines(ifstream &infile) {
	size_t lines = 0;	

	infile.seekg(0, ios::beg);

	do {
		int c = infile.get();
		if (c == '\n') lines++; 

	} while (!infile.eof());


	return lines;
}

static size_t count_words(ifstream &infile) {
	size_t words = 0;

	infile.seekg(0, ios::beg);

	string word;

	while (infile >> word) {
		words++;
	}

	return words;
}
