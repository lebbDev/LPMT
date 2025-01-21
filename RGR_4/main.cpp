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
	/*cout << "������� ��� �����: ";
	cin >> filename;*/

	// ����������� ����
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
		cout << "������ ������������ �����\n" << "������ ���������\n";
		return 0;
	}

	// �������������� ����
	ParserSynt parser;

	if (!parser.FillCompareTable())  // ��������� ������� ��������� ���������������
		return 0;
	
	if (!parser.run(program))
	{
		cout << "-----------------------------\n";
		cout << "������ ��������������� �����\n" << "������ ���������\n";
		return 0;
	}
	
	
	if (!parser.TranslateToStackLang(fileRes, program))
	{
		cout << "-----------------------------\n";
		cout << "������ ��� ����������\n" << "������ ���������\n";
		return 0;
	}


	// �������������
	Parser parserSt;
	ParsedProgram programSt = parserSt.run("testRes.txt");

	/*ofstream fout("tokVar_list.txt");

	program.PrintTokens(fout);
	fout << endl;

	program.PrintNames(fout);
	fout << endl;

	fout.close();*/

	/* ������ ������ */
	//fout.open("list_errors.txt");

	ofstream fout("list_errors.txt");

	errors_count = programSt.PrintErrors(fout);
	fout << endl;

	fout.close();

	if (errors_count != 0)
	{
		cout << "-----------------------------\n";
		std::cout << "������ ��� ������ ��������������\n" << "������ ���������\n";
		std::cout << "������ ����� � �������� ����� ���������� � �����'list_errors'\n";
		return 0;
	}

	cout << "==============================\n";
	cout << "��������� ������ ���������:\n";

	Interpreter interpreter(std::move(programSt));
	interpreter.run();

	return 0;
}