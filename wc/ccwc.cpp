#include <unistd.h>
#include <stdlib.h>

using namespace std;

#include <string>
#include <iostream>

static void help(void);
static void count_bytes(void);
static void count_words(void);
static void count_lines(void);

int main(int argc, char* argv[])
{
	bool cflag = false;
	string input_filename = "";
	int c;

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

	

	return 0;
}

static void help() {

}
