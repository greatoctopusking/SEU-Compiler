#include "ast.h"

int main(int argc, char* argv[]) {
	if (argc < 2) { std::cerr << "usage: compiler <file>\n"; return 1; }
	std::FILE* file = std::fopen(argv[1], "rb");
	if (!file) { std::cerr << "cannot open " << argv[1] << '\n'; return 1; }
	
	std::unique_ptr<BaseAST> ast;
	parse(file, ast);
	if (ast == nullptr) {
		std::cerr << "parsing failed\n";
		return 1;
	}
	ast->print();

	return 0;
}