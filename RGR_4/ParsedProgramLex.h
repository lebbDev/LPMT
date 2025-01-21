#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "LongNat.h"

enum SymbolicTokenClass {
	Letter_ST, Digit_ST, Arithmetic_ST,
	Cmp_ST, Space_ST, LF_ST, Semicolon_ST, EofSymb_ST,
	Comma_ST, Colon_ST, Dollar_ST, LBracket_ST, RBracket_ST, Other_ST
};

enum CmpValue {
	Negation, Equal, NotEqual, Less,
	LessOrEqual, Bigger, BiggerOrEqual
};


enum TokenClass {
	Int_T, Long_T, Declare_T, As_T, Set_T, To_T, Rem_T,
	If_T, Goto_T, Else_T, Input_T, Output_T, Cin_T, Cout_T,
	For_T, Adding_T, Next_T, Switch_T, Case_T, Break_T,
	Default_T, End_T, Fail_T, Digit_T,

	Variable_T, Constant_T, LongConstant_T, Label_T,

	Comma_T, Semicolon_T, ArithmeticOp_T, Assignment_T,
	Colon_T, CmpOp_T, LBracket_T, RBracket_T, Error_T, Eof_T
};

enum class ObjectType { Variable, Constant, LongConstant, Label };

struct SymbolicToken
{
	SymbolicTokenClass tokenClass;
	int value;
};

const int SYMBOL_TOKEN_CLASS_COUNT = 14;

struct Token
{
	TokenClass tokenClass;
	int value;
	int line;

	Token(TokenClass tok, int v, int l);

	friend std::ostream& operator<<(std::ostream& stream, Token& token)
	{
		switch (token.tokenClass) {
		case Int_T:		stream << "int";	break;
		case Long_T:		stream << "long";	break;
		case Declare_T:	stream << "declare"; break;
		case As_T:		stream << "as";		break;
		case Set_T:		stream << "set";	break;
		case To_T:		stream << "to";		break;
		case If_T:		stream << "if";		break;
		case Goto_T:		stream << "goto";   break;
		case Else_T:		stream << "else";   break;
		case Input_T:		stream << "<-";		break;
		case Output_T:	stream << "->";		break;
		case Cin_T:		stream << "cin";	break;
		case Cout_T:		stream << "cout";	break;
		case Rem_T:		stream << "Комментарий"; break;
		case For_T:		stream << "for"; break;
		case Adding_T:	stream << "adding"; break;
		case Next_T:		stream << "next"; break;
		case Switch_T:	stream << "switch"; break;
		case Case_T:		stream << "case"; break;
		case Break_T:		stream << "break"; break;
		case Default_T:	stream << "default"; break;
		case End_T:		stream << "end"; break;
		case Fail_T:		stream << "fail"; break;
		case Digit_T:		stream << "digit"; break;

		case Label_T:		stream << "Метка_(" << token.value << ")";	break;
		case Variable_T: stream << "Переменная_(" << token.value << ")"; break;
		case Constant_T: stream << "Константа_(" << token.value << ")"; break;
		case LongConstant_T: stream << "Дл_Константа_(" << token.value << ")"; break;

		case LBracket_T: stream << "("; break;
		case RBracket_T: stream << ")"; break;
		case Semicolon_T: stream << ";";	break;
		case Colon_T: stream << ":"; break;
		case Comma_T: stream << ",";	break;

		case Assignment_T: stream << "="; break;
		case ArithmeticOp_T: stream << (char)token.value; break;
		case CmpOp_T:
		{
			switch (token.value)
			{
			case Equal:		    stream << "==";  break;
			case NotEqual:      stream << "!="; break;
			case Less:		    stream << '<';  break;
			case LessOrEqual:   stream << "<="; break;
			case Bigger:        stream << '>';  break;
			case BiggerOrEqual: stream << ">="; break;
			default:            stream << "Неизвестное отношение";
			}
			break;
		}

		case Error_T:     stream << "Ошибка";      break;
		case Eof_T:		stream << "Конец файла"; break;
		default:        stream << "Неизвестная лексема";
		}

		return stream;
	}
};

struct ObjectName
{
	ObjectType type;
	void* namePointer;

	ObjectName(ObjectType ot, void* np);

	friend std::ostream& operator<<(std::ostream& stream, ObjectName& obj)
	{
		switch (obj.type)
		{
		case ObjectType::Variable: stream << "Переменная: " << *((std::string*)obj.namePointer); break;
		case ObjectType::Constant: stream << "Константа: " << *((int*)obj.namePointer); break;
		case ObjectType::LongConstant: stream << "Длинная константа: " << *((LongNat*)obj.namePointer); break;
		case ObjectType::Label: stream << "Метка: " << *((std::string*)obj.namePointer); break;
		default: stream << "Неизвестный объект...";
		}

		return stream;
	}
};

class ParsedProgramLex
{
public:
	std::string programName;
	std::vector <Token> tokens;
	std::vector <ObjectName> nameTable;

	ParsedProgramLex(std::string filename);
	ParsedProgramLex(std::vector<Token>&& parsedTokens, std::vector<ObjectName> parsedNameTable, std::string filename);
	ParsedProgramLex(ParsedProgramLex&& program);
	~ParsedProgramLex();
	void PrintTokens();
	void PrintNames();
	int PrintErrors();
};