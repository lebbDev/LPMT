#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "ParsedProgram.h"
#include "Parser.h"
#include "LongNat.h"
using namespace std;

SymbolicToken Parser::transliterator(int symbol)
{
	SymbolicToken st;
	st.value = 0;

	if (('a' <= symbol && symbol <= 'z') || ('A' <= symbol && symbol <= 'Z'))
	{
		st.tokenClass = Letter;
		st.value = symbol;
	}
	else if ('0' <= symbol && symbol <= '9')
	{
		st.tokenClass = Digit;
		st.value = symbol - '0';
	}
	else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '%')
	{
		st.tokenClass = Arithmetic;
		st.value = symbol;
	}
	else if (symbol == '<' || symbol == '>' || symbol == '=' || symbol == '!')
	{
		st.tokenClass = Cmp;
		switch (symbol)
		{
		case '<': st.value = Less; break;
		case '>': st.value = Bigger; break;
		case '=': st.value = Equal; break;
		case '!': st.value = Negation; break;
		}
	}
	else if (symbol == ' ')
	{
		st.tokenClass = Space;
	}
	else if (symbol == '\n')
	{
		st.tokenClass = LF;
	}
	else if (symbol == ';')
	{
		st.tokenClass = Semicolon;
	}
	else if (symbol == EOF)
	{
		st.tokenClass = EofSymb;
	}
	else
	{
		st.tokenClass = Other;
	}
	return st;
}

void Parser::AddToken()
{
	tokens.push_back(Token(tokenClass, tokenValue, lineNumber));
}

void Parser::AddConstant()
{
	for (int i = 0; i < nameTable.size(); ++i)
	{
		if (nameTable[i].type != ObjectType::Constant) continue;

		if (*((int*)nameTable[i].namePointer) == number)
		{
			nameIndex = i;
			return;
		}
	}
	int* newConstant = new int(number);
	nameTable.push_back(ObjectName(ObjectType::Constant, newConstant));
	nameIndex = nameTable.size() - 1;
}

void Parser::AddLongConstant()
{
	for (int i = 0; i < nameTable.size(); ++i)
	{
		if (nameTable[i].type != ObjectType::LongConstant) continue;

		if (*((LongNat*)nameTable[i].namePointer) == longNat)
		{
			nameIndex = i;
			return;
		}
	}

	LongNat* new_long = new LongNat(longNat);
	nameTable.push_back(ObjectName(ObjectType::LongConstant, new_long));
	nameIndex = nameTable.size() - 1;
}

void Parser::AddVariable()
{
	for (int i = 0; i < nameTable.size(); ++i)
	{
		if (nameTable[i].type != ObjectType::Variable) continue;

		if (*((string*)nameTable[i].namePointer) == varName)
		{
			nameIndex = i;
			return;
		}
	}
	string* newVar = new string(varName);
	nameTable.push_back(ObjectName(ObjectType::Variable, newVar));
	nameIndex = nameTable.size() - 1;
}

Parser::Parser()
{
	lineNumber = 1;
	nameIndex = -1;

	for (int i = 0; i < STATES_COUNT; ++i)
	{
		for (int j = 0; j < SYMBOL_TOKEN_CLASS_COUNT; ++j)
		{
			table[i][j] = &Parser::J1a;
		}
	}

	table[st_A1][Letter] = &Parser::B1a;   table[st_F1][Digit] = &Parser::G1a;  table[st_A1][Arithmetic] = &Parser::C1a;
	table[st_A2][Letter] = &Parser::B1a;   table[st_F2][Digit] = &Parser::G1a;	table[st_A2][Arithmetic] = &Parser::C1a;
	table[st_B1][Letter] = &Parser::M1;    table[st_G1][Digit] = &Parser::G1b;	table[st_A1][Arithmetic] = &Parser::C1a;
	table[st_F1][Letter] = &Parser::H1a;   table[st_H1][Digit] = &Parser::H1b;	table[st_I1][Arithmetic] = &Parser::I1;
	table[st_F3][Letter] = &Parser::H1a;   table[st_I1][Digit] = &Parser::I1;	table[st_I2][Arithmetic] = &Parser::I2;
	table[st_H1][Letter] = &Parser::H1b;   table[st_I2][Digit] = &Parser::I2;	table[st_J1][Arithmetic] = &Parser::J1;
	table[st_I1][Letter] = &Parser::I1;	   table[st_J1][Digit] = &Parser::J1;
	table[st_I2][Letter] = &Parser::I2;
	table[st_J1][Letter] = &Parser::J1;

	table[st_A1][Cmp] = &Parser::D1a;	   table[st_A1][Space] = &Parser::A1;  table[st_A1][LF] = &Parser::A1b;
	table[st_A2][Cmp] = &Parser::D1a;      table[st_A2][Space] = &Parser::A2;  table[st_A2][LF] = &Parser::A2a;
	table[st_D1][Cmp] = &Parser::C1h;      table[st_C1][Space] = &Parser::C1;  table[st_B1][LF] = &Parser::A2f;
	table[st_I1][Cmp] = &Parser::I1;       table[st_D1][Space] = &Parser::C1g; table[st_C1][LF] = &Parser::A2a;
	table[st_I2][Cmp] = &Parser::I2;       table[st_E1][Space] = &Parser::F1;  table[st_D1][LF] = &Parser::A2e;
	table[st_J1][Cmp] = &Parser::J1;       table[st_E2][Space] = &Parser::F2;  table[st_E1][LF] = &Parser::A2f;

										   table[st_E3][Space] = &Parser::F3;  table[st_E2][LF] = &Parser::A2f;
	                                       table[st_F1][Space] = &Parser::F1;  table[st_E3][LF] = &Parser::A2f;
	                                       table[st_F2][Space] = &Parser::F2;  table[st_F1][LF] = &Parser::A2f;
	                                       table[st_F3][Space] = &Parser::F3;  table[st_F2][LF] = &Parser::A2f;
	                                       table[st_G1][Space] = &Parser::C1e; table[st_F3][LF] = &Parser::A2f;
	                                       table[st_H1][Space] = &Parser::C1f; table[st_G1][LF] = &Parser::A2c;
	                                       table[st_I1][Space] = &Parser::I1;  table[st_H1][LF] = &Parser::A2d;
	                                       table[st_I2][Space] = &Parser::I2;  table[st_I1][LF] = &Parser::A1a;
	                                       table[st_J1][Space] = &Parser::J1;  table[st_I2][LF] = &Parser::A2b;
																			   table[st_J1][LF] = &Parser::A2a;

	table[st_A1][Semicolon] = &Parser::I1a;	table[st_A2][EofSymb] = &Parser::Exit1;	table[st_I1][Other] = &Parser::I1;
	table[st_A2][Semicolon] = &Parser::I2a;	table[st_C1][EofSymb] = &Parser::Exit1;	table[st_I2][Other] = &Parser::I2;
	table[st_C1][Semicolon] = &Parser::I2a;	table[st_D1][EofSymb] = &Parser::Exit2;	table[st_J1][Other] = &Parser::J1;
	table[st_D1][Semicolon] = &Parser::I2d;	table[st_G1][EofSymb] = &Parser::Exit3;
	table[st_G1][Semicolon] = &Parser::I2b;	table[st_H1][EofSymb] = &Parser::Exit4;
	table[st_H1][Semicolon] = &Parser::I2c;	table[st_I2][EofSymb] = &Parser::Exit5;
	table[st_I1][Semicolon] = &Parser::I1;	table[st_J1][EofSymb] = &Parser::Exit1;
	table[st_I2][Semicolon] = &Parser::I2;
	table[st_J1][Semicolon] = &Parser::J1;

	//начальный вектор

	for (int i = 0; i < 26; ++i)
	{
		detectionTable.initVector[i] = -1;
	}

	detectionTable.initVector['e' - 'a'] = 0;
	detectionTable.initVector['j' - 'a'] = 2;
	detectionTable.initVector['p' - 'a'] = 5;
	detectionTable.initVector['r' - 'a'] = 10;
	detectionTable.initVector['w' - 'a'] = 13;

	for (int i = 0; i < 21; ++i)
	{
		detectionTable.table[i].alt = -1;
		detectionTable.table[i].procedure = &Parser::B1b;
	}

	detectionTable.table[0].letter = 'n';
	detectionTable.table[1].letter = 'd';											detectionTable.table[1].procedure = &Parser::C1b;

	detectionTable.table[2].letter = 'i';		detectionTable.table[2].alt = 3;	detectionTable.table[2].procedure = &Parser::E2a;

	detectionTable.table[3].letter = 'm';
	detectionTable.table[4].letter = 'p';											detectionTable.table[4].procedure = &Parser::E2b;

	detectionTable.table[5].letter = 'o';		detectionTable.table[5].alt = 7;
	detectionTable.table[6].letter = 'p';											detectionTable.table[6].procedure = &Parser::E3a;

	detectionTable.table[7].letter = 'u';
	detectionTable.table[8].letter = 's';
	detectionTable.table[9].letter = 'h';											detectionTable.table[9].procedure = &Parser::E1a;

	detectionTable.table[10].letter = 'e';      detectionTable.table[10].alt = 17;
	detectionTable.table[11].letter = 'a';
	detectionTable.table[12].letter = 'd';											detectionTable.table[12].procedure = &Parser::C1c;

	detectionTable.table[13].letter = 'r';
	detectionTable.table[14].letter = 'i';
	detectionTable.table[15].letter = 't';
	detectionTable.table[16].letter = 'e';                                          detectionTable.table[16].procedure = &Parser::C1d;

	detectionTable.table[17].letter = 'l';
	detectionTable.table[18].letter = 'o';
	detectionTable.table[19].letter = 'n';
	detectionTable.table[20].letter = 'g';                                          detectionTable.table[20].procedure = &Parser::C1l;
}

ParsedProgram Parser::run(const char* filename)
{
	ifstream in(filename);

	if (!in)
	{
		cout << "Не удалось открыть файл " << filename << endl;
		return ParsedProgram(filename);
	}

	int symbol;
	State state = st_A1;
	while (state != st_Stop)
	{
		symbol = in.get();
		symbToken = transliterator(symbol);
		state = (this->*table[state][symbToken.tokenClass])();
	}

	in.close();

	return ParsedProgram(std::move(tokens), std::move(nameTable), filename);
}