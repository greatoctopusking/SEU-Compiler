#include "ast.h"

//rule1: T_INT_CONST := [1-9][0-9]*
//rule2: T_IDENT := [a-zA-Z_][a-zA-Z0-9_]*
//rule3: KW_RETURN := "return"
//rule4: KW_INT := "int"
//rule5: whitespace := [ \t\n\r]+

class DFA {
	std::vector<std::vector<int>> trans(50, std::vector<int>(256, -1));
	DFA() {
		for (char c = '1'; c <= '9'; c++) {
			trans[0][c] = 1;
		}
		for (char c = '0'; c <= '9'; c++) {
			trans[1][c] = 1;
			trans[2][c] = 2;
		}
		for (char c = 'a'; c <= 'z'; c++) {
			if (c == 'r') {
				trans[0][c] = 3; //state3: r^eturn
			}
			else if (c == 'i') {
				trans[0][c] = 4; //state4: i^nt
			}
			else {
				trans[0][c] = 2;
			})

			trans[2][c] = 2;
			trans[8][c] = 2;
			trans[11][c] = 2;

			if (c == 'e') {
				trans[3][c] = 5; //state5: re^turn
			}
			else {
				trans[3][c] = 2; 
			}

			if (c == 'n') {
				trans[4][c] = 6; //state6: in^t
				trans[10][c] = 11; //state11: return^
			}
			else {
				trans[4][c] = 2;
				trans[10][c] = 2;
			}

			if (c == 't') {
				trans[5][c] = 7; //state7: ret^urn
				trans[6][c] = 8; //state8: int^
			}
			else {
				trans[5][c] = 2;
				trans[6][c] = 2;
			}

			if (c == 'u') {
				trans[7][c] = 9; //state9: retu^rn
			}
			else {
				trans[7][c] = 2;
			}

			if (c == 'r') {
				trans[9][c] = 10; //state10: retur^n
			}
			else {
				trans[9][c] = 2;
			}
		}
		for (char c = 'A'; c <= 'Z'; c++) {
			trans[0][c] = 2;
			trans[2][c] = 2;
		}
		trans[0]['_'] = 2;
		trans[2]['_'] = 2;
		trans[8]['_'] = 2;
		trans[11]['_'] = 2;
	}
};

void lex(FILE* file, std::vector<Token>& tokens) {
	DFA dfa;
	
}