#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "ParsedProgram.h"
#include "LongNat.h"

enum State_STACK {
	ST_A1, ST_A2, ST_B1, ST_C1, ST_D1, ST_E1, ST_E2, ST_E3,
	ST_F1, ST_F2, ST_F3, ST_G1, ST_H1, ST_I1, ST_I2, ST_J1,
	ST_L1, ST_L2, ST_S1, ST_S2, ST_S3, ST_S4, ST_S5,
	ST_Stop
};

const int STATES_COUNT_ST = 24;

class Parser
{
	SymbolicToken_STACK symbToken;

	TokenClass_STACK tokenClass;	// класс лексемы языка
	int tokenValue;	// значение лексемы языка
	int lineNumber;	// номер строки

	int nameIndex;	// регистр указателя
	int number;		// регистр числа
	CmpValue_STACK cmpValue;	// регистр отношения
	std::string varName;
	LongNat longNat;

	std::vector <Token_STACK> tokens;	// вектор лексем
	std::vector <ObjectName_STACK> nameTable;

	static SymbolicToken_STACK transliterator(int symbol);
	void AddToken();
	void AddConstant();
	void AddLongConstant();
	void AddVariable();

	// Таблица процедур, таблица обнаружения, начальный вектор
	using parserProcedure = State_STACK(Parser::*)();
	parserProcedure table[STATES_COUNT_ST][SYMBOL_TOKEN_CLASS_COUNT_ST];

	struct DetectionTable {
		int initVector[26];

		struct DetectionTableLine {
			char letter;
			char alt;
			parserProcedure procedure;
		};
		DetectionTableLine table[23];
	};
	DetectionTable detectionTable;
	int detectionIndex;


	// Перечисление процедур
	State_STACK A1()
	{
		return ST_A1;
	}

	State_STACK A1a()
	{
		AddToken();
		lineNumber++;
		return ST_A1;
	}

	State_STACK A1b()
	{
		lineNumber++;
		return ST_A1;
	}

	State_STACK A2()
	{
		return ST_A2;
	}

	State_STACK A2a()
	{
		lineNumber++;
		return ST_A2;
	}

	State_STACK A2b()
	{
		AddToken();
		lineNumber++;
		return ST_A2;
	}

	State_STACK A2c()
	{
		AddConstant();
		tokenValue = nameIndex;
		AddToken();
		lineNumber++;
		return ST_A2;
	}

	State_STACK A2d()
	{
		AddVariable();
		tokenValue = nameIndex;
		AddToken();
		lineNumber++;
		return ST_A2;
	}

	State_STACK A2e()
	{
		if (cmpValue == Negation_ST) return J1a();
		tokenValue = cmpValue;
		AddToken();
		lineNumber++;
		return ST_A2;
	}

	State_STACK A2f()
	{
		tokenClass = Error;
		tokenValue = 0;
		AddToken();
		lineNumber++;
		return ST_A2;
	}

	State_STACK A2g()
	{
		AddLongConstant();
		tokenValue = nameIndex;
		AddToken();
		lineNumber++;
		return ST_A2;
	}

	State_STACK A2h()
	{
		tokenValue = number;
		AddToken();
		lineNumber++;
		return ST_A2;
	}

	State_STACK A2i()
	{
		tokenValue = longNat;
		AddToken();
		lineNumber++;
		return ST_A2;
	}


	State_STACK B1()
	{
		return ST_B1;
	}

	State_STACK B1a()
	{
		if ('A' <= symbToken.value && symbToken.value <= 'Z') return J1a();

		detectionIndex = detectionTable.initVector[symbToken.value - 'a'];
		if (detectionIndex == -1) return J1a();
		return ST_B1;
	}

	State_STACK B1b()
	{
		detectionIndex++;
		return ST_B1;
	}

	State_STACK C1()
	{
		return ST_C1;
	}

	State_STACK C1a()
	{
		tokenClass = ArithmeticOp;
		tokenValue = symbToken.value;
		AddToken();
		return ST_C1;
	}

	State_STACK C1b()
	{
		tokenClass = End;
		tokenValue = 0;
		AddToken();
		return ST_C1;
	}

	State_STACK C1c()
	{
		tokenClass = Read;
		tokenValue = 0;
		AddToken();
		return ST_C1;
	}

	State_STACK C1d()
	{
		tokenClass = Write;
		tokenValue = 0;
		AddToken();
		return ST_C1;
	}

	State_STACK C1e()
	{
		AddConstant();
		tokenValue = nameIndex;
		AddToken();
		return ST_C1;
	}

	State_STACK C1f()
	{
		AddVariable();
		tokenValue = nameIndex;
		AddToken();
		return ST_C1;
	}

	State_STACK C1g()
	{
		if (cmpValue == Negation_ST) return J1a();
		AddToken();
		return ST_C1;
	}

	State_STACK C1h()
	{
		if (cmpValue == Equal_ST) return J1a();
		if (symbToken.value == Equal_ST)
		{
			tokenValue = cmpValue + symbToken.value;
			AddToken();
		}
		else
			return J1a();
		return ST_C1;
	}

	State_STACK C1l()
	{
		tokenClass = Rlong;
		tokenValue = 0;
		AddToken();
		return ST_C1;
	}

	State_STACK C1m()
	{
		AddLongConstant();
		tokenValue = nameIndex;
		AddToken();
		return ST_C1;
	}

	State_STACK C1n()
	{
		tokenClass = DigitVar;
		return ST_S1;
	}

	State_STACK D1()
	{
		return ST_D1;
	}

	State_STACK D1a()
	{
		tokenClass = CmpOp;
		tokenValue = cmpValue = (CmpValue_STACK)symbToken.value;
		return ST_D1;
	}

	State_STACK E1()
	{
		return ST_E1;
	}

	State_STACK E1a()
	{
		tokenClass = Push;
		return ST_E1;
	}

	State_STACK E2()
	{
		return ST_E2;
	}

	State_STACK E2a()
	{
		tokenClass = Ji;
		return ST_E2;
	}

	State_STACK E2b()
	{
		tokenClass = Jmp;
		return ST_E2;
	}

	State_STACK E3()
	{
		return ST_E3;
	}

	State_STACK E3a()
	{
		tokenClass = Pop;
		return ST_E3;
	}

	State_STACK F1()
	{
		return ST_F1;
	}

	State_STACK F2()
	{
		return ST_F2;
	}

	State_STACK F3()
	{
		return ST_F3;
	}
	// Для обычных чисел
	State_STACK G1()
	{
		return ST_G1;
	}
	
	State_STACK G1a()
	{
		number = symbToken.value;
		return ST_G1;
	}

	State_STACK G1b()
	{
		number = 10 * number + symbToken.value;
		return ST_G1;
	}
	// Для переменных
	State_STACK H1()
	{
		return ST_H1;
	}

	State_STACK H1a()
	{
		varName = symbToken.value;
		return ST_H1;
	}

	State_STACK H1b()
	{
		varName += symbToken.value;
		return ST_H1;
	}
	// Для коментариев
	State_STACK I1()
	{
		return ST_I1;
	}

	State_STACK I1a()
	{
		tokenClass = Comment;
		tokenValue = 0;
		return ST_I1;
	}

	State_STACK I2()
	{
		return ST_I2;
	}

	State_STACK I2a()
	{
		tokenClass = Comment;
		tokenValue = 0;
		return ST_I2;
	}

	State_STACK I2b()
	{
		AddConstant();
		tokenValue = nameIndex;
		AddToken();
		tokenClass = Comment;
		tokenValue = 0;
		return ST_I2;
	}

	State_STACK I2c()
	{
		AddVariable();
		tokenValue = nameIndex;
		AddToken();
		tokenClass = Comment;
		tokenValue = 0;
		return ST_I2;
	}

	State_STACK I2d()
	{
		if (cmpValue == Negation_ST) return J1a();
		tokenValue = cmpValue;
		AddToken();
		return ST_I2;
	}
	// Для считывания длин. коснтант
	State_STACK L1()
	{
		return ST_L1;
	}

	State_STACK L1a()
	{
		longNat = (LongNat)symbToken.value;
		return ST_L2;
	}

	State_STACK L2()
	{
		return ST_L2;
	}

	State_STACK L2b()
	{
		longNat = longNat * (LongNat)10 + (LongNat)symbToken.value;
		return ST_L2;
	}
	// Для операции digit со своим типом данных
	
	State_STACK S1()
	{
		return ST_S1;
	}
	
	State_STACK S2()
	{
		return ST_S2;
	}

	State_STACK S2a()
	{
		varName = symbToken.value;
		return ST_S2;
	}

	State_STACK S2b()
	{
		varName += symbToken.value;
		return ST_S2;
	}

	State_STACK S2c()
	{
		AddVariable();
		tokenValue = nameIndex;
		AddToken();

		tokenClass = DigitIndex;
		return ST_S3;
	}

	State_STACK S3()
	{
		return ST_S3;
	}

	State_STACK S4()
	{
		return ST_S4;
	}

	State_STACK S4a()
	{
		number = symbToken.value;
		return ST_S4;
	}

	State_STACK S4b()
	{
		number = 10 * number + symbToken.value;
		return ST_S4;
	}

	State_STACK S4c()
	{
		tokenValue = number;
		AddToken();
		return ST_A2;
	}

	State_STACK S5()
	{
		return ST_S5;
	}

	State_STACK S5a()
	{
		longNat = (LongNat)symbToken.value;
		return ST_S5;
	}

	State_STACK S5b()
	{
		longNat = longNat * (LongNat)10 + (LongNat)symbToken.value;
		return ST_S5;
	}

	State_STACK S5c()
	{
		tokenValue = longNat;
		AddToken();
		return ST_A2;
	}

	State_STACK M1()
	{
		while (true)
		{
			if (detectionTable.table[detectionIndex].letter == symbToken.value) return (this->*detectionTable.table[detectionIndex].procedure)();

			detectionIndex = detectionTable.table[detectionIndex].alt;
			if (detectionIndex == -1) return J1a();
		}
	}

	State_STACK Exit1()
	{
		tokenClass = Eof;
		tokenValue = 0;
		AddToken();
		return ST_Stop;
	}

	State_STACK Exit2()
	{
		if (cmpValue == Negation_ST) return J1a();

		tokenValue = cmpValue;
		AddToken();
		tokenClass = Eof;
		tokenValue = 0;
		AddToken();
		return ST_Stop;
	}

	State_STACK Exit3()
	{
		AddConstant();
		tokenValue = nameIndex;
		AddToken();
		tokenClass = Eof;
		tokenValue = 0;
		AddToken();
		return ST_Stop;
	}

	State_STACK Exit4()
	{
		AddVariable();
		tokenValue = nameIndex;
		AddToken();
		tokenClass = Eof;
		tokenValue = 0;
		AddToken();
		return ST_Stop;
	}

	State_STACK Exit5()
	{
		AddToken();
		tokenClass = Eof;
		tokenValue = 0;
		AddToken();
		return ST_Stop;
	}

	State_STACK Exit6()
	{
		AddLongConstant();
		tokenValue = nameIndex;
		AddToken();
		tokenClass = Eof;
		tokenValue = 0;
		AddToken();
		return ST_Stop;
	}

	State_STACK J1()
	{
		return ST_J1;
	}

	State_STACK J1a()
	{
		tokenClass = Error;
		tokenValue = 0;
		AddToken();
		return ST_J1;
	}

public:
	Parser();
	ParsedProgram run(const char* filename);
};