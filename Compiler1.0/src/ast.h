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
    virtual void print() const = 0;
    virtual ~BaseAST() = default;
};

//void parse(FILE* file, BaseAST* ast);

void lex(FILE* file, std::vector<Token>& tokens);

enum Type {
	Int,
	Void
};

class CompUnit : public BaseAST {
public:
	std::unique_ptr<BaseAST> funcdef;
	void print() const override {
		std::cout << "CompUnit: " << std::endl;
		std::cout << '{' << std::endl;
		if (funcdef) {
			funcdef->print();
		}
		std::cout << '}' << std::endl;
	}
	~CompUnit() = default;
};

class FuncDef : public BaseAST {
public:
	std::unique_ptr<BaseAST> functype;
	std::unique_ptr<BaseAST> ident;
	std::unique_ptr<BaseAST> block;
	void print() const override {
		std::cout << "FuncDef: " << std::endl;
		std::cout << '{' << std::endl;
		if (functype) {
			functype->print();
		}
		if (ident) {
			ident->print();
		}
		if (block) {
			block->print();
		}
		std::cout << '}' << std::endl;
	}
	~FuncDef() = default;
};


class FuncType : public BaseAST {
public:
	Type value = Type::Int;
	void print() const override {
		std::cout << "FuncType: ";
		switch (value) {
		case Type::Int: 
			std::cout << "int"; break;
		}
	}
	~FuncType() = default;
};

class Ident : public BaseAST {
public:
	std::string value;
	void print() const override {
		std::cout << "Ident: ";
		std::cout << value << std::endl;
	}
	~Ident() = default;
};

class Block : public BaseAST {
public:
	std::unique_ptr<BaseAST> stmt;
	void print() const override {
		std::cout << "Block: " << std::endl;
		std::cout << '{' << std::endl;
		if (stmt) {
			stmt->print();
		}
		std::cout << '}' << std::endl;
	}
	~Block() = default;
};

class Stmt : public BaseAST {
public:
	std::unique_ptr<BaseAST> number;
	void print() const override {
		std::cout << "Stmt: " << std::endl;
		std::cout << '{' << std::endl;
		if (number) {
			number->print();
		}
		std::cout << '}' << std::endl;
	}
	~Stmt() = default;
};

class Number : public BaseAST {
public:
	std::unique_ptr<BaseAST> int_const;
	void print() const override {
		std::cout << "Number: " << std::endl;
		std::cout << '{' << std::endl;
		if (int_const) {
			int_const->print();
		}
		std::cout << '}' << std::endl;
	}
	~Number() = default;
};

class IntConst :public BaseAST {
public:
	int value = 0;
	void print()const override {
		std::cout << "IntConst: " << value << std::endl;
	}
	~IntConst() = default;
};