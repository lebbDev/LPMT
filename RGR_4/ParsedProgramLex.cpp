#include <fstream>
#include "ParsedProgramLex.h"
using namespace std;

ParsedProgramLex::ParsedProgramLex(string filename)
{
	programName = filename;
	tokens.push_back(Token(Error_T, 0, 1));
}

ParsedProgramLex::ParsedProgramLex(vector<Token>&& parsedTokens, vector<ObjectName> parsedNameTable, string filename)
{
	programName = filename;
	tokens = parsedTokens;
	nameTable = parsedNameTable;
}

ParsedProgramLex::ParsedProgramLex(ParsedProgramLex&& program)
{
	programName = program.programName;
	program.programName = nullptr;

	tokens = std::move(program.tokens);
	nameTable = std::move(program.nameTable);
}

ParsedProgramLex::~ParsedProgramLex()
{
	for (auto& name : nameTable)
	{
		switch (name.type)
		{
		case ObjectType::Variable: delete (string*)name.namePointer; break;
		case ObjectType::Constant: delete (int*)name.namePointer; break;
		case ObjectType::LongConstant: delete (LongNat*)name.namePointer; break;
		case ObjectType::Label: delete (string*)name.namePointer; break;
		}
	}
}

void ParsedProgramLex::PrintTokens()
{
	int prevLine = tokens[0].line, curLine;

	cout << "Список лексем в файле " << programName << ":\n";
	cout << tokens[0].line << ": ";

	for (auto& token : tokens) {
		curLine = token.line;

		if (curLine != prevLine)
			cout << "\n" << curLine << ": ";

		cout << token << " ";
		prevLine = curLine;
	}
}

void ParsedProgramLex::PrintNames()
{
	cout << "\n";
	cout << "Таблица имён:\n";
	for (int i = 0; i < nameTable.size(); ++i) {
		cout << i << ". " << nameTable[i] << '\n';
	}
}

int ParsedProgramLex::PrintErrors()
{
	int errors_count = 0;
	for (auto& token : tokens)
	{
		if (token.tokenClass != Error_T) continue;
		errors_count++;
		
		if (errors_count == 1) cout << "Ошибки в строках:\n";
		cout << token.line << '\n';
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