#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "LongNat.h"

enum SymbolicTokenClass_STACK {
	Letter, Digit, Arithmetic,
	Cmp, Space, LF, Semicolon, Dollar, Comma, EofSymb, Other
};

const int SYMBOL_TOKEN_CLASS_COUNT_ST = 10;

struct SymbolicToken_STACK
{
	SymbolicTokenClass_STACK tokenClass;
	int value;
};

enum CmpValue_STACK {
	Negation_ST = 1, Equal_ST, NotEqual_ST, Less_ST,
	Bigger_ST, LessOrEqual_ST, BiggerOrEqual_ST
};

enum TokenClass_STACK {
	Push, Pop, Jmp, Ji, Read, Rlong, Write, End, DigitVar, DigitIndex,
	ArithmeticOp, CmpOp,
	Comment, Error, Eof
};

struct Token_STACK
{
	TokenClass_STACK tokenClass;
	int value;
	int line;

	Token_STACK(TokenClass_STACK tok, int v, int l);

	friend std::ostream& operator<<(std::ostream& stream, Token_STACK& token)
	{
		stream << token.line << ": (";
		switch (token.tokenClass) {
		case Push:  stream << "push, " << token.value; break;
		case Pop:   stream << "pop, " << token.value; break;
		case Jmp:   stream << "jmp, " << token.value; break;
		case Ji:    stream << "ji, " << token.value; break;
		case Read:  stream << "read";  break;
		case Rlong: stream << "rlong"; break;
		case Write: stream << "write"; break;
		case End:   stream << "end";   break;
		case DigitVar: stream << "dig " << token.value; break;
		case DigitIndex: stream << ", " << token.value; break;

		case ArithmeticOp: stream << (char)token.value; break;
		case CmpOp:
		{
			switch (token.value)
			{
			case Equal_ST:		    stream << '=';  break;
			case NotEqual_ST:      stream << "!="; break;
			case Less_ST:		    stream << '<';  break;
			case LessOrEqual_ST:   stream << "<="; break;
			case Bigger_ST:        stream << '>';  break;
			case BiggerOrEqual_ST: stream << ">="; break;
			default:            stream << "Неизвестное отношение...";
			}
			break;
		}

		case Comment:   stream << "Комментарий"; break;
		case Error:     stream << "Ошибка";      break;
		case Eof:		stream << "Конец файла"; break;
		default:        stream << "Неизвестная лексема...";
		}
		stream << ')';

		return stream;
	}
};

enum class ObjectType_STACK { Variable, Constant, LongConstant };

struct ObjectName_STACK
{
	ObjectType_STACK type;
	void* namePointer;

	ObjectName_STACK(ObjectType_STACK ot, void* np);

	friend std::ostream& operator<<(std::ostream& stream, ObjectName_STACK& obj)
	{
		switch (obj.type)
		{
		case ObjectType_STACK::Variable: stream << "Переменная: " << *((std::string*)obj.namePointer); break;
		case ObjectType_STACK::Constant: stream << "Константа: " << *((int*)obj.namePointer); break;
		case ObjectType_STACK::LongConstant: stream << "Длинная константа: " << *((LongNat*)obj.namePointer); break;
		default: stream << "Неизвестный объект...";
		}

		return stream;
	}
};

class ParsedProgram
{
public:
	const char* programName;
	std::vector <Token_STACK> tokens;
	std::vector <ObjectName_STACK> nameTable;

	friend class Interpeter;

	ParsedProgram(const char* filename);
	ParsedProgram(std::vector<Token_STACK>&& parsedTokens, std::vector<ObjectName_STACK> parsedNameTable, const char* filename);
	ParsedProgram(ParsedProgram&& program);
	~ParsedProgram();
	void PrintTokens(std::ostream& stream);
	void PrintNames(std::ostream& stream);
	int PrintErrors(std::ostream& stream);
};