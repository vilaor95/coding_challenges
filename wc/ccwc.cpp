#include <unistd.h>
#include <stdlib.h>

#include <string>
#include <iostream>
#include <fstream>

static void help(void);
static size_t count_bytes(std::istream &infile);
static size_t count_words(std::istream &infile);
static size_t count_lines(std::istream &infile);
static size_t count_characters(std::istream &infile);

const std::string separator = "  ";

int main(int argc, char* argv[]) {
	bool cflag = false, lflag = false, wflag = false, mflag = false;
	std::string input_filename = "";
	int c;

	size_t number_of_bytes, number_of_lines, number_of_words, number_of_characters;

	std::istream* input = &std::cin;

	std::ifstream input_file;

	while((c = getopt(argc, argv, "clwm")) != -1) {
		switch(c) {
			case 'c':
				cflag = true;
				break;
			case 'l':
				lflag = true;
				break;
			case 'w':
				wflag = true; break;
			case 'm':
				mflag = true;
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

		if (argc == 2) {
			cflag = lflag = wflag = true;
		}
	
		input_file.open(input_filename);	
		input = &input_file;
	}

	if (lflag) {
		number_of_lines = count_lines(*input);

		std::cout << number_of_lines << separator;
	}

	if (wflag) {
		number_of_words = count_words(*input);

		std::cout << number_of_words << separator;
	}

	if (cflag) {
		number_of_bytes = count_bytes(*input);

		std::cout << number_of_bytes << separator;
	}

	if (mflag) {
		number_of_characters = count_characters(*input);

		std::cout << number_of_characters << separator;
	}

	std::cout << input_filename;

	return 0;
}

static void help() {
	exit(1);
}

static size_t count_bytes(std::istream &infile) {
	size_t bytes = 0;

	infile.clear();
	infile.seekg(0, std::ios::beg);
	infile.seekg(0, std::ios::end);
	bytes = infile.tellg();

	return bytes;
}

static size_t count_lines(std::istream &infile) {
	size_t lines = 0;	

	infile.clear();
	infile.seekg(0, std::ios::beg);
	std::string line;
	while (std::getline(infile, line)) lines++;

	return lines;
}

static size_t count_words(std::istream &infile) {
	size_t words = 0;

	infile.clear();
	infile.seekg(0, std::ios::beg);

	std::string word;

	while (infile >> word) {
		words++;
	}

	return words;
}

static size_t count_characters(std::istream &infile) {
	size_t characters = 0;

	infile.clear();
	infile.seekg(0, std::ios::beg);

	char ch;
	while(infile >> std::noskipws >> ch)
	{
		if ((ch & 0x80) == 0x00)
		{
			characters++;
		}
		else if ((ch & 0xC0) == 0xC0)
		{
			characters++;
			infile >> ch;
		}
		else if ((ch & 0xE0) == 0xE0)
		{
			characters++;
			infile >> ch;
			infile >> ch;
		}
		else if ((ch & 0xF0) == 0xF0)
		{
			characters++;
			infile >> ch;
			infile >> ch;
			infile >> ch;
		}
		else {
			//error
		}
	}

	return characters;
}
