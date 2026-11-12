#include"ast.h"
#include<unordered_map>
#include<stack>

/*
	r1: CompUnit  ::= FuncDef;

	r2: FuncDef   ::= FuncType IDENT "(" ")" Block;
	r3: FuncType  ::= "int";

	r4: Block     ::= "{" Stmt "}";
	r5: Stmt      ::= "return" Number ";";
	r6: Number    ::= INT_CONST;
*/

//terminals = { KW_RETURN, KW_INT, T_IDENT, T_OPEN_PAREN, T_CLOSE_PAREN, T_OPEN_CURLY, T_CLOSE_CURLY, T_INT_CONST, T_SEMI, T_EOF }
//non-terminals = { CompUnit, FuncDef, FuncType, Block, Stmt, Number }

//LR(0) parsing table
/*
	0: IDENT
	1: T_OPEN_PAREN
	2: T_CLOSE_PAREN
	3: KW_INT
	4: T_OPEN_CURLY
	5: T_CLOSE_CURLY
	6: KW_RETURN
	7: T_SEMI
	8: T_INT_CONST
	9: T_EOF
	10: CompUnit
	11: FuncDef
	12: FuncType
	13: Block
	14: Stmt
	15: Number
*/
//positive: shift, negative: reduce, 256: accept, 0: error

void buildParsingTable(std::vector<std::vector<short>>& parsing_table) {
	parsing_table[0][3] = 4;
	parsing_table[0][10] = 1;
	parsing_table[0][11] = 2;
	parsing_table[0][12] = 3;
	parsing_table[1][9] = 256;
	parsing_table[3][0] = 5;
	parsing_table[5][1] = 6;
	parsing_table[6][2] = 7;
	parsing_table[7][4] = 9;
	parsing_table[9][6] = 12;
	parsing_table[10][5] = 11;
	parsing_table[12][8] = 15;
	parsing_table[13][7] = 14;
	parsing_table[7][13] = 8;
	parsing_table[9][14] = 10;
	parsing_table[12][15] = 13;

	for (int i = 0; i < 10; ++i) {
		parsing_table[2][i] = -1;
		parsing_table[4][i] = -3;
		parsing_table[8][i] = -2;
		parsing_table[11][i] = -4;
		parsing_table[14][i] = -5;
		parsing_table[15][i] = -6;
	}
}

void parse(FILE* file, BaseAST* ast) {
	std::vector<Token> tokens;
	lex(file, tokens);

	vector<vector<short>> parsing_table(16, vector<short>(16, 0));

	buildParsingTable(parsing_table);

	stack<int> state_stack;
	stack<int> ast_stack;
}