#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "ParsedProgram.h"
#include "LongNat.h"

enum State {
	st_A1, st_A2, st_B1, st_C1, st_D1, st_E1, st_E2, st_E3,
	st_F1, st_F2, st_F3, st_G1, st_H1, st_I1, st_I2, st_J1, st_Stop
};

const int STATES_COUNT = 16;

class Parser
{
	SymbolicToken symbToken;

	TokenClass tokenClass;	// класс лексемы языка
	int tokenValue;	// значение лексемы языка
	int lineNumber;	// номер строки

	int nameIndex;	// регистр указателя
	int number;		// регистр числа
	CmpValue cmpValue;	// регистр отношения
	std::string varName;
	LongNat longNat;

	std::vector <Token> tokens;	// вектор лексем
	std::vector <ObjectName> nameTable;

	static SymbolicToken transliterator(int symbol);
	void AddToken();
	void AddConstant();
	void AddLongConstant();
	void AddVariable();

	// Таблица процедур, таблица обнаружения, начальный вектор
	using parserProcedure = State(Parser::*)();
	parserProcedure table[STATES_COUNT][SYMBOL_TOKEN_CLASS_COUNT];

	struct DetectionTable {
		int initVector[26];

		struct DetectionTableLine {
			char letter;
			char alt;
			parserProcedure procedure;
		};
		DetectionTableLine table[21];
	};
	DetectionTable detectionTable;
	int detectionIndex;


	// Перечисление процедур
	State A1()
	{
		return st_A1;
	}

	State A1a()
	{
		AddToken();
		lineNumber++;
		return st_A1;
	}

	State A1b()
	{
		lineNumber++;
		return st_A1;
	}

	State A2()
	{
		return st_A2;
	}

	State A2a()
	{
		lineNumber++;
		return st_A2;
	}

	State A2b()
	{
		AddToken();
		lineNumber++;
		return st_A2;
	}

	State A2c()
	{
		AddConstant();
		tokenValue = nameIndex;
		AddToken();
		lineNumber++;
		return st_A2;
	}

	State A2d()
	{
		AddVariable();
		tokenValue = nameIndex;
		AddToken();
		lineNumber++;
		return st_A2;
	}

	State A2e()
	{
		if (cmpValue == Negation) return J1a();
		tokenValue = cmpValue;
		AddToken();
		lineNumber++;
		return st_A2;
	}

	State A2f()
	{
		tokenClass = Error;
		tokenValue = 0;
		AddToken();
		lineNumber++;
		return st_A2;
	}

	State B1()
	{
		return st_B1;
	}

	State B1a()
	{
		if ('A' <= symbToken.value && symbToken.value <= 'Z') return J1a();

		detectionIndex = detectionTable.initVector[symbToken.value - 'a'];
		if (detectionIndex == -1) return J1a();
		return st_B1;
	}

	State B1b()
	{
		detectionIndex++;
		return st_B1;
	}

	State C1()
	{
		return st_C1;
	}

	State C1a()
	{
		tokenClass = ArithmeticOp;
		tokenValue = symbToken.value;
		AddToken();
		return st_C1;
	}

	State C1b()
	{
		tokenClass = End;
		tokenValue = 0;
		AddToken();
		return st_C1;
	}

	State C1c()
	{
		tokenClass = Read;
		tokenValue = 0;
		AddToken();
		return st_C1;
	}

	State C1d()
	{
		tokenClass = Write;
		tokenValue = 0;
		AddToken();
		return st_C1;
	}

	State C1e()
	{
		AddConstant();
		tokenValue = nameIndex;
		AddToken();
		return st_C1;
	}

	State C1f()
	{
		AddVariable();
		tokenValue = nameIndex;
		AddToken();
		return st_C1;
	}

	State C1g()
	{
		if (cmpValue == Negation) return J1a();
		AddToken();
		return st_C1;
	}

	State C1h()
	{
		if (cmpValue == Equal) return J1a();
		if (symbToken.value == Equal)
		{
			tokenValue = cmpValue + symbToken.value;
			AddToken();
		}
		else
			return J1a();
		return st_C1;
	}

	State C1l()
	{
		tokenClass = Rlong;
		tokenValue = 0;
		AddToken();
		return st_C1;
	}

	State D1()
	{
		return st_D1;
	}

	State D1a()
	{
		tokenClass = CmpOp;
		tokenValue = cmpValue = (CmpValue)symbToken.value;
		return st_D1;
	}

	State E1()
	{
		return st_E1;
	}

	State E1a()
	{
		tokenClass = Push;
		return st_E1;
	}

	State E2()
	{
		return st_E2;
	}

	State E2a()
	{
		tokenClass = Ji;
		return st_E2;
	}

	State E2b()
	{
		tokenClass = Jmp;
		return st_E2;
	}

	State E3()
	{
		return st_E3;
	}

	State E3a()
	{
		tokenClass = Pop;
		return st_E3;
	}

	State F1()
	{
		return st_F1;
	}

	State F2()
	{
		return st_F2;
	}

	State F3()
	{
		return st_F3;
	}

	State G1()
	{
		return st_G1;
	}

	State G1a()
	{
		number = symbToken.value;
		return st_G1;
	}

	State G1b()
	{
		number = 10 * number + symbToken.value;
		return st_G1;
	}

	State H1()
	{
		return st_H1;
	}

	State H1a()
	{
		varName = symbToken.value;
		return st_H1;
	}

	State H1b()
	{
		varName += symbToken.value;
		return st_H1;
	}

	State I1()
	{
		return st_I1;
	}

	State I1a()
	{
		tokenClass = Comment;
		tokenValue = 0;
		return st_I1;
	}

	State I2()
	{
		return st_I2;
	}

	State I2a()
	{
		tokenClass = Comment;
		tokenValue = 0;
		return st_I2;
	}

	State I2b()
	{
		AddConstant();
		tokenValue = nameIndex;
		AddToken();
		tokenClass = Comment;
		tokenValue = 0;
		return st_I2;
	}

	State I2c()
	{
		AddVariable();
		tokenValue = nameIndex;
		AddToken();
		tokenClass = Comment;
		tokenValue = 0;
		return st_I2;
	}

	State I2d()
	{
		if (cmpValue == Negation) return J1a();
		tokenValue = cmpValue;
		AddToken();
		return st_I2;
	}

	State M1()
	{
		while (true)
		{
			if (detectionTable.table[detectionIndex].letter == symbToken.value) return (this->*detectionTable.table[detectionIndex].procedure)();

			detectionIndex = detectionTable.table[detectionIndex].alt;
			if (detectionIndex == -1) return J1a();
		}
	}

	State Exit1()
	{
		tokenClass = Eof;
		tokenValue = 0;
		AddToken();
		return st_Stop;
	}

	State Exit2()
	{
		if (cmpValue == Negation) return J1a();

		tokenValue = cmpValue;
		AddToken();
		tokenClass = Eof;
		tokenValue = 0;
		AddToken();
		return st_Stop;
	}

	State Exit3()
	{
		AddConstant();
		tokenValue = nameIndex;
		AddToken();
		tokenClass = Eof;
		tokenValue = 0;
		AddToken();
		return st_Stop;
	}

	State Exit4()
	{
		AddVariable();
		tokenValue = nameIndex;
		AddToken();
		tokenClass = Eof;
		tokenValue = 0;
		AddToken();
		return st_Stop;
	}

	State Exit5()
	{
		AddToken();
		tokenClass = Eof;
		tokenValue = 0;
		AddToken();
		return st_Stop;
	}

	State J1()
	{
		return st_J1;
	}

	State J1a()
	{
		tokenClass = Error;
		tokenValue = 0;
		AddToken();
		return st_J1;
	}

public:
	Parser();
	ParsedProgram run(const char* filename);
};