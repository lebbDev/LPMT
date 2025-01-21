#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "ParsedProgramLex.h"

enum State {
	st_A1, st_A2, st_B1, st_B2, st_B3, st_C1, st_D1, st_D2, st_E1,
	st_F1, st_F2, st_G1, st_G2, st_H1, st_I1, st_I2, st_P1,
	st_Err, st_Stop
};

const int STATES_COUNT = 18;

class Lexer
{
	SymbolicToken symbToken;

	TokenClass tokenClass;	// класс лексемы языка
	int tokenValue;	// значение лексемы языка
	int lineNumber;	// номер строки

	int nameIndex;	// регистр указателя
	int number;		// регистр числа
	CmpValue cmpValue;	// регистр отношения

	std::string varName;
	std::string mark;
	LongNat longNat;

public:
	std::vector <Token> tokens;	// вектор лексем
	std::vector <ObjectName> nameTable; // таблица имен

private:
	static SymbolicToken transliterator(int symbol);
	void AddToken();
	void AddConstant();
	void AddLongConstant();
	void AddVariable();
	void AddMark();

	// Таблица процедур, таблица обнаружения, начальный вектор
	using parserProcedure = State(Lexer::*)();
	parserProcedure table[STATES_COUNT][SYMBOL_TOKEN_CLASS_COUNT];

	struct DetectionTable {
		int initVector[26];

		struct DetectionTableLine {
			char letter;
			char alt;
			parserProcedure procedure;
		};
		DetectionTableLine table[65];
	};
	DetectionTable detectionTable;
	int detectionIndex;


	// Перечисление процедур
	State A1()
	{
		return State::st_A1;
	}

	State A1a()
	{
		lineNumber++;
		return State::st_A1;
	}

	State A1b()
	{
		tokenClass = Error_T;
		tokenValue = 0;
		AddToken();
		lineNumber++;
		return State::st_A1;
	}

	State A1c()
	{
		AddVariable();
		tokenValue = nameIndex;
		tokenClass = Variable_T;
		AddToken();
		lineNumber++;
		return State::st_A1;
	}

	State A1d()
	{
		AddConstant();
		tokenValue = nameIndex;
		tokenClass = Constant_T;
		AddToken();
		lineNumber++;
		return State::st_A1;
	}

	State A1e()
	{
		AddLongConstant();
		tokenValue = nameIndex;
		tokenClass = LongConstant_T;
		AddToken();
		lineNumber++;
		return State::st_A1;
	}

	State A1f()
	{
		AddMark();
		tokenValue = nameIndex;
		tokenClass = Label_T;
		AddToken();
		lineNumber++;
		return State::st_A1;
	}

	State A1g()
	{
		AddToken();
		lineNumber++;
		return State::st_A1;
	}

	State A1h()
	{
		switch (tokenClass)
		{
		case Goto_T: Err2(); break;
		case Next_T: Err2(); break;
		case Case_T: Err2(); break;
		default:
		{
			AddToken();
			lineNumber++;
			return State::st_A1;
		}
		}
	}

	State A2()
	{
		return State::st_A2;
	}

	State A2a()
	{
		tokenClass = Semicolon_T;
		tokenValue = 0;
		return State::st_A2;
	}

	State A2b()
	{
		tokenClass = Comma_T;
		tokenValue = 0;
		return State::st_A2;
	}

	State A2c()
	{
		tokenClass = LBracket_T;
		tokenValue = 0;
		return State::st_A2;
	}

	State A2d()
	{
		tokenClass = RBracket_T;
		tokenValue = 0;
		return State::st_A2;
	}

	State A2e()
	{
		AddVariable();
		tokenValue = nameIndex;
		tokenClass = Variable_T;
		AddToken();

		tokenClass = Comma_T;
		tokenValue = 0;
		return State::st_A2;
	}

	State A2f()
	{
		if (tokenClass == Int_T || tokenClass == Long_T)
		{
			AddToken();

			tokenClass = Comma_T;
			tokenValue = 0;
			return State::st_A2;
		}

		Err1();
	}

	State B1()
	{
		return State::st_B1;
	}

	State B1a()
	{
		varName = symbToken.value;

		if ('A' <= symbToken.value && symbToken.value <= 'Z') return B2();

		detectionIndex = detectionTable.initVector[symbToken.value - 'a'];
		if (detectionIndex == -1) return B2();
		return State::st_B1;
	}

	State B1b()
	{
		detectionIndex++;
		return State::st_B1;
	}

	State B2()
	{
		return State::st_B2;
	}

	State B2a()
	{
		if (symbToken.value < 10) varName += (symbToken.value + '0');
		else varName += symbToken.value;
		return State::st_B2;
	}

	State B3()
	{
		return State::st_B3;
	}

	State B3a()
	{
		tokenClass = As_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3b()
	{
		tokenClass = Cin_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3c()
	{
		tokenClass = Cout_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3d()
	{
		tokenClass = Declare_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3e()
	{
		tokenClass = Else_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3f()
	{
		tokenClass = Goto_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3g()
	{
		tokenClass = Rem_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3h()
	{
		tokenClass = Set_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3i()
	{
		tokenClass = To_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3j()
	{
		tokenClass = Int_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3k()
	{
		tokenClass = Long_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3l()
	{
		tokenClass = If_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3m()
	{
		tokenClass = For_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3n()
	{
		tokenClass = Adding_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3o()
	{
		tokenClass = Next_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3p()
	{
		tokenClass = Switch_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3r()
	{
		tokenClass = Case_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3s()
	{
		tokenClass = Break_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3t()
	{
		tokenClass = Default_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3u()
	{
		tokenClass = End_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3v()
	{
		tokenClass = Fail_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State B3w()
	{
		tokenClass = Digit_T;
		tokenValue = 0;
		return State::st_B3;
	}

	State C1()
	{
		return State::st_C1;
	}

	State C1a()
	{
		number = symbToken.value;
		return State::st_C1;
	}

	State C1b()
	{
		number = 10 * number + symbToken.value;
		return State::st_C1;
	}

	State D1()
	{
		return State::st_D1;
	}

	State D1a()
	{
		tokenValue = cmpValue = (CmpValue)symbToken.value;

		tokenClass = CmpOp_T;

		if (cmpValue == Equal)
			tokenClass = Assignment_T;

		return State::st_D1;
	}

	State D2()
	{
		return State::st_D2;
	}

	State D2a()
	{
		if (symbToken.value == Equal)
		{
			switch (cmpValue)
			{
			case Negation: tokenValue = NotEqual; break;
			case Equal: tokenValue = Equal; break;
			default: tokenValue = cmpValue + symbToken.value; break;
			}
		}
		else
			return Err1();

		tokenClass = CmpOp_T;
		return State::st_D2;
	}

	State E1()
	{
		return State::st_E1;
	}

	State E1a()
	{
		tokenClass = ArithmeticOp_T;
		tokenValue = symbToken.value;
		return State::st_E1;
	}

	State F1()
	{
		return State::st_F1;
	}

	State F2()
	{
		return State::st_F2;
	}

	State F2a()
	{
		longNat = symbToken.value;
		return State::st_F2;
	}

	State F2b()
	{
		longNat = (LongNat)10 * longNat + (LongNat)symbToken.value;
		return State::st_F2;
	}

	State G1()
	{
		return State::st_G1;
	}

	State G1a()
	{
		tokenClass = Colon_T;
		tokenValue = 0;
		AddToken();

		return State::st_G1;
	}

	State G2()
	{
		return State::st_G2;
	}

	State G2a()
	{
		mark = symbToken.value;
		return State::st_G2;
	}

	State G2b()
	{
		if (symbToken.value < 10) mark += (symbToken.value + '0');
		else mark += symbToken.value;
		return State::st_G2;
	}

	State H1()
	{
		return State::st_H1;
	}

	State I1()
	{
		return State::st_I1;
	}

	State I1a()
	{
		if (symbToken.value == Bigger && tokenValue == '-')
		{
			tokenValue = 0;
			tokenClass = Output_T;
			return State::st_I2;
		}

		return Err1();
	}

	State I1b()
	{
		if (cmpValue == Less && symbToken.value == '-')
		{
			tokenValue = 0;
			tokenClass = Input_T;
			return State::st_I2;
		}

		return Err1();
	}

	State I2()
	{
		return State::st_I2;
	}

	State S1a()
	{
		AddVariable();
		tokenValue = nameIndex;
		tokenClass = Variable_T;
		AddToken();
		return State::st_A1;
	}

	State S1b()
	{
		AddConstant();
		tokenValue = nameIndex;
		tokenClass = Constant_T;
		AddToken();
		return State::st_A1;
	}

	State S1c()
	{
		AddLongConstant();
		tokenValue = nameIndex;
		tokenClass = LongConstant_T;
		AddToken();
		return State::st_A1;
	}

	State S1d()
	{
		AddMark();
		tokenValue = nameIndex;
		tokenClass = Label_T;
		AddToken();
		return State::st_A1;
	}

	State S1e()
	{
		switch (tokenClass)
		{
		case Goto_T: AddToken(); return State::st_G1;
		case Next_T: AddToken(); return State::st_G1;
		case Rem_T: AddToken(); return State::st_H1;
		default: AddToken(); return State::st_A1;
		}
	}

	State S1h()
	{
		if (tokenClass == Assignment_T)
			tokenValue = 0;

		AddToken();
		return State::st_A1;
	}

	State P1()
	{
		return State::st_P1;
	}

	State P1a()
	{
		if (tokenClass == Default_T)
		{
			AddToken();

			tokenValue = 0;
			tokenClass = Colon_T;
			return State::st_A2;
		}
		else return Err1();
	}

	State P1b()
	{
		AddConstant();

		tokenValue = nameIndex;
		tokenClass = Constant_T;
		AddToken();

		tokenValue = 0;
		tokenClass = Colon_T;
		return State::st_A2;
	}

	State P1c()
	{
		AddLongConstant();

		tokenValue = nameIndex;
		tokenClass = LongConstant_T;
		AddToken();

		tokenValue = 0;
		tokenClass = Colon_T;
		return State::st_A2;
	}

	State M1()
	{
		varName += symbToken.value;

		while (true)
		{
			if (detectionTable.table[detectionIndex].letter == symbToken.value) return (this->*detectionTable.table[detectionIndex].procedure)();

			detectionIndex = detectionTable.table[detectionIndex].alt;
			if (detectionIndex == -1) return B2();
		}
	}

	State Exit1()
	{
		tokenClass = Eof_T;
		tokenValue = 0;
		AddToken();
		return State::st_Stop;
	}

	State Exit2()
	{
		AddVariable();
		tokenValue = nameIndex;
		tokenClass = Variable_T;
		AddToken();

		tokenClass = Eof_T;
		tokenValue = 0;
		AddToken();
		return State::st_Stop;
	}

	State Exit3()
	{
		AddToken();

		tokenClass = Eof_T;
		tokenValue = 0;
		AddToken();
		return State::st_Stop;
	}

	State Exit4()
	{
		AddConstant();
		tokenValue = nameIndex;
		tokenClass = Constant_T;
		AddToken();

		tokenClass = Eof_T;
		tokenValue = 0;
		AddToken();
		return State::st_Stop;
	}

	State Exit5()
	{
		if (cmpValue == Negation) return Err1();
		if (cmpValue == Equal) tokenValue = 0;
		AddToken();

		tokenClass = Eof_T;
		tokenValue = 0;
		AddToken();
		return State::st_Stop;
	}

	State Exit6()
	{
		AddToken();

		tokenClass = Eof_T;
		tokenValue = 0;
		AddToken();
		return State::st_Stop;
	}

	State Exit7()
	{
		AddLongConstant();
		tokenValue = nameIndex;
		tokenClass = LongConstant_T;
		AddToken();

		tokenClass = Eof_T;
		tokenValue = 0;
		AddToken();
		return State::st_Stop;
	}

	State Exit8()
	{
		AddMark();
		tokenValue = nameIndex;
		tokenClass = Label_T;
		AddToken();

		tokenClass = Eof_T;
		tokenValue = 0;
		AddToken();
		return State::st_Stop;
	}

	State Exit9()
	{
		tokenClass = Error_T;
		tokenValue = 0;
		AddToken();

		tokenClass = Eof_T;
		tokenValue = 0;
		AddToken();
		return State::st_Stop;
	}

	State Err()
	{
		return State::st_Err;
	}

	State Err1()
	{
		tokenClass = Error_T;
		tokenValue = 0;
		AddToken();
		return State::st_Err;
	}

	State Err2()
	{
		tokenClass = Error_T;
		tokenValue = 0;
		AddToken();
		lineNumber++;
		return State::st_A1;
	}

public:
	Lexer();
	ParsedProgramLex run(std::string filename);
};