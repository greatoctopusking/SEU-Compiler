#include "ast.h"

int main(int argc, char* argv[]) {
	if (argc < 2) { std::cerr << "usage: compiler <file>\n"; return 1; }
	std::FILE* file = std::fopen(argv[1], "rb");
	if (!file) { std::cerr << "cannot open " << argv[1] << '\n'; return 1; }
	
	std::vector<Token> tokens;
	lex(file, tokens);
	fclose(file);
	for (const auto& token : tokens) {
		std::cout << "Token: " << tokenTypeToString(token.type) << " '" << token.value << "'\n";
	}

	return 0;
}