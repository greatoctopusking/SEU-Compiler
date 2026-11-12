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
	0: KW_RETURN
	1: KW_INT
	2: T_IDENT
	3: T_OPEN_PAREN
	4: T_CLOSE_PAREN
	5: T_OPEN_CURLY
	6: T_CLOSE_CURLY
	7: T_INT_CONST
	8: T_SEMI
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
	parsing_table[0][1] = 4;
	parsing_table[0][10] = 1;
	parsing_table[0][11] = 2;
	parsing_table[0][12] = 3;
	parsing_table[1][9] = 256;
	parsing_table[3][2] = 5;
	parsing_table[5][3] = 6;
	parsing_table[6][4] = 7;
	parsing_table[7][5] = 9;
	parsing_table[9][0] = 12;
	parsing_table[10][6] = 11;
	parsing_table[12][7] = 15;
	parsing_table[13][8] = 14;
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

struct SemValue {
	std::unique_ptr<BaseAST> ast = nullptr;
	std::string text;
};

void parse(FILE* file, std::unique_ptr<BaseAST>& ast) {
	std::vector<Token> tokens;
	lex(file, tokens);
	if (tokens.empty() || tokens.back().type != TokenType::T_EOF) {
		std::cerr << "lex error: no EOF token\n";
		return;
	}

	std::vector<std::vector<short>> parsing_table(16, std::vector<short>(16, 0));

	buildParsingTable(parsing_table);

	std::stack<int> state_stack;
	std::stack<SemValue> ast_stack;

	state_stack.push(0);
	bool accepted = false;
	int token_index = 0, action;
	while (!accepted) {
		action = parsing_table[state_stack.top()][static_cast<int>(tokens[token_index].type)];
		if (action == 256) {
			if (state_stack.size() != 2 || ast_stack.size() != 1) {
				std::cerr << "parse error: invalid state or AST stack size\n";
				return;
			}
			accepted = true;
			ast = std::move(ast_stack.top().ast);
			return;
		}
		else if (action > 0) {
			state_stack.push(action);
			ast_stack.push(SemValue{ nullptr, tokens[token_index].value });
			token_index++;
		}
		else if (action < 0) {
			switch (action) {
			case -1: {
				auto funcdef = std::move(ast_stack.top().ast); ast_stack.pop();
				ast_stack.push(SemValue{ std::make_unique<CompUnit>(std::move(funcdef)), "" });

				state_stack.pop();
				state_stack.push(parsing_table[state_stack.top()][10]);
				break;
			}
			case -2: {
				auto block = std::move(ast_stack.top().ast); ast_stack.pop();
				ast_stack.pop(); ast_stack.pop(); 
				auto ident = std::make_unique<Ident>(ast_stack.top().text); ast_stack.pop();
				auto functype = std::move(ast_stack.top().ast); ast_stack.pop();
				ast_stack.push(SemValue{ std::make_unique<FuncDef>(std::move(functype), std::move(ident), std::move(block)), "" });

				for (int i = 0; i < 5; ++i) state_stack.pop();
				state_stack.push(parsing_table[state_stack.top()][11]);
				break;
			}
			case -3: {
				std::string func_type = ast_stack.top().text; ast_stack.pop();
				ast_stack.push(SemValue{ std::make_unique<FuncType>(std::move(func_type)), "" });

				state_stack.pop();
				state_stack.push(parsing_table[state_stack.top()][12]);
				break;
			}
			case -4: {
				ast_stack.pop();
				auto stmt = std::move(ast_stack.top().ast); ast_stack.pop();
				ast_stack.pop();
				ast_stack.push(SemValue{ std::make_unique<Block>(std::move(stmt)), "" });

				for (int i = 0; i < 3; ++i) state_stack.pop();
				state_stack.push(parsing_table[state_stack.top()][13]);
				break;
			}
			case -5: {
				ast_stack.pop();
				auto number = std::move(ast_stack.top().ast); ast_stack.pop();
				ast_stack.pop(); 
				ast_stack.push(SemValue{ std::make_unique<Stmt>(std::move(number)), "" });

				for (int i = 0; i < 3; ++i) state_stack.pop();
				state_stack.push(parsing_table[state_stack.top()][14]);
				break;
			}
			case -6: {
				std::string int_value = ast_stack.top().text; ast_stack.pop();
				ast_stack.push(SemValue{ std::make_unique<Number>(std::make_unique<IntConst>(int_value)), "" });
				state_stack.pop();
				state_stack.push(parsing_table[state_stack.top()][15]);
				break;
			}
			}
		}
		else {
			std::cerr << "parse error: unexpected token " << tokens[token_index].value << " at offset " << token_index << "\n";
			return;
		}
	}
}