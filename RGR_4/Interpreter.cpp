#include <iostream>
#include <vector>
#include <map>
#include "LongNat.h"
#include "Interpreter.h"
using namespace std;

void Interpreter::push(int nameIndex)
{
	ObjectName_STACK& obj_name = program.nameTable[nameIndex];

	Object new_obj;
	switch (obj_name.type)
	{
	case ObjectType_STACK::Variable:
	{
		if (variables.count(*(string*)obj_name.namePointer) == 0)
		{
			error();
			return;
		}
		new_obj = variables[*(string*)obj_name.namePointer];
		break;
	}
	case ObjectType_STACK::Constant: new_obj.set(new int(*(int*)obj_name.namePointer), ValueType::Integer); break;
	case ObjectType_STACK::LongConstant: new_obj.set(new LongNat(*(LongNat*)obj_name.namePointer), ValueType::LongNat); break;
	}

	stack.push_back(std::move(new_obj));
	tokenIndex++;
}

void Interpreter::pop(int nameIndex)
{
	if (stack.size() < 1)
	{
		error();
		return;
	}

	ObjectName_STACK& obj_name = program.nameTable[nameIndex];

	Object obj = std::move(stack.back());
	stack.pop_back();

	variables[*(string*)obj_name.namePointer] = obj;

	tokenIndex++;
}

void Interpreter::jmp(int nameIndex)
{
	ObjectName_STACK& obj_name = program.nameTable[nameIndex];
	int line_to_jump = *(int*)obj_name.namePointer;

	if (line_to_jump == 0)
	{
		error();
		return;
	}

	for (int tok_index = line_to_jump - 1; tok_index < program.tokens.size(); ++tok_index)
	{
		if (program.tokens[tok_index].line == line_to_jump)
		{
			tokenIndex = tok_index;
			return;
		}
	}

	error();
}

void Interpreter::ji(int nameIndex)
{
	if (stack.size() < 1)
	{
		error();
		return;
	}

	Object obj = std::move(stack.back());
	stack.pop_back();

	bool jump;
	switch (obj.get_type())
	{
	case ValueType::Integer: jump = *(int*)obj.get_ptr(); break;
	case ValueType::LongNat: jump = *(LongNat*)obj.get_ptr(); break;
	}

	if (jump) jmp(nameIndex);
	else tokenIndex++;
}

void Interpreter::read()
{
	while (cin.peek() == ' ' || cin.peek() == '\n') cin.ignore();

	Object new_obj;
	if (isdigit(cin.peek()))
	{
		int* n = new int;
		cin >> *n;
		new_obj.set(n, ValueType::Integer);
	}
	else
	{
		error();
		return;
	}

	stack.push_back(std::move(new_obj));
	tokenIndex++;
}

void Interpreter::rlong()
{
	while (cin.peek() == ' ' || cin.peek() == '\n') cin.ignore();

	Object new_obj;

	if (isdigit(cin.peek()))
	{
		LongNat* p = new LongNat;
		cin >> *p;
		new_obj.set(p, ValueType::LongNat);
	}
	else
	{
		error();
		return;
	}

	stack.push_back(std::move(new_obj));
	tokenIndex++;
}

void Interpreter::write()
{
	if (stack.size() < 1)
	{
		error();
		return;
	}

	Object obj = std::move(stack.back());
	stack.pop_back();

	switch (obj.get_type())
	{
	case ValueType::Integer: cout << *(int*)obj.get_ptr(); break;
	case ValueType::LongNat: cout << *(LongNat*)obj.get_ptr(); break;
	}
	cout << endl;
	tokenIndex++;
}

void Interpreter::end()
{
	interpreting = false;
}

void Interpreter::calculate(char operation)
{
	if (stack.size() < 2)
	{
		error();
		return;
	}

	Object obj2 = std::move(stack.back());
	stack.pop_back();
	Object obj1 = std::move(stack.back());
	stack.pop_back();

	switch (operation)
	{
	case '+': stack.push_back(obj1 + obj2); break;
	case '-': stack.push_back(obj1 - obj2); break;
	case '*': stack.push_back(obj1 * obj2); break;
	case '/': stack.push_back(obj1 / obj2); break;
	case '%': stack.push_back(obj1 % obj2); break;
	}
	tokenIndex++;
}

void Interpreter::compare(CmpValue_STACK operation)
{
	if (stack.size() < 2)
	{
		error();
		return;
	}

	Object obj2 = std::move(stack.back());
	stack.pop_back();
	Object obj1 = std::move(stack.back());
	stack.pop_back();

	try
	{
		bool cmp_result;

		switch (operation)
		{
		case Equal_ST:	cmp_result = (obj1 == obj2); break;
		case NotEqual_ST: cmp_result = (obj1 != obj2); break;
		case Less_ST: cmp_result = (obj1 < obj2); break;
		case LessOrEqual_ST: cmp_result = (obj1 <= obj2); break;
		case Bigger_ST: cmp_result = (obj1 > obj2); break;
		case BiggerOrEqual_ST:	cmp_result = (obj1 >= obj2); break;
		}

		stack.push_back(Object(new int(cmp_result), ValueType::Integer));
		tokenIndex++;
	}
	catch (...) { error(); }
}

void Interpreter::digit(int nameIndex, int digitIndex)
{
	ObjectName_STACK& obj_name = program.nameTable[nameIndex];
	Object var_obj;
	Object new_obj;

	if (variables.count(*(string*)obj_name.namePointer) == 0)
	{
		error();
		return;
	}

	var_obj = variables[*(string*)obj_name.namePointer];

	int digitVar;

	try
	{
		digitVar = var_obj.getDigitLong(digitIndex);
	}
	catch (...)
	{
		error();
		return;
	}

	new_obj.set(new int(digitVar), ValueType::Integer);

	stack.push_back(std::move(new_obj));
	tokenIndex++;
}

void Interpreter::skip()
{
	tokenIndex++;
}

void Interpreter::error()
{
	std::cout << "-----------------------------\n";
	std::cout << "Ошибка при работе интерпретатора...\n" << "Работа завершена\n";
	interpreting = false;
}

void Interpreter::execute_token(Token_STACK token)
{
	switch (token.tokenClass)
	{
	case Push: push(token.value); break;
	case Pop: pop(token.value); break;
	case Jmp: jmp(token.value); break;
	case Ji: ji(token.value); break;
	case Read: read(); break;
	case Rlong: rlong(); break;
	case Write: write(); break;
	case End: end(); break;
	case ArithmeticOp: calculate(token.value); break;
	case CmpOp:	compare((CmpValue_STACK)token.value); break;
	case DigitVar:
	{
		digitVarIndex = token.value;
		tokenIndex++;
		break;
	}
	case DigitIndex: digit(digitVarIndex, token.value); break;
	case Comment: skip(); break;
	case Error:	error(); break;
	case Eof: end(); break;
	}
}

void Interpreter::run()
{
	tokenIndex = 0;
	interpreting = true;

	while (interpreting)
		execute_token(program.tokens[tokenIndex]);
}