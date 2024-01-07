#include <iostream>
#include <fstream>
#include <vector>

enum RetCode_e {
	SUCCESS = 0,
	FAILURE = 1,
};

static void invalid(void);
static void error(void);

static RetCode_e parse_json(std::istream &stream);
static void lex(std::istream &stream, std::vector<std::string> &tokens);
static RetCode_e parse(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

static int ident_level = 0;

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

static RetCode_e parse_json(std::istream &stream) {
	RetCode_e ret = FAILURE;

	std::vector<std::string> tokens;	

	lex(stream, tokens);
	ret = parse(tokens.begin(), tokens.end());

	std::cout << "\n";

	return ret;
}

static void lex(std::istream &stream, std::vector<std::string> &tokens) {

	char c;
	static std::string token = "";
	while (stream >> c) {
		switch (c) {
			case '{':
			case '}':
			case ':':
			case ',':
				if (token.size() != 0) {
					tokens.push_back(token);
					token.clear();
				}
				tokens.push_back(std::string(1, c));
				break;

			case ' ':
				tokens.push_back(token);
				token.clear();
				break;
			default:
				token.push_back(c);
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
	RetCode_e ret = FAILURE;

	ident_level++;	

	std::string token = *begin;
	//std::cout << "parse_object next token " << token << "\n";

	if (token == "}") {
		ident_level--;

		std::cout << std::string(ident_level, '\t');
		
		ret = SUCCESS;
	}
	else {
		while (true) {
			std::string key = *begin;
			if (key[0] != '"') {
				return FAILURE;
			}
			std::cout << std::string(ident_level, '\t') << key << " ";

			begin += 1;

			if (*begin != ":") {
				return FAILURE;
				break;
			}
			token = *begin;
			std::cout << token << " ";

			begin += 1;

			ret = parse(begin, end);

			begin += 1;

			token = *begin;
			if (token == "}") {
				ident_level--;

				std::cout << "\n" << std::string(ident_level, '\t') << token;

				ret = SUCCESS;
				break;
			}

			if (token != ",") {
				ret = FAILURE;
				break;
			}
			std::cout << token << "\n";

			begin += 1;
		}
	}

	return ret;
}

static RetCode_e parse(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
	RetCode_e ret = FAILURE;

	std::string token = *begin;

	//std::cout << "parse next token: " << t << "\n";

	if (token == "{") {
		std::cout << std::string(ident_level, '\t') << token << "\n";
		ret = parse_object(begin+1, end);
	} else if (token == "[") {

	} else {
		std::cout << token;
	}

	return ret;
}
