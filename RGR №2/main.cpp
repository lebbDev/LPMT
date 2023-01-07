#include <iostream>
#include "ParsedProgram.h"
#include "Parser.h"
#include "Interpreter.h"
using namespace std;

int main()
{
	setlocale(0, "ru");
	
	Parser parser;
	ParsedProgram program = parser.run("input3.txt");

	ofstream fout("tokVar_list.txt");

	program.PrintTokens(fout);
	fout << endl;

	program.PrintNames(fout);
	fout << endl;

	fout.close();

	/* запись ошибок */
	fout.open("list_errors.txt");

	int errors_count = program.PrintErrors(fout);
	fout << endl;

	fout.close();

	if (errors_count != 0)
    {
		std::cout << "Программа содержит ошибки...\nНомера строк с ошибками можно посмотреть в файле 'list_errors'" << std::endl;
		return 0;
	}

	Interpreter interpreter(std::move(program));
	interpreter.run();
	
	return 0;
}