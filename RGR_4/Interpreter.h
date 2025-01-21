#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "Object.h"
#include "ParsedProgram.h"

class Interpreter
{
	std::vector<Object> stack;
	std::map<std::string, Object> variables;

	ParsedProgram program;
	int tokenIndex;
	bool interpreting;

	int digitVarIndex;

	void push(int nameIndex);
	void pop(int nameIndex);
	void jmp(int nameIndex);
	void ji(int nameIndex);
	void read();
	void rlong();
	void write();
	void end();
	void calculate(char operation);
	void compare(CmpValue_STACK operation);
	void digit(int nameIndex, int digitIndex);
	void skip();
	void error();
	void execute_token(Token_STACK token);

public:
	Interpreter(ParsedProgram&& _program) : program(std::move(_program)) {}

	void run();
};