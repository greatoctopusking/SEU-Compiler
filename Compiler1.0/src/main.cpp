#include "ast.h"

int main() {
	std::string path;
	std::cout << "compile -> ";
	std::cin >> path;
	FILE* file = std::fopen(path.c_str(), "r");
	assert(file != nullptr);
	auto res = parse(file);
	res->print();
	return 0;
}