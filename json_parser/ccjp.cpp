#include <iostream>
#include <fstream>
#include <vector>

enum RetCode_e {
	SUCCESS = 0,
	FAILURE = 1,
};

static void invalid(void);
static void error(void);

static RetCode_e parse_json(std::istream &file);
static void lex(std::istream &file, std::vector<std::string> &tokens);
static RetCode_e parse(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

int main(int argc, char *argv[]) {
	RetCode_e ret = FAILURE;

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

	if (file.peek() != std::ifstream::traits_type::eof()) {
		ret = parse_json(file);
	}

	return ret;	
}

static void invalid() {
	std::cout << "Invalid number of arguments";
	exit(255);
}

static void error() {
	std::cout << "Fail to parse JSON file\n";
	exit(255);
}

static RetCode_e parse_json(std::istream &file) {
	RetCode_e ret = FAILURE;

	std::vector<std::string> tokens;	

	lex(file, tokens);
	ret = parse(tokens.begin(), tokens.end());

	return ret;
}

static void lex(std::istream &file, std::vector<std::string> &tokens) {

	char c;
	while (file >> c) {
		switch (c) {
			case '{':
			case '}':
				tokens.push_back(std::string(1, c));
				break;

			//strings
			case '"':
			case '\'':

			case ' ':
			default:
				break;
		}
	}

	//print tokens
	{
		std::string token_string;
		std::cout << "tokens: ";
		for (auto token : tokens) {
			token_string.append(token);
			token_string.append(",");
		}
		token_string.erase(token_string.end()-1);
		std::cout << token_string << "\n";
	}
}

static RetCode_e parse_object(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
	RetCode_e ret;

	std::string t = *begin;
	std::cout << "parse_object next token " << t << "\n";

	if (t == "}") {
		ret = SUCCESS;
	}

	return ret;
}

static RetCode_e parse(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
	RetCode_e ret = FAILURE;
	
	std::string t = *begin;

	std::cout << "parse next token: " << t << "\n";

	if (t == "{") {
		ret = parse_object(begin+1, end);
	} else if (t == "[") {

	} else {

	}

	return ret;
}
