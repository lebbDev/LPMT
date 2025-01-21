#include <fstream>
#include "ParsedProgram.h"
using namespace std;

ParsedProgram::ParsedProgram(const char* filename)
{
	programName = filename;
	tokens.push_back(Token_STACK(Error, 0, 1));
}

ParsedProgram::ParsedProgram(vector<Token_STACK>&& parsedTokens, vector<ObjectName_STACK> parsedNameTable, const char* filename)
{
	programName = filename;
	tokens = parsedTokens;
	nameTable = parsedNameTable;
}

ParsedProgram::ParsedProgram(ParsedProgram&& program)
{
	programName = program.programName;
	program.programName = nullptr;

	tokens = std::move(program.tokens);
	nameTable = std::move(program.nameTable);
}

ParsedProgram::~ParsedProgram()
{
	for (auto& name : nameTable)
	{
		switch (name.type)
		{
		case ObjectType_STACK::Variable: delete (string*)name.namePointer; break;
		case ObjectType_STACK::Constant: delete (int*)name.namePointer; break;
		case ObjectType_STACK::LongConstant: delete (LongNat*)name.namePointer; break;
		}
	}
}

void ParsedProgram::PrintTokens(ostream& stream)
{
	stream << "������ ������ � ����� " << programName << ":\n";
	for (auto& token : tokens) {
		stream << token << '\n';
	}
}

void ParsedProgram::PrintNames(ostream& stream)
{
	stream << "������� ���������� � ��������:\n";
	for (int i = 0; i < nameTable.size(); ++i) {
		stream << i << ". " << nameTable[i] << '\n';
	}
}

int ParsedProgram::PrintErrors(ostream& stream)
{
	stream << "������� ������ � ����� " << programName << " � �������:\n";

	int errors_count = 0;
	for (auto& token : tokens)
	{
		if (token.tokenClass != Error) continue;

		stream << token.line << '\n';
		++errors_count;
	}

	return errors_count;
}

Token_STACK::Token_STACK(TokenClass_STACK tok, int v, int l)
{
	tokenClass = tok;
	value = v;
	line = l;
}

ObjectName_STACK::ObjectName_STACK(ObjectType_STACK ot, void* np)
{
	type = ot;
	namePointer = np;
}