#include <cstdlib>
#include <stack>

#include <string>
#include <iostream>
#include <fstream>

#define GOOD (0xCAFEBABE)

#define TRUE (1)
#define FALSE (0)

#define __ (-1) //error code

enum states_e {
	GO,
	OK,
	OB,
	AR,
	KE,
	CO,
	VA,
	ST,
	NR_STATES
};

enum inputs_e {
	SPACE,
	WHITE,
	LCURB,
	RCURB,
	LSQRB,
	RSQRB,
	COLON,
	COMMA,
	QUOTE,
	ETC,
	NR_INPUTS
};

static int ascii_input[] = {
	__,    __,    __,    __,    __,    __,    __,    __,
	__,    WHITE, WHITE, __,    __,    WHITE, __,    __,
	__,    __,    __,    __,    __,    __,    __,    __,
	__,    __,    __,    __,    __,    __,    __,    __,

	SPACE, ETC,   QUOTE, ETC,   ETC,   ETC,   ETC,   ETC,
	ETC,   ETC,   ETC,   ETC,   COMMA, ETC,   ETC,   ETC,
	ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,
	ETC,   ETC,   ETC,   ETC,   COLON, ETC,   ETC,   ETC,

	ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,
	ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,
	ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,
	ETC,   ETC,   ETC,   LSQRB, ETC,   RSQRB, ETC,   ETC,

	ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,
	ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,
	ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,
	ETC,   ETC,   ETC,   LCURB, ETC,   RCURB, ETC,   ETC,
};

enum actions_e {
	NO = -10, //NEW_OBJECT
	NA = -9,  //NEW_ARRAY
	XO = -8,  //CLOSE_OBJECT
	XA = -7,  //CLOSE_ARRAY
};

enum mode_e {
	MODE_OBJECT,
	MODE_ARRAY,
	MODE_KEY,
	MODE_DONE,
	NR_MODES
};

static int transitions_table[NR_STATES][NR_INPUTS] {
	/*      SP  WH  {   }   [   ]   :   ,   "   ETC*/
	/*GO*/ {GO, GO, NO, __, __, __, __, __, __, __},
	/*OK*/ {__, __, __, __, __, __, __, __, __, __},
	/*OB*/ {__, __, __, XO, __, __, __, __, __, __},
	/*AR*/ {__, __, __, __, __, __, __, __, __, __},
	/*KE*/ {__, __, __, __, __, __, __, __, __, __},
	/*CO*/ {__, __, __, __, __, __, __, __, __, __},
	/*VA*/ {__, __, __, __, __, __, __, __, __, __},
	/*ST*/ {__, __, __, __, __, __, __, __, __, __},
};

class JsonChecker {
	private:
		int valid;
		int state;
		int depth;

		std::stack<int> st;

	public:	
		JsonChecker (unsigned int depth);
		int check_char(int next_char);
		int parse_done(void);
};

JsonChecker::JsonChecker (unsigned int depth)
{
	this->valid = GOOD;
	this->state = GO;
	this->depth = depth;

	this->st.push(MODE_DONE);
}

int
JsonChecker::check_char(int next_char)
{
	if (this->valid != GOOD) {
		return FALSE;
	}

	int input_char = ascii_input[next_char];	

	int next_state = transitions_table[this->state][input_char];

	switch(next_state) {
		case GO:
		case OK:
		case OB:
		case AR:
		case KE:
		case CO:
		case VA:
		case ST:
			this->state = next_state;
			break;

		case NO: //NEW_OBJECT
			this->st.push(MODE_KEY);
			this->state = OB;
			break;
		case NA: //NEW_ARRAY
		case XO: //CLOSE_OBJECT
			if (this->st.top() != MODE_KEY)
				return FALSE;

			this->st.pop();
			this->state = OK;
			break;
		case XA: //CLOSE_ARRAY

		case __: //INVALID
			return FALSE;
			break;
	}

	return TRUE;
}

int
JsonChecker::parse_done()
{
	if (this->valid != GOOD) {
		return FALSE;
	}
	
	return ((this->state == OK) && (this->st.top() == MODE_DONE));
}


int main(int argc, char* argv[])
{
	std::string filename = "";
	std::ifstream file;

	if (argc != 2) {
		exit(1);
	}

	filename.assign(argv[1]);

	file.open(filename);
	if (file.fail()) {
		exit(1);
	}

	JsonChecker jc(20);

	char c;
	while (file >> std::noskipws >> c) {
		if (!jc.check_char(c)) {
			std::cout << "JSON checker char: syntax error\n";
			exit(1);
		}
	}

	if (!jc.parse_done()) {
		std::cout << "JSON checker end: syntax error\n";
		exit(1);
	}
}
