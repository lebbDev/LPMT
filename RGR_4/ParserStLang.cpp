#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "ParsedProgram.h"
#include "ParserStLang.h"
using namespace std;

SymbolicToken_STACK Parser::transliterator(int symbol)
{
	SymbolicToken_STACK st;
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
		case '<': st.value = Less_ST; break;
		case '>': st.value = Bigger_ST; break;
		case '=': st.value = Equal_ST; break;
		case '!': st.value = Negation_ST; break;
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
	else if (symbol == '$')
	{
		st.tokenClass = Dollar;
	}
	else if (symbol == ',')
	{
		st.tokenClass = Comma;
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
	tokens.push_back(Token_STACK(tokenClass, tokenValue, lineNumber));
}

void Parser::AddConstant()
{
	for (int i = 0; i < nameTable.size(); ++i)
	{
		if (nameTable[i].type != ObjectType_STACK::Constant) continue;

		if (*((int*)nameTable[i].namePointer) == number)
		{
			nameIndex = i;
			return;
		}
	}
	int* newConstant = new int(number);
	nameTable.push_back(ObjectName_STACK(ObjectType_STACK::Constant, newConstant));
	nameIndex = nameTable.size() - 1;
}

void Parser::AddLongConstant()
{
	for (int i = 0; i < nameTable.size(); ++i)
	{
		if (nameTable[i].type != ObjectType_STACK::LongConstant) continue;

		if (*((LongNat*)nameTable[i].namePointer) == longNat)
		{
			nameIndex = i;
			return;
		}
	}

	LongNat* new_long = new LongNat(longNat);
	nameTable.push_back(ObjectName_STACK(ObjectType_STACK::LongConstant, new_long));
	nameIndex = nameTable.size() - 1;
}

void Parser::AddVariable()
{
	for (int i = 0; i < nameTable.size(); ++i)
	{
		if (nameTable[i].type != ObjectType_STACK::Variable) continue;

		if (*((string*)nameTable[i].namePointer) == varName)
		{
			nameIndex = i;
			return;
		}
	}
	string* newVar = new string(varName);
	nameTable.push_back(ObjectName_STACK(ObjectType_STACK::Variable, newVar));
	nameIndex = nameTable.size() - 1;
}

Parser::Parser()
{
	lineNumber = 1;
	nameIndex = -1;

	for (int i = 0; i < STATES_COUNT_ST; ++i)
	{
		for (int j = 0; j < SYMBOL_TOKEN_CLASS_COUNT_ST; ++j)
		{
			table[i][j] = &Parser::J1a;
		}
	}

	table[ST_A1][Letter] = &Parser::B1a;   table[ST_F1][Digit] = &Parser::G1a;  table[ST_A1][Arithmetic] = &Parser::C1a;
	table[ST_A2][Letter] = &Parser::B1a;   table[ST_F2][Digit] = &Parser::G1a;	table[ST_A2][Arithmetic] = &Parser::C1a;
	table[ST_B1][Letter] = &Parser::M1;    table[ST_G1][Digit] = &Parser::G1b;	table[ST_A1][Arithmetic] = &Parser::C1a;
	table[ST_F1][Letter] = &Parser::H1a;   table[ST_H1][Digit] = &Parser::H1b;	table[ST_I1][Arithmetic] = &Parser::I1;
	table[ST_F3][Letter] = &Parser::H1a;   table[ST_I1][Digit] = &Parser::I1;	table[ST_I2][Arithmetic] = &Parser::I2;
	table[ST_H1][Letter] = &Parser::H1b;   table[ST_I2][Digit] = &Parser::I2;	table[ST_J1][Arithmetic] = &Parser::J1;
	table[ST_I1][Letter] = &Parser::I1;	   table[ST_J1][Digit] = &Parser::J1;
	table[ST_I2][Letter] = &Parser::I2;	   table[ST_L1][Digit] = &Parser::L1a;
	table[ST_J1][Letter] = &Parser::J1;	   table[ST_L2][Digit] = &Parser::L2b;
	table[ST_S1][Letter] = &Parser::S2a;   table[ST_S3][Digit] = &Parser::S4a;
	table[ST_S2][Letter] = &Parser::S2b;   table[ST_S2][Digit] = &Parser::S2b;

	table[ST_A1][Cmp] = &Parser::D1a;	   table[ST_A1][Space] = &Parser::A1;  table[ST_A1][LF] = &Parser::A1b;
	table[ST_A2][Cmp] = &Parser::D1a;      table[ST_A2][Space] = &Parser::A2;  table[ST_A2][LF] = &Parser::A2a;
	table[ST_D1][Cmp] = &Parser::C1h;      table[ST_C1][Space] = &Parser::C1;  table[ST_B1][LF] = &Parser::A2f;
	table[ST_I1][Cmp] = &Parser::I1;       table[ST_D1][Space] = &Parser::C1g; table[ST_C1][LF] = &Parser::A2a;
	table[ST_I2][Cmp] = &Parser::I2;       table[ST_E1][Space] = &Parser::F1;  table[ST_D1][LF] = &Parser::A2e;
	table[ST_J1][Cmp] = &Parser::J1;       table[ST_E2][Space] = &Parser::F2;  table[ST_E1][LF] = &Parser::A2f;

										   table[ST_E3][Space] = &Parser::F3;  table[ST_E2][LF] = &Parser::A2f;
	                                       table[ST_F1][Space] = &Parser::F1;  table[ST_E3][LF] = &Parser::A2f;
	                                       table[ST_F2][Space] = &Parser::F2;  table[ST_F1][LF] = &Parser::A2f;
	                                       table[ST_F3][Space] = &Parser::F3;  table[ST_F2][LF] = &Parser::A2f;
	                                       table[ST_G1][Space] = &Parser::C1e; table[ST_F3][LF] = &Parser::A2f;
	                                       table[ST_H1][Space] = &Parser::C1f; table[ST_G1][LF] = &Parser::A2c;
	                                       table[ST_I1][Space] = &Parser::I1;  table[ST_H1][LF] = &Parser::A2d;
	                                       table[ST_I2][Space] = &Parser::I2;  table[ST_I1][LF] = &Parser::A1a;
	                                       table[ST_J1][Space] = &Parser::J1;  table[ST_I2][LF] = &Parser::A2b;
										   table[ST_L1][Space] = &Parser::C1m; table[ST_J1][LF] = &Parser::A2a;
										   table[ST_L2][Space] = &Parser::C1m; table[ST_L1][LF] = &Parser::A2g;
										   table[ST_S1][Space] = &Parser::S1;  table[ST_L2][LF] = &Parser::A2g;
										   table[ST_S3][Space] = &Parser::S3;  table[ST_S4][LF] = &Parser::A2h;
										   table[ST_S4][Space] = &Parser::S4c; table[ST_S5][LF] = &Parser::A2i;
										   table[ST_S5][Space] = &Parser::S5c;

	table[ST_A1][Semicolon] = &Parser::I1a;	table[ST_A2][EofSymb] = &Parser::Exit1;	table[ST_I1][Other] = &Parser::I1;
	table[ST_A2][Semicolon] = &Parser::I2a;	table[ST_C1][EofSymb] = &Parser::Exit1;	table[ST_I2][Other] = &Parser::I2;
	table[ST_C1][Semicolon] = &Parser::I2a;	table[ST_D1][EofSymb] = &Parser::Exit2;	table[ST_J1][Other] = &Parser::J1;
	table[ST_D1][Semicolon] = &Parser::I2d;	table[ST_G1][EofSymb] = &Parser::Exit3;
	table[ST_G1][Semicolon] = &Parser::I2b;	table[ST_H1][EofSymb] = &Parser::Exit4;
	table[ST_H1][Semicolon] = &Parser::I2c;	table[ST_I2][EofSymb] = &Parser::Exit5;
	table[ST_I1][Semicolon] = &Parser::I1;	table[ST_J1][EofSymb] = &Parser::Exit1;
	table[ST_I2][Semicolon] = &Parser::I2;	table[ST_L1][EofSymb] = &Parser::Exit6;
	table[ST_J1][Semicolon] = &Parser::J1;	table[ST_L2][EofSymb] = &Parser::Exit6;

	table[ST_F1][Dollar] = &Parser::L1; 
	table[ST_I1][Dollar] = &Parser::I1;
	table[ST_I2][Dollar] = &Parser::I2;
	table[ST_J1][Dollar] = &Parser::J1;
	table[ST_S3][Dollar] = &Parser::S5a;

	table[ST_S2][Comma] = &Parser::S2c;

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
	detectionTable.initVector['d' - 'a'] = 21;

	for (int i = 0; i < 23; ++i)
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

	detectionTable.table[21].letter = 'i';
	detectionTable.table[22].letter = 'g';											detectionTable.table[22].procedure = &Parser::C1n;
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
	State_STACK state = ST_A1;
	while (state != ST_Stop)
	{
		symbol = in.get();
		symbToken = transliterator(symbol);
		state = (this->*table[state][symbToken.tokenClass])();
	}

	in.close();

	return ParsedProgram(std::move(tokens), std::move(nameTable), filename);
}