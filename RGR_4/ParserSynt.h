#pragma once
#include <map>
#include <set>
#include <vector>
#include <list>
#include <stack>
#include "Lexer.h"
#include "Object.h"

enum Nonterminals {
	Start_NT = 40, Loop_1_NT, Loop_2_NT, ProgramCase_NT, Program_NT,
	Operator_NT, Declares_NT, Declare_NT, Decl_NT, Type_NT,
	List_1_NT, Set_NT, ListVar_NT, ListVar2_NT, ListVar3_NT, SetEnd_NT, 
	Next_NT,  ToExpr_NT, AddingExpr_NT,
	InputVar_NT, List_2_NT, OutputExpr_NT, List_3_NT, 
	BodySwitch_NT, Cases_NT, Case_NT, Default_NT, CaseBeg_NT, 
	Test_NT, Expression_NT, ExpressionLine_NT, Term_NT, TermLine_NT, 
	Factor_NT, CenterFunc_NT, Constant_NT, Var_NT, LabelEnd_NT,
	StackEnd
};

const int NONTERM_COUNT = 39;

enum Terminals {
	Int_TM, Long_TM, Declare_TM, As_TM, Set_TM, To_TM, Rem_TM,
	If_TM, Goto_TM, Else_TM, Input_TM, Output_TM, Cin_TM, Cout_TM,
	For_TM, Adding_TM, Next_TM, Switch_TM, Case_TM, Break_TM,
	Default_TM, End_TM, Fail_TM, Digit_TM,

	Variable_TM, Constant_TM, LongConstant_TM, Label_TM,

	Comma_TM, Semicolon_TM, Plus_TM, Minus_TM, Mult_TM, Division_TM, DivRemander_TM,
	Assignment_TM, Colon_TM, CmpOp_TM, LBracket_TM, RBracket_TM
};

const int TERM_COUNT = 40;

enum class ComparePrecedence
{
	Equal, Less, Bigger, Error
};

enum class Atom
{
	Declare, CinL, CinI, Cout, Set, Label, Goto, If, IfElse, For, ForAdding, Next,
	Switch, Case, Break, Default, EndSwitch, Fail, None
};

class ParserSynt
{
public: 
	// Для синт. анализатора
	std::multimap<int, std::vector<int>> ruleTable;
	std::map<int, std::set<int>> beginSet;
	std::map<int, std::set<int>> endSet;

	ComparePrecedence compareTable[NONTERM_COUNT + TERM_COUNT][NONTERM_COUNT + TERM_COUNT];

	struct el_stack
	{
		int el_rule;
		ComparePrecedence cmp;
	};

	std::vector<el_stack> stack;
	std::vector<int> ruleVec;

	
	// Для транслятора
	struct el_atomsTable
	{
		Atom classAtom;
		std::string atr1;
		std::string atr2;
		std::string atr3;
		std::vector<Token> atr4;
		std::vector<Token> atr5;
		std::vector<Token> atr6;
	};

	std::vector<el_atomsTable> atomsTable;
	el_atomsTable tmpAtom;

	std::map<std::string, ValueType> declaredVars;
	std::map<std::string, int> declaredLabels;

	bool cmpFlag;
	bool toFlag;
	bool digitFlag;
	bool isInt;
	bool isLong;

	std::map<int, int> atomLine;
	int endLine;

public:
	ParserSynt();
	bool FillCompareTable();
	bool run(ParsedProgramLex& program);
	bool TranslateToStackLang(std::string filename, ParsedProgramLex& program);

private:
	void GetBeginSet();
	void GetEndSet();
	void GetEqualPrecedence();
	void GetLessPrecedence();
	void GetBiggerPrecedence();
	void TranslateToken(int& classTok, int& val);
	bool GetRuleHead();
	void TranslateToAtom(const Token tok, int index, ParsedProgramLex& program);
	int TranslateExpression(std::vector<Token>& vect);
	bool PrintExpression(std::vector<Token>& vect, std::ofstream& out, ParsedProgramLex& program);
	void CountLines();
};