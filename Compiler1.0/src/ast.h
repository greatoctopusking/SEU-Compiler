#pragma once
#include<iostream>
#include<cassert>
#include<string>
#include<cstdio>
#include<memory>
#include<vector>
#include<cctype>

enum TokenType {
	KW_RETURN,
	KW_INT,
	T_IDENT,
	T_OPEN_PAREN,
	T_CLOSE_PAREN,
	T_OPEN_CURLY,
	T_CLOSE_CURLY,
	T_INT_CONST,
	T_SEMI,
	T_EOF,
	UNKNOWN
};

const char* tokenTypeToString(TokenType type);

struct Token {
	TokenType type;
	std::string value;
};

class BaseAST {
public:
    virtual void print(int depth = 0) const = 0;
    virtual ~BaseAST() = default;
};

void parse(FILE* file, std::unique_ptr<BaseAST>& ast);

void lex(FILE* file, std::vector<Token>& tokens);

enum Type {
	Int,
	Void
};

class CompUnit : public BaseAST {
public:
	std::unique_ptr<BaseAST> funcdef;

	CompUnit(std::unique_ptr<BaseAST> funcdef) : funcdef(std::move(funcdef)) {}

	void print(int depth) const override {
		std::cout << std::string(depth * 2, ' ') << "CompUnit: " << std::endl;
		std::cout << std::string(depth * 2, ' ') << '{' << std::endl;
		if (funcdef) {
			funcdef->print(depth + 1);
		}
		std::cout << std::string(depth * 2, ' ') << '}' << std::endl;
	}
	~CompUnit() = default;
};

class FuncDef : public BaseAST {
public:
	std::unique_ptr<BaseAST> functype;
	std::unique_ptr<BaseAST> ident;
	std::unique_ptr<BaseAST> block;

	FuncDef(std::unique_ptr<BaseAST> functype, std::unique_ptr<BaseAST> ident, std::unique_ptr<BaseAST> block)
		: functype(std::move(functype)), ident(std::move(ident)), block(std::move(block)) {}

	void print(int depth) const override {
		std::cout << std::string(depth * 2, ' ') << "FuncDef: " << std::endl;
		std::cout << std::string(depth * 2, ' ') << '{' << std::endl;
		if (functype) {
			functype->print(depth + 1);
		}
		if (ident) {
			ident->print(depth + 1);
		}
		if (block) {
			block->print(depth + 1);
		}
		std::cout << std::string(depth * 2, ' ') << '}' << std::endl;
	}
	~FuncDef() = default;
};


class FuncType : public BaseAST {
public:
	Type value = Type::Int;
	FuncType(std::string v) {
		if (v == "int") {
			value = Type::Int;
		}
		else if (v == "void") {
			value = Type::Void;
		}
		else {
			assert(false && "Invalid function type");
		}
	}
	void print(int depth) const override {
		std::cout << std::string(depth * 2, ' ') << "FuncType: ";
		switch (value) {
		case Type::Int: 
			std::cout << "int" << std::endl; break;
		}
	}
	~FuncType() = default;
};

class Ident : public BaseAST {
public:
	std::string value;
	Ident(std::string v) : value(v) {}
	void print(int depth) const override {
		std::cout << std::string(depth * 2, ' ') << "Ident: ";
		std::cout << value << std::endl;
	}
	~Ident() = default;
};

class Block : public BaseAST {
public:
	std::unique_ptr<BaseAST> stmt;
	Block(std::unique_ptr<BaseAST> stmt) : stmt(std::move(stmt)) {}
	void print(int depth) const override {
		std::cout << std::string(depth * 2, ' ') << "Block: " << std::endl;
		std::cout << std::string(depth * 2, ' ') << '{' << std::endl;
		if (stmt) {
			stmt->print(depth + 1);
		}
		std::cout << std::string(depth * 2, ' ') << '}' << std::endl;
	}
	~Block() = default;
};

class Stmt : public BaseAST {
public:
	std::unique_ptr<BaseAST> number;
	Stmt(std::unique_ptr<BaseAST> number) : number(std::move(number)) {}
	void print(int depth) const override {
		std::cout << std::string(depth * 2, ' ') << "Stmt: " << std::endl;
		std::cout << std::string(depth * 2, ' ') << '{' << std::endl;
		if (number) {
			number->print(depth + 1);
		}
		std::cout << std::string(depth * 2, ' ') << '}' << std::endl;
	}
	~Stmt() = default;
};

class Number : public BaseAST {
public:
	std::unique_ptr<BaseAST> int_const;
	Number(std::unique_ptr<BaseAST> int_const) : int_const(std::move(int_const)) {}
	void print(int depth) const override {
		std::cout << std::string(depth * 2, ' ') << "Number: " << std::endl;
		std::cout << std::string(depth * 2, ' ') << '{' << std::endl;
		if (int_const) {
			int_const->print(depth + 1);
		}
		std::cout << std::string(depth * 2, ' ') << '}' << std::endl;
	}
	~Number() = default;
};

class IntConst :public BaseAST {
public:
	int value = 0;
	IntConst(std::string v) {
		try {
			value = std::stoi(v);
		} catch (const std::exception&) {
			value = 0;
		}
	}
	void print(int depth) const override {
		std::cout << std::string(depth * 2, ' ') << "IntConst: " << value << std::endl;
	}
	~IntConst() = default;
};