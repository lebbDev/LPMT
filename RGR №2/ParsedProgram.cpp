#include <fstream>
#include "ParsedProgram.h"
#include "LongNat.h"
using namespace std;

ParsedProgram::ParsedProgram(const char* filename)
{
	programName = filename;
	tokens.push_back(Token(Error, 0, 1));
}

ParsedProgram::ParsedProgram(vector<Token>&& parsedTokens, vector<ObjectName> parsedNameTable, const char* filename)
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
		case ObjectType::Variable: delete (string*)name.namePointer; break;
		case ObjectType::Constant: delete (int*)name.namePointer; break;
		case ObjectType::LongConstant: delete (LongNat*)name.namePointer; break;
		}
	}
}

void ParsedProgram::PrintTokens(ostream& stream)
{
	stream << "Список лексем в файле " << programName << ":\n";
	for (auto& token : tokens) {
		stream << token << '\n';
	}
}

void ParsedProgram::PrintNames(ostream& stream)
{
	stream << "Таблица переменных и констант:\n";
	for (int i = 0; i < nameTable.size(); ++i) {
		stream << i << ". " << nameTable[i] << '\n';
	}
}

int ParsedProgram::PrintErrors(ostream& stream)
{
	stream << "Найдены ошибки в файле " << programName << " в строках:\n";

	int errors_count = 0;
	for (auto& token : tokens)
	{
		if (token.tokenClass != Error) continue;

		stream << token.line << '\n';
		++errors_count;
	}

	return errors_count;
}

Token::Token(TokenClass tok, int v, int l)
{
	tokenClass = tok;
	value = v;
	line = l;
}

ObjectName::ObjectName(ObjectType ot, void* np)
{
	type = ot;
	namePointer = np;
}