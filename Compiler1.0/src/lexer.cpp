#include "ast.h"

//rule1: T_INT_CONST := [1-9][0-9]* | [0]
//rule2: T_IDENT := [a-zA-Z_][a-zA-Z0-9_]*
//rule3: KW_RETURN := "return"
//rule4: KW_INT := "int"
//rule5: whitespace := [ \t\n\r]+

class DFA {
public:
	std::vector<std::vector<int>> trans{ 50, std::vector<int>(256, -1) };
	std::vector<TokenType> accept_type{ 50, TokenType::UNKNOWN };
	DFA() {
		accept_type[1] = TokenType::T_INT_CONST;
		accept_type[2] = TokenType::T_IDENT;
		accept_type[11] = TokenType::KW_RETURN;
		accept_type[8] = TokenType::KW_INT;
		accept_type[12] = TokenType::T_INT_CONST;

		for (char c = '1'; c <= '9'; c++) {
			trans[0][c] = 1;
		}
		for (char c = '0'; c <= '9'; c++) {
			trans[1][c] = 1;
			trans[2][c] = 2;
			trans[8][c] = 2;
			trans[11][c] = 2;
			trans[12][c] = 13;
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
			}

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
			trans[8][c] = 2;
			trans[11][c] = 2;
		}
		trans[0]['_'] = 2;
		trans[2]['_'] = 2;
		trans[8]['_'] = 2;
		trans[11]['_'] = 2;
		trans[0]['0'] = 12;
	}
};

void lex(std::FILE* file, std::vector<Token>& tokens) {
	std::string src;
	int c;
	while ((c = std::fgetc(file)) != EOF) src.push_back(static_cast<char>(c));

	DFA dfa;
	std::size_t i = 0;
	while (i < src.size()) {
		if (std::isspace(static_cast<unsigned char>(src[i]))) { ++i; continue; }

		TokenType single = TokenType::UNKNOWN;
		switch (src[i]) {
		case '(': single = TokenType::T_OPEN_PAREN; break;
		case ')': single = TokenType::T_CLOSE_PAREN; break;
		case '{': single = TokenType::T_OPEN_CURLY;  break;
		case '}': single = TokenType::T_CLOSE_CURLY; break;
		case ';': single = TokenType::T_SEMI;        break;
		}
		if (single != TokenType::UNKNOWN) {
			tokens.push_back({ single, std::string(1, src[i]) });
			++i;
			continue;
		}

		int state = 0;
		TokenType last_accept = TokenType::UNKNOWN;
		std::size_t j = i, last_pos = i;
		while (j < src.size()) {
			int ns = dfa.trans[state][static_cast<unsigned char>(src[j])];
			if (ns == -1) break; 
			state = ns;
			++j;
			if (dfa.accept_type[state] != TokenType::UNKNOWN) {  
				last_accept = dfa.accept_type[state];
				last_pos = j;
			}
		}
		if (state == 13) {
			std::cerr << "lex error: number with leading zero at offset " << i << "\n";
			return;
		}
		if (last_accept == TokenType::UNKNOWN) {   
			std::cerr << "lex error at offset " << i << ": '" << src[i] << "'\n";
			return;
		}
		tokens.push_back({ last_accept, src.substr(i, last_pos - i) }); 
		i = last_pos;  
	}
	tokens.push_back({ TokenType::T_EOF, "" }); 
}

const char* tokenTypeToString(TokenType type) {
	switch (type) {
	case TokenType::KW_RETURN: return "KW_RETURN";
	case TokenType::KW_INT: return "KW_INT";
	case TokenType::T_IDENT: return "T_IDENT";
	case TokenType::T_OPEN_PAREN: return "T_OPEN_PAREN";
	case TokenType::T_CLOSE_PAREN: return "T_CLOSE_PAREN";
	case TokenType::T_OPEN_CURLY: return "T_OPEN_CURLY";
	case TokenType::T_CLOSE_CURLY: return "T_CLOSE_CURLY";
	case TokenType::T_INT_CONST: return "T_INT_CONST";
	case TokenType::T_SEMI: return "T_SEMI";
	case TokenType::T_EOF: return "T_EOF";
	default: return "UNKNOWN";
	}
}