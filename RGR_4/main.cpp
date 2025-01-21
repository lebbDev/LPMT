#include <iostream>
#include "ParsedProgramLex.h"
#include "Lexer.h"
#include "ParserSynt.h"
#include "ParsedProgram.h"
#include "ParserStLang.h"
#include "Interpreter.h"
using namespace std;

int main()
{
	setlocale(0, "ru");

	string fileSource = "testTRUE2.txt";
	string fileRes = "testRes.txt";
	/*cout << "Введите имя файла: ";
	cin >> filename;*/

	// Лескический блок
	Lexer lexer;
	ParsedProgramLex program = lexer.run(fileSource);

	/*program.PrintTokens();
	cout << endl;*/

	/*program.PrintNames();
	cout << endl;*/

	int errors_count = program.PrintErrors();

	if (errors_count)
	{
		cout << "-----------------------------\n";
		cout << "Ошибки лексического блока\n" << "Работа завершена\n";
		return 0;
	}

	// Синтаксический блок
	ParserSynt parser;

	if (!parser.FillCompareTable())  // Заполняем таблицу отношений предшествования
		return 0;
	
	if (!parser.run(program))
	{
		cout << "-----------------------------\n";
		cout << "Ошибки синтаксического блока\n" << "Работа завершена\n";
		return 0;
	}
	
	
	if (!parser.TranslateToStackLang(fileRes, program))
	{
		cout << "-----------------------------\n";
		cout << "Ошибки при трансляции\n" << "Работа завершена\n";
		return 0;
	}


	// Интерпретатор
	Parser parserSt;
	ParsedProgram programSt = parserSt.run("testRes.txt");

	/*ofstream fout("tokVar_list.txt");

	program.PrintTokens(fout);
	fout << endl;

	program.PrintNames(fout);
	fout << endl;

	fout.close();*/

	/* запись ошибок */
	//fout.open("list_errors.txt");

	ofstream fout("list_errors.txt");

	errors_count = programSt.PrintErrors(fout);
	fout << endl;

	fout.close();

	if (errors_count != 0)
	{
		cout << "-----------------------------\n";
		std::cout << "Ошибки при работе интерпретатора\n" << "Работа завершена\n";
		std::cout << "Номера строк с ошибками можно посмотреть в файле'list_errors'\n";
		return 0;
	}

	cout << "==============================\n";
	cout << "Результат работы программы:\n";

	Interpreter interpreter(std::move(programSt));
	interpreter.run();

	return 0;
}