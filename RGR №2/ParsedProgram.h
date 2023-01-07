#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "LongNat.h"

enum SymbolicTokenClass {
	Letter, Digit, Arithmetic,
	Cmp, Space, LF, Semicolon, EofSymb, Other
};

const int SYMBOL_TOKEN_CLASS_COUNT = 9;

struct SymbolicToken
{
	SymbolicTokenClass tokenClass;
	int value;
};

enum CmpValue {
	Negation, Equal, NotEqual, Less,
	LessOrEqual, Bigger, BiggerOrEqual
};

enum TokenClass {
	Push, Pop, Jmp, Ji, Read, Rlong, Write, End,
	ArithmeticOp, CmpOp,
	Comment, Error, Eof
};

struct Token
{
	TokenClass tokenClass;
	int value;
	int line;

	Token(TokenClass tok, int v, int l);

	friend std::ostream& operator<<(std::ostream& stream, Token& token)
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

		case ArithmeticOp: stream << (char)token.value; break;
		case CmpOp:
		{
			switch (token.value)
			{
			case Equal:		    stream << '=';  break;
			case NotEqual:      stream << "!="; break;
			case Less:		    stream << '<';  break;
			case LessOrEqual:   stream << "<="; break;
			case Bigger:        stream << '>';  break;
			case BiggerOrEqual: stream << ">="; break;
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

enum class ObjectType { Variable, Constant, LongConstant };

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
		default: stream << "Неизвестный объект...";
		}

		return stream;
	}
};

class ParsedProgram
{
public:
	const char* programName;
	std::vector <Token> tokens;
	std::vector <ObjectName> nameTable;

	friend class Interpeter;

	ParsedProgram(const char* filename);
	ParsedProgram(std::vector<Token>&& parsedTokens, std::vector<ObjectName> parsedNameTable, const char* filename);
	ParsedProgram(ParsedProgram&& program);
	~ParsedProgram();
	void PrintTokens(std::ostream& stream);
	void PrintNames(std::ostream& stream);
	int PrintErrors(std::ostream& stream);
};