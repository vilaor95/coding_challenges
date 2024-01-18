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
	GO = 0,
	OK = 1,
	OB = 2,
	AR = 3,
	KE = 4,
	CO = 5,
	VA = 6,
	ST = 7,
	T1 = 8, //tr
	T2 = 9, //tru
	T3 = 10,//true
	F1 = 11,//fa
	F2 = 12,//fal
	F3 = 13,//fals
	F4 = 14,//false
	N1 = 15,//nu
	N2 = 16,//nul
	N3 = 17,//null
	ZE = 18,//zero
	IN = 19,//integer
	BS = 20,//backslash
	MI = 21,//minus
	FR = 22,//fractional
	E1 = 23,//exponential unsgined 
	E2 = 24,//exponential signed
	E3 = 25,//exponent
	U1 = 26,
	U2 = 27,
	U3 = 28,
	U4 = 29,
	
	NR_STATES
};

enum inputs_e {
	SPACE = 0,
	WHITE = 1,
	LCURB = 2,
	RCURB = 3,
	LSQRB = 4,
	RSQRB = 5,
	COLON = 6,
	COMMA = 7,
	QUOTE = 8,
	LOW_A = 9,
	LOW_B,
	LOW_C,
	LOW_D,
	LOW_E,
	LOW_F,
	LOW_L,
	LOW_N,
	LOW_R,
	LOW_S,
	LOW_T,
	LOW_U = 20,
	ZERO  = 21,
	DIGIT = 22,
	BACKS = 23,
	MIN   = 24,
	POINT = 25,
	PLUS  = 26,
	UP_A  = 27,
	UP_B,
	UP_C,
	UP_D,
	UP_E,
	UP_F  = 32,
	SLASH = 33,
	ETC,
	NR_INPUTS
};

static int ascii_input[] = {
	//0
	__,    __,    __,    __,    __,    __,    __,    __,
	__,    WHITE, WHITE, __,    __,    WHITE, __,    __,
	__,    __,    __,    __,    __,    __,    __,    __,
	__,    __,    __,    __,    __,    __,    __,    __,

	//32
	SPACE, ETC,   QUOTE, ETC,   ETC,   ETC,   ETC,   ETC,
	ETC,   ETC,   ETC,   PLUS,  COMMA, MIN,   POINT, SLASH,
	ZERO,  DIGIT, DIGIT, DIGIT, DIGIT, DIGIT, DIGIT, DIGIT,
	DIGIT, DIGIT, COLON, ETC,   ETC,   ETC,   ETC,   ETC,

	//64
	ETC,   UP_A,  UP_B,  UP_C,  UP_D,  UP_E,  UP_F,  ETC,
	ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,
	ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,   ETC,
	ETC,   ETC,   ETC,   LSQRB, BACKS, RSQRB, ETC,   ETC,

	//96
	ETC,   LOW_A, LOW_B, LOW_C, LOW_D, LOW_E, LOW_F, ETC,
	ETC,   ETC,   ETC,   ETC,   LOW_L, ETC,   LOW_N, ETC,
	ETC,   ETC,   LOW_R, LOW_S, LOW_T, LOW_U, ETC,   ETC,
	ETC,   ETC,   ETC,   LCURB, ETC,   RCURB, ETC,   ETC,
};

enum actions_e {
	NO = -10, //NEW_OBJECT
	NA = -9,  //NEW_ARRAY
	XE = -8,  //CLOSE_EMPTY
	XA = -7,  //CLOSE_ARRAY
	XK = -6,  //CLOSE_KEY
	FC = -5,  //FOUND_COLON
	XO = -4,  //CLOSE_OBJECT
	NC = -3,  //NEXT_COMMA
};

enum mode_e {
	MODE_OBJECT,
	MODE_ARRAY,
	MODE_KEY,
	MODE_DONE,
	NR_MODES
};

static int transitions_table[NR_STATES][NR_INPUTS] {
	/*                                                                                             1-9                                                 */
	/*      SP  WH  {   }   [   ]   :   ,   "   a   b   c   d   e   f   l   n   r   s   t   u   0   |   \   -   .   +   A   B   C   D   E   F   /   ETC*/
	/*GO*/ {GO, GO, NO, __, NA, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*OK*/ {OK, OK, __, XO, __, XA, __, NC, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*OB*/ {OB, OB, __, XE, __, __, __, __, ST, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*AR*/ {AR, AR, __, __, NA, XA, __, __, ST, __, __, __, __, __, F1, __, N1, __, __, T1, __, ZE, IN, __, __, __, __, __, __, __, __, __, __, __, __},
	/*KE*/ {KE, KE, __, __, __, __, __, __, ST, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*CO*/ {CO, CO, __, __, __, __, FC, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*VA*/ {VA, VA, NO, __, NA, __, __, __, ST, __, __, __, __, __, F1, __, N1, __, __, T1, __, ZE, IN, __, MI, __, __, __, __, __, __, __, __, __, __},
	/*ST*/ {ST, __, ST, ST, ST, ST, ST, ST, XK, ST, ST, ST, ST, ST, ST, ST, ST, ST, ST, ST, ST, ST, ST, BS, ST, ST, ST, ST, ST, ST, ST, ST, ST, ST, ST},
	/*T1*/ {__, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, T2, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*T2*/ {__, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, T3, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*T3*/ {__, __, __, XO, __, XA, __, __, __, __, __, __, __, OK, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*F1*/ {__, __, __, __, __, __, __, __, __, F2, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*F2*/ {__, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F3, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*F3*/ {__, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F4, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*F4*/ {__, __, __, XO, __, XA, __, __, __, __, __, __, __, OK, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*N1*/ {__, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, N2, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*N2*/ {__, __, __, __, __, __, __, __, __, __, __, __, __, __, __, N3, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*N3*/ {__, __, __, XO, __, XA, __, __, __, __, __, __, __, __, __, OK, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __},
	/*ZE*/ {OK, OK, __, XO, __, XA, __, NC, __, __, __, __, __, E1, __, __, __, __, __, __, __, __, __, __, __, FR, __, __, __, __, __, E1, __, __, __},
	/*IN*/ {OK, OK, __, XO, __, XA, __, NC, __, __, __, __, __, E1, __, __, __, __, __, __, __, IN, IN, __, __, FR, __, __, __, __, __, E1, __, __, __},
	/*BS*/ {__, __, __, __, __, __, __, __, ST, __, ST, __, __, __, ST, __, ST, ST, __, ST, U1, __, __, ST, __, __, __, __, __, __, __, __, __, ST, __},
	/*MI*/ {__, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, ZE, IN, __, __, __, __, __, __, __, __, __, __, __, __},
	/*FR*/ {OK, OK, __, XO, __, XA, __, NC, __, __, __, __, __, E1, __, __, __, __, __, __, __, FR, FR, __, __, __, __, __, __, __, __, E1, __, __, __},
	/*E1*/ {__, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, E3, E3, __, E2, __, E2, __, __, __, __, __, __, __, __},
	/*E2*/ {__, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, E3, E3, __, __, __, __, __, __, __, __, __, __, __, __},
	/*E3*/ {OK, OK, __, XO, __, XA, __, NC, __, __, __, __, __, __, __, __, __, __, __, __, __, E3, E3, __, __, __, __, __, __, __, __, __, __, __, __},
	/*U1*/ {__, __, __, __, __, __, __, __, __, U2, U2, U2, U2, U2, U2, __, __, __, __, __, __, U2, U2, __, __, __, __, U2, U2, U2, U2, U2, U2, __, __},
	/*U2*/ {__, __, __, __, __, __, __, __, __, U3, U3, U3, U3, U3, U3, __, __, __, __, __, __, U3, U3, __, __, __, __, U3, U3, U3, U3, U3, U3, __, __},
	/*U3*/ {__, __, __, __, __, __, __, __, __, U4, U4, U4, U4, U4, U4, __, __, __, __, __, __, U4, U4, __, __, __, __, U4, U4, U4, U4, U4, U4, __, __},
	/*U4*/ {__, __, __, __, __, __, __, __, __, ST, ST, ST, ST, ST, ST, __, __, __, __, __, __, ST, ST, __, __, __, __, ST, ST, ST, ST, ST, ST, __, __},
};

class JsonChecker {
	private:
		unsigned int valid;
		int state;
		unsigned int depth;
		unsigned int max_depth;

		std::stack<int> st;

	public:	
		JsonChecker (unsigned int depth);
		int check_char(int next_char);
		int parse_done(void);
};

JsonChecker::JsonChecker (unsigned int max_depth)
{
	this->valid = GOOD;
	this->state = GO;
	this->depth = 0;
	this->max_depth = max_depth;

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

	if (next_state >= 0) {
		this->state = next_state;
	} else {
		switch(next_state) {
			case NO: //NEW_OBJECT
				this->st.push(MODE_KEY);
				this->state = OB;
				break;

			case NA: //NEW_ARRAY
				this->st.push(MODE_ARRAY);
				this->state = AR;
				this->depth++;
				break;

			case XE: //CLOSE_EMPTY
				if (this->st.top() != MODE_KEY)
					return FALSE;

				this->st.pop();
				this->state = OK;
				break;

			case XO: //CLOSE_OBJECT
				if (this->st.top() != MODE_OBJECT)
					return FALSE;

				this->st.pop();
				this->state = OK;
				break;

			case XA: //CLOSE_ARRAY
				if (this->st.top() != MODE_ARRAY)
					return FALSE;

				if (this->depth >= this->max_depth)
					return FALSE;

				this->depth--;
				this->st.pop();
				this->state = OK;
				break;

			case XK:
				switch (this->st.top()) {
					case MODE_KEY:
						this->state = CO;
						break;
					case MODE_OBJECT:
					case MODE_ARRAY:
						this->state = OK;
						break;
				}
				break;
			
			case FC:
				if (this->st.top() != MODE_KEY) {
					return FALSE;
				}
				this->st.pop();
				this->st.push(MODE_OBJECT);
				this->state = VA;
				break;

			case NC:
				switch (this->st.top()) {
					case MODE_OBJECT:
						this->st.pop(); this->st.push(MODE_KEY);
						this->state = KE;
						break;
					case MODE_ARRAY:
						this->state = VA;
						break;
					default:
						return FALSE;
						break;
				}
				break;

			case __: //INVALID
				return FALSE;
				break;
		}
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
