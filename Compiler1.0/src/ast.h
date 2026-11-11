#pragma once
#include<iostream>
#include<cassert>
#include<string>
#include<cstdio>
#include<memory>

BaseAST* parse(FILE* file);

class BaseAST {
public:
    virtual void print() const = 0;
    virtual ~BaseAST() = default;
};

enum Type {
	Int;
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
		std::cout << value << std::endl;
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
	std::unique_ptr<BaseAST> num;
	void print() const override {
		std::cout << "Stmt: " << std::endl;
		std::cout << '{' << std::endl;
		if (num) {
			num->print();
		}
		std::cout << '}' << std::endl;
	}
	~Stmt() = default;
};

class Num : public BaseAST {
public:
	int value = 0;
	void print() const override {
		std::cout << "Num: " << value << std::endl;
	}
	~Num() = default;
};