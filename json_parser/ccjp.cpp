#include <iostream>
#include <fstream>

static void invalid(void);
static void error(void);

static void parse_json(std::istream &file);

int main(int argc, char *argv[]) {

	std::string filename = "";

	std::ifstream file;

	if (argc != 2) {
		invalid();	
	}

	filename.assign(argv[1]);

	file.open(filename);

	if (file.fail()) {
		error();
	}

	parse_json(file);

	std::cout << "Succeed to parse JSON file\n";
	return 0;	
}

static void invalid() {
	std::cout << "Invalid number of arguments";
	exit(255);
}

static void error() {
	std::cout << "Fail to parse JSON file\n";
	exit(255);
}

static void parse_json(std::istream &file) {

}
