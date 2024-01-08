#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define IDENT_LEVEL (std::string(2*ident_level, ' '))

enum RetCode_e {
	SUCCESS = 0,
	FAILURE = 1,
};

static void invalid(void);
static void error(void);

static RetCode_e parse_json(std::istream &stream);
static void lex(std::istream &stream, std::vector<std::string> &tokens);
static RetCode_e parse(std::vector<std::string>::iterator *begin, std::vector<std::string>::iterator *end);

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

	std::vector<std::string>::iterator begin = tokens.begin();
	std::vector<std::string>::iterator end = tokens.end();

	ret = parse(&begin, &end);
	std::cout << "\n";

	if (begin != end) {
		std::cout << "!!!ERROR - Extra value after close\n";
		ret = FAILURE;
	}

	return ret;
}

static void lex(std::istream &stream, std::vector<std::string> &tokens) {

	char c;
	static std::string token = "";
	while (stream >> std::noskipws >> c) {
		switch (c) {
			case '{':
			case '}':
			case '[':
			case ']':
			case ':':
			case ',':
				if (token.size() != 0) {
					tokens.push_back(token);
					token.clear();
				}
				tokens.push_back(std::string(1, c));
				break;

			case '"':
				token.push_back(c);
				if (token.size() != 1) {
					tokens.push_back(token);
					token.clear();
				}
				break;
			case ' ':
				if (token.size() != 0) {
					token.push_back(c);
				}
				break;
			case '\n':
				break;
			default:
				token.push_back(c);
				break;
		}
	}

	//print tokens
	/*
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
	*/
}

static RetCode_e parse_object(std::vector<std::string>::iterator *begin, std::vector<std::string>::iterator *end) {
	RetCode_e ret = FAILURE;

	ident_level++;	

	std::string token = **begin;

	if (token == "}") {
		ident_level--;
		std::cout << IDENT_LEVEL << "}";
		ret = SUCCESS;
	}
	else {
		while (true) {
			std::string key = **begin;
			if (key[0] != '"') {
				return FAILURE;
			}
			std::cout << IDENT_LEVEL << key << " ";

			(*begin)++;
			token = **begin;
			if (token != ":") {
				return FAILURE;
				break;
			}
			std::cout << token << " ";

			(*begin)++;

			ret = parse(begin, end);
			if (ret != SUCCESS) {
				break;
			}

			token = **begin;
			(*begin)++;
			if (token == "}") {
				ident_level--;

				std::cout << "\n" << IDENT_LEVEL << token;

				ret = SUCCESS;
				break;
			}

			if (token != ",") {
				ret = FAILURE;
				break;
			}
			std::cout << token << "\n";
		}
	}

	return ret;
}

static RetCode_e parse_array(std::vector<std::string>::iterator *begin, std::vector<std::string>::iterator *end) {
	RetCode_e ret = FAILURE;

	std::string token = **begin;

	if (token == "]") {
		(*begin)++;
		std::cout << token;
		ret = SUCCESS;
	}
	else {
		while (true) {
			ret = parse(begin, end);
			if (ret != SUCCESS) break;

			token = **begin;
			(*begin)++;
			if (token == "]") {
				std::cout << token;
				ret = SUCCESS;
				break;
			}
			if (token != ",") {
				ret = FAILURE;
				break;
			}
			std::cout << token << " ";
		}
	}

	return ret;
}

static bool is_valid_number(std::string token) {
	if (token[0] == '0') return false;

	return !token.empty() && std::find_if(token.begin(), 
			token.end(), [](unsigned char c) { return !std::isdigit(c);  }) == token.end();	
}

static bool is_valid(std::string token) {
	bool valid = false;

	static std::string keywords[] = {std::string("true"), std::string("false"), std::string("null")};

	if ((*(token.begin()) == '"') && (*(token.end()-1) == '"')) valid = true;

	if (is_valid_number(token)) valid = true;	

	for (const auto& k : keywords) {
		if (token == k) valid = true;
	}

	return valid;
}

static RetCode_e parse(std::vector<std::string>::iterator *begin, std::vector<std::string>::iterator *end) {
	RetCode_e ret = FAILURE;

	std::string token = **begin;
	(*begin)++;

	if (token == "{") {
		std::cout << IDENT_LEVEL << token << "\n";
		ret = parse_object(begin, end);
	} else if (token == "[") {
		std::cout << token;
		ret = parse_array(begin, end);
	} else {
		if (is_valid(token)) {
			std::cout << token;
			ret = SUCCESS;
		}
		else {
			ret = FAILURE;
		}
	}

	return ret;
}
