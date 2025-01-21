#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Lexer.h"
using namespace std;

SymbolicToken Lexer::transliterator(int symbol)
{
	SymbolicToken st;
	st.value = 0;

	if (('a' <= symbol && symbol <= 'z') || ('A' <= symbol && symbol <= 'Z'))
	{
		st.tokenClass = Letter_ST;
		st.value = symbol;
	}
	else if ('0' <= symbol && symbol <= '9')
	{
		st.tokenClass = Digit_ST;
		st.value = symbol - '0';
	}
	else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '%')
	{
		st.tokenClass = Arithmetic_ST;
		st.value = symbol;
	}
	else if (symbol == '<' || symbol == '>' || symbol == '=' || symbol == '!')
	{
		st.tokenClass = Cmp_ST;
		switch (symbol)
		{
		case '<': st.value = Less; break;
		case '>': st.value = Bigger; break;
		case '=': st.value = Equal; break;
		case '!': st.value = Negation; break;
		}
	}
	else if (symbol == ' ' || symbol == '\t')
	{
		st.tokenClass = Space_ST;
	}
	else if (symbol == '\n')
	{
		st.tokenClass = LF_ST;
	}
	else if (symbol == ';')
	{
		st.tokenClass = Semicolon_ST;
	}
	else if (symbol == ',')
	{
		st.tokenClass = Comma_ST;
	}
	else if (symbol == ':')
	{
		st.tokenClass = Colon_ST;
	}
	else if (symbol == '$')
	{
		st.tokenClass = Dollar_ST;
	}
	else if (symbol == '(')
	{
		st.tokenClass = LBracket_ST;
	}
	else if (symbol == ')')
	{
		st.tokenClass = RBracket_ST;
	}
	else if (symbol == EOF)
	{
		st.tokenClass = EofSymb_ST;
	}
	else
	{
		st.tokenClass = Other_ST;
	}
	return st;
}

void Lexer::AddToken()
{
	tokens.push_back(Token(tokenClass, tokenValue, lineNumber));
}

void Lexer::AddConstant()
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

void Lexer::AddLongConstant()
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

void Lexer::AddVariable()
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

void Lexer::AddMark()
{
	for (int i = 0; i < nameTable.size(); ++i)
	{
		if (nameTable[i].type != ObjectType::Label) continue;

		if (*((string*)nameTable[i].namePointer) == mark)
		{
			nameIndex = i;
			return;
		}
	}
	string* newMark = new string(mark);
	nameTable.push_back(ObjectName(ObjectType::Label, newMark));
	nameIndex = nameTable.size() - 1;
}

Lexer::Lexer()
{
	lineNumber = 1;
	nameIndex = -1;

	for (int i = 0; i < STATES_COUNT; ++i)
	{
		for (int j = 0; j < SYMBOL_TOKEN_CLASS_COUNT; ++j)
		{
			table[i][j] = &Lexer::Err1;
		}
	}

	table[st_A1][Letter_ST] = &Lexer::B1a;
	table[st_B1][Letter_ST] = &Lexer::M1;
	table[st_B2][Letter_ST] = &Lexer::B2a;
	table[st_B3][Letter_ST] = &Lexer::B2a;
	table[st_G1][Letter_ST] = &Lexer::G2a;
	table[st_G2][Letter_ST] = &Lexer::G2b;
	table[st_H1][Letter_ST] = &Lexer::H1;
	table[st_Err][Letter_ST] = &Lexer::Err;

	table[st_A1][Digit_ST] = &Lexer::C1a;
	table[st_B1][Digit_ST] = &Lexer::B2a;
	table[st_B2][Digit_ST] = &Lexer::B2a;
	table[st_B3][Digit_ST] = &Lexer::B2a;
	table[st_C1][Digit_ST] = &Lexer::C1b;
	table[st_F1][Digit_ST] = &Lexer::F2a;
	table[st_F2][Digit_ST] = &Lexer::F2b;
	table[st_G2][Digit_ST] = &Lexer::G2b;
	table[st_H1][Digit_ST] = &Lexer::H1;
	table[st_Err][Digit_ST] = &Lexer::Err;

	table[st_A1][Arithmetic_ST] = &Lexer::E1a;
	table[st_D1][Arithmetic_ST] = &Lexer::I1b;
	table[st_H1][Arithmetic_ST] = &Lexer::H1;
	table[st_Err][Arithmetic_ST] = &Lexer::Err;

	table[st_A1][Cmp_ST] = &Lexer::D1a;
	table[st_D1][Cmp_ST] = &Lexer::D2a;
	table[st_E1][Cmp_ST] = &Lexer::I1a;
	table[st_H1][Cmp_ST] = &Lexer::H1;
	table[st_I1][Cmp_ST] = &Lexer::I1a;
	table[st_Err][Cmp_ST] = &Lexer::Err;

	table[st_A1][Space_ST] = &Lexer::A1;
	table[st_A2][Space_ST] = &Lexer::S1h;
	table[st_B1][Space_ST] = &Lexer::S1a;
	table[st_B2][Space_ST] = &Lexer::S1a;
	table[st_B3][Space_ST] = &Lexer::S1e;
	table[st_C1][Space_ST] = &Lexer::S1b;
	table[st_D1][Space_ST] = &Lexer::S1h;
	table[st_D2][Space_ST] = &Lexer::S1h;
	table[st_E1][Space_ST] = &Lexer::S1h;
	table[st_F1][Space_ST] = &Lexer::S1c;
	table[st_F2][Space_ST] = &Lexer::S1c;
	table[st_G1][Space_ST] = &Lexer::S1d;
	table[st_G2][Space_ST] = &Lexer::S1d;
	table[st_H1][Space_ST] = &Lexer::H1;
	table[st_I2][Space_ST] = &Lexer::S1h;
	table[st_Err][Space_ST] = &Lexer::A1;

	table[st_A1][LF_ST] = &Lexer::A1a;
	table[st_A2][LF_ST] = &Lexer::A1g;
	table[st_B1][LF_ST] = &Lexer::A1c;
	table[st_B2][LF_ST] = &Lexer::A1c;
	table[st_B3][LF_ST] = &Lexer::A1h;
	table[st_C1][LF_ST] = &Lexer::A1d;
	table[st_D1][LF_ST] = &Lexer::A1g;
	table[st_D2][LF_ST] = &Lexer::A1g;
	table[st_E1][LF_ST] = &Lexer::A1g;
	table[st_F1][LF_ST] = &Lexer::A1e;
	table[st_F2][LF_ST] = &Lexer::A1e;
	table[st_G1][LF_ST] = &Lexer::A1f;
	table[st_G2][LF_ST] = &Lexer::A1f;
	table[st_H1][LF_ST] = &Lexer::A1a;
	table[st_I1][LF_ST] = &Lexer::A1b;
	table[st_I2][LF_ST] = &Lexer::A1g;
	table[st_Err][LF_ST] = &Lexer::A1a;

	table[st_A1][Semicolon_ST] = &Lexer::A2a;
	table[st_H1][Semicolon_ST] = &Lexer::H1;
	table[st_Err][Semicolon_ST] = &Lexer::Err;

	table[st_A1][Comma_ST] = &Lexer::A2b;
	table[st_B1][Comma_ST] = &Lexer::A2e;
	table[st_B2][Comma_ST] = &Lexer::A2e;
	table[st_B3][Comma_ST] = &Lexer::A2f;
	table[st_H1][Comma_ST] = &Lexer::H1;
	table[st_Err][Comma_ST] = &Lexer::Err;

	table[st_A1][Colon_ST] = &Lexer::G1a;
	table[st_B3][Colon_ST] = &Lexer::P1a;
	table[st_C1][Colon_ST] = &Lexer::P1b;
	table[st_F2][Colon_ST] = &Lexer::P1c;
	table[st_H1][Colon_ST] = &Lexer::H1;
	table[st_Err][Colon_ST] = &Lexer::Err;

	table[st_A1][Dollar_ST] = &Lexer::F1;
	table[st_H1][Dollar_ST] = &Lexer::H1;
	table[st_Err][Dollar_ST] = &Lexer::Err;

	table[st_A1][LBracket_ST] = &Lexer::A2c;
	table[st_H1][LBracket_ST] = &Lexer::H1;
	table[st_Err][LBracket_ST] = &Lexer::Err;

	table[st_A1][RBracket_ST] = &Lexer::A2d;
	table[st_H1][RBracket_ST] = &Lexer::H1;
	table[st_Err][RBracket_ST] = &Lexer::Err;

	table[st_A1][EofSymb_ST] = &Lexer::Exit1;
	table[st_A2][EofSymb_ST] = &Lexer::Exit3;
	table[st_B1][EofSymb_ST] = &Lexer::Exit2;
	table[st_B2][EofSymb_ST] = &Lexer::Exit2;
	table[st_B3][EofSymb_ST] = &Lexer::Exit3;
	table[st_C1][EofSymb_ST] = &Lexer::Exit4;
	table[st_D1][EofSymb_ST] = &Lexer::Exit5;
	table[st_D2][EofSymb_ST] = &Lexer::Exit6;
	table[st_E1][EofSymb_ST] = &Lexer::Exit1;
	table[st_F1][EofSymb_ST] = &Lexer::Exit7;
	table[st_F2][EofSymb_ST] = &Lexer::Exit7;
	table[st_G1][EofSymb_ST] = &Lexer::Exit8;
	table[st_G2][EofSymb_ST] = &Lexer::Exit8;
	table[st_H1][EofSymb_ST] = &Lexer::Exit1;
	table[st_I1][EofSymb_ST] = &Lexer::Exit9;
	table[st_I2][EofSymb_ST] = &Lexer::Exit3;
	table[st_Err][EofSymb_ST] = &Lexer::Exit1;

	table[st_A1][Other_ST] = &Lexer::Err1;
	table[st_A2][Other_ST] = &Lexer::Err1;
	table[st_B1][Other_ST] = &Lexer::Err1;
	table[st_B2][Other_ST] = &Lexer::Err1;
	table[st_B3][Other_ST] = &Lexer::Err1;
	table[st_C1][Other_ST] = &Lexer::Err1;
	table[st_D1][Other_ST] = &Lexer::Err1;
	table[st_D2][Other_ST] = &Lexer::Err1;
	table[st_E1][Other_ST] = &Lexer::Err1;
	table[st_F1][Other_ST] = &Lexer::Err1;
	table[st_F2][Other_ST] = &Lexer::Err1;
	table[st_G1][Other_ST] = &Lexer::Err1;
	table[st_G2][Other_ST] = &Lexer::Err1;
	table[st_H1][Other_ST] = &Lexer::H1;
	table[st_I1][Other_ST] = &Lexer::Err1;
	table[st_I2][Other_ST] = &Lexer::Err1;
	table[st_Err][Other_ST] = &Lexer::Err;

	
	//начальный вектор

	for (int i = 0; i < 26; ++i)
	{
		detectionTable.initVector[i] = -1;
	}

	detectionTable.initVector['a' - 'a'] = 0;
	detectionTable.initVector['b' - 'a'] = 47;
	detectionTable.initVector['c' - 'a'] = 1;
	detectionTable.initVector['d' - 'a'] = 6;
	detectionTable.initVector['e' - 'a'] = 12;
	detectionTable.initVector['f' - 'a'] = 34;
	detectionTable.initVector['g' - 'a'] = 15;
	detectionTable.initVector['n' - 'a'] = 36;
	detectionTable.initVector['r' - 'a'] = 18;
	detectionTable.initVector['s' - 'a'] = 20;
	detectionTable.initVector['t' - 'a'] = 22;
	detectionTable.initVector['i' - 'a'] = 23;
	detectionTable.initVector['l' - 'a'] = 25;

	for (int i = 0; i < 65; ++i)
	{
		detectionTable.table[i].alt = -1;
		detectionTable.table[i].procedure = &Lexer::B1b;
	}

	detectionTable.table[0].letter = 's';   detectionTable.table[0].alt = 29;										detectionTable.table[0].procedure = &Lexer::B3a;

	detectionTable.table[1].letter = 'i';	detectionTable.table[1].alt = 3;
	detectionTable.table[2].letter = 'n';										detectionTable.table[2].procedure = &Lexer::B3b;

	detectionTable.table[3].letter = 'o';   detectionTable.table[3].alt = 44;
	detectionTable.table[4].letter = 'u';		
	detectionTable.table[5].letter = 't';										detectionTable.table[5].procedure = &Lexer::B3c;

	detectionTable.table[6].letter = 'e';	detectionTable.table[6].alt = 61;
	detectionTable.table[7].letter = 'c';   detectionTable.table[7].alt = 51;
	detectionTable.table[8].letter = 'l';
	detectionTable.table[9].letter = 'a';											
	detectionTable.table[10].letter = 'r';
	detectionTable.table[11].letter = 'e';										detectionTable.table[11].procedure = &Lexer::B3d;

	detectionTable.table[12].letter = 'l';	detectionTable.table[12].alt = 56;
	detectionTable.table[13].letter = 's';
	detectionTable.table[14].letter = 'e';										detectionTable.table[14].procedure = &Lexer::B3e;

	detectionTable.table[15].letter = 'o';
	detectionTable.table[16].letter = 't';                                          
	detectionTable.table[17].letter = 'o';										detectionTable.table[17].procedure = &Lexer::B3f;

	detectionTable.table[18].letter = 'e';
	detectionTable.table[19].letter = 'm';										detectionTable.table[19].procedure = &Lexer::B3g;

	detectionTable.table[20].letter = 'e';    detectionTable.table[20].alt = 39;
	detectionTable.table[21].letter = 't';										detectionTable.table[21].procedure = &Lexer::B3h;

	detectionTable.table[22].letter = 'o';										detectionTable.table[22].procedure = &Lexer::B3i;

	detectionTable.table[23].letter = 'n';   detectionTable.table[23].alt = 28;
	detectionTable.table[24].letter = 't';										detectionTable.table[24].procedure = &Lexer::B3j;

	detectionTable.table[25].letter = 'o';
	detectionTable.table[26].letter = 'n';
	detectionTable.table[27].letter = 'g';										detectionTable.table[27].procedure = &Lexer::B3k;

	detectionTable.table[28].letter = 'f';                                      detectionTable.table[28].procedure = &Lexer::B3l;

	detectionTable.table[29].letter = 'd';                      
	detectionTable.table[30].letter = 'd';
	detectionTable.table[31].letter = 'i';
	detectionTable.table[32].letter = 'n';
	detectionTable.table[33].letter = 'g';                                      detectionTable.table[33].procedure = &Lexer::B3n;

	detectionTable.table[34].letter = 'o';  detectionTable.table[34].alt = 58;
	detectionTable.table[35].letter = 'r';                                      detectionTable.table[35].procedure = &Lexer::B3m;

	detectionTable.table[36].letter = 'e';
	detectionTable.table[37].letter = 'x';
	detectionTable.table[38].letter = 't';                                      detectionTable.table[38].procedure = &Lexer::B3o;

	detectionTable.table[39].letter = 'w';
	detectionTable.table[40].letter = 'i';
	detectionTable.table[41].letter = 't';
	detectionTable.table[42].letter = 'c';
	detectionTable.table[43].letter = 'h';										detectionTable.table[43].procedure = &Lexer::B3p;

	detectionTable.table[44].letter = 'a';
	detectionTable.table[45].letter = 's';
	detectionTable.table[46].letter = 'e';										detectionTable.table[46].procedure = &Lexer::B3r;

	detectionTable.table[47].letter = 'r';
	detectionTable.table[48].letter = 'e';
	detectionTable.table[49].letter = 'a';
	detectionTable.table[50].letter = 'k';										detectionTable.table[50].procedure = &Lexer::B3s;

	detectionTable.table[51].letter = 'f';
	detectionTable.table[52].letter = 'a';
	detectionTable.table[53].letter = 'u';
	detectionTable.table[54].letter = 'l';
	detectionTable.table[55].letter = 't';										detectionTable.table[55].procedure = &Lexer::B3t;

	detectionTable.table[56].letter = 'n';
	detectionTable.table[57].letter = 'd';										detectionTable.table[57].procedure = &Lexer::B3u;

	detectionTable.table[58].letter = 'a';
	detectionTable.table[59].letter = 'i';
	detectionTable.table[60].letter = 'l';										detectionTable.table[60].procedure = &Lexer::B3v;

	detectionTable.table[61].letter = 'i';
	detectionTable.table[62].letter = 'g';
	detectionTable.table[63].letter = 'i';
	detectionTable.table[64].letter = 't';										detectionTable.table[64].procedure = &Lexer::B3w;
}

ParsedProgramLex Lexer::run(string filename)
{
	ifstream in(filename);

	if (!in)
	{
		cout << "Не удалось открыть файл " << filename << endl;
		return ParsedProgramLex(filename);
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

	return ParsedProgramLex(std::move(tokens), std::move(nameTable), filename);
}