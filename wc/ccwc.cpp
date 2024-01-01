#include <unistd.h>
#include <stdlib.h>

using namespace std;

#include <string>
#include <iostream>
#include <fstream>

static void help(void);
static size_t count_bytes(ifstream &infile);
static void count_words(void);
static void count_lines(void);

int main(int argc, char* argv[])
{
	bool cflag = false;
	string input_filename = "";
	int c;

	size_t number_of_bytes;

	ifstream input_file;

	while((c = getopt(argc, argv, "c")) != -1) {
		switch(c) {
			case 'c':
				cflag = true;
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

	cout << input_filename;

	input_file.close();

	return 0;
}

static void help() {
	exit(1);
}

static size_t count_bytes(ifstream &infile) {
	infile.seekg(0, ios::end);
	return infile.tellg();
}
