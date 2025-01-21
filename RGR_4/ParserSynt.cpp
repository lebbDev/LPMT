#include <fstream>
#include "ParserSynt.h"
#include "ParsedProgramLex.h"
using namespace std;


ParserSynt::ParserSynt()
{
	// Инициализация таблицы правил

	ruleTable.emplace(Start_NT, vector<int>{ Declares_NT, Program_NT });
	ruleTable.emplace(Start_NT, vector<int>{ Program_NT });

	ruleTable.emplace(Loop_1_NT, vector<int>{ Colon_TM, Label_TM, For_TM, Variable_TM, Assignment_TM, ToExpr_NT, Program_NT });
	ruleTable.emplace(Loop_2_NT, vector<int>{ Colon_TM, Label_TM, For_TM, Variable_TM, Assignment_TM, ToExpr_NT, AddingExpr_NT, Program_NT });
	ruleTable.emplace(ProgramCase_NT, vector<int>{ Colon_TM, Program_NT });

	ruleTable.emplace(Program_NT, vector<int>{ Operator_NT, Program_NT });
	ruleTable.emplace(Program_NT, vector<int>{ Operator_NT });

	ruleTable.emplace(Operator_NT, vector<int>{ Semicolon_TM });
	ruleTable.emplace(Operator_NT, vector<int>{ List_1_NT, Semicolon_TM });
	ruleTable.emplace(Operator_NT, vector<int>{ Loop_1_NT, Next_NT });
	ruleTable.emplace(Operator_NT, vector<int>{ Loop_2_NT, Next_NT });
	ruleTable.emplace(Operator_NT, vector<int>{ If_TM, Test_NT, Goto_TM, LabelEnd_NT, Else_TM, Goto_TM, LabelEnd_NT, Semicolon_TM });
	ruleTable.emplace(Operator_NT, vector<int>{ If_TM, Test_NT, Goto_TM, LabelEnd_NT, Semicolon_TM });
	ruleTable.emplace(Operator_NT, vector<int>{ InputVar_NT, Cin_TM, Semicolon_TM });
	ruleTable.emplace(Operator_NT, vector<int>{ OutputExpr_NT, Cout_TM, Semicolon_TM });
	ruleTable.emplace(Operator_NT, vector<int>{ Colon_TM, Label_TM });
	ruleTable.emplace(Operator_NT, vector<int>{ Goto_TM, LabelEnd_NT, Semicolon_TM});
	ruleTable.emplace(Operator_NT, vector<int>{ Switch_TM, LBracket_TM, Expression_NT, RBracket_TM, BodySwitch_NT, End_TM, Switch_TM, Semicolon_TM });
	ruleTable.emplace(Operator_NT, vector<int>{ Fail_TM, Semicolon_TM });
	ruleTable.emplace(Operator_NT, vector<int>{ Rem_TM });

	ruleTable.emplace(Declares_NT, vector<int>{ Declare_NT, Semicolon_TM });
	ruleTable.emplace(Declare_NT, vector<int>{ Declare_TM, Variable_TM, As_TM, Type_NT, Decl_NT });
	ruleTable.emplace(Declare_NT, vector<int>{ Declare_TM, Variable_TM, As_TM, Type_NT });
	ruleTable.emplace(Decl_NT, vector<int>{ Comma_TM, Variable_TM, As_TM, Type_NT, Decl_NT });
	ruleTable.emplace(Decl_NT, vector<int>{ Comma_TM, Variable_TM, As_TM, Type_NT });
	ruleTable.emplace(Type_NT, vector<int>{ Int_TM });
	ruleTable.emplace(Type_NT, vector<int>{ Long_TM });

	ruleTable.emplace(List_1_NT, vector<int>{ Set_NT, ListVar_NT, SetEnd_NT });
	ruleTable.emplace(List_1_NT, vector<int>{ Set_NT, SetEnd_NT });
	ruleTable.emplace(Set_NT, vector<int>{ Set_TM, Variable_TM });
	ruleTable.emplace(ListVar_NT, vector<int>{ ListVar2_NT });
	ruleTable.emplace(ListVar2_NT, vector<int>{ ListVar2_NT, ListVar3_NT });
	ruleTable.emplace(ListVar2_NT, vector<int>{ ListVar3_NT });
	ruleTable.emplace(ListVar3_NT, vector<int>{ Comma_TM, Variable_TM });
	ruleTable.emplace(SetEnd_NT, vector<int>{ To_TM, Expression_NT });

	ruleTable.emplace(Next_NT, vector<int>{ Next_TM, Label_TM });
	ruleTable.emplace(ToExpr_NT, vector<int>{ Expression_NT, To_TM, Expression_NT });
	ruleTable.emplace(AddingExpr_NT, vector<int>{ Adding_TM, Expression_NT });

	ruleTable.emplace(InputVar_NT, vector<int>{ List_2_NT });
	ruleTable.emplace(List_2_NT, vector<int>{ Variable_TM, Input_TM, List_2_NT });
	ruleTable.emplace(List_2_NT, vector<int>{ Variable_TM, Input_TM });

	ruleTable.emplace(OutputExpr_NT, vector<int>{ List_3_NT });
	ruleTable.emplace(List_3_NT, vector<int>{ Expression_NT, Output_TM, List_3_NT });
	ruleTable.emplace(List_3_NT, vector<int>{ Expression_NT, Output_TM });

	ruleTable.emplace(BodySwitch_NT, vector<int>{ Cases_NT });
	ruleTable.emplace(Cases_NT, vector<int>{ Case_NT, Cases_NT });
	ruleTable.emplace(Cases_NT, vector<int>{ Default_NT });
	ruleTable.emplace(Case_NT, vector<int>{ CaseBeg_NT, ProgramCase_NT, Break_TM, Semicolon_TM });
	ruleTable.emplace(Default_NT, vector<int>{ Default_TM, ProgramCase_NT, Break_TM, Semicolon_TM });
	ruleTable.emplace(CaseBeg_NT, vector<int>{ Case_TM, Constant_NT });

	ruleTable.emplace(Test_NT, vector<int>{ Expression_NT, CmpOp_TM, Expression_NT });

	ruleTable.emplace(Expression_NT, vector<int>{ Term_NT });
	ruleTable.emplace(Expression_NT, vector<int>{ Term_NT, ExpressionLine_NT });
	ruleTable.emplace(ExpressionLine_NT, vector<int>{ Plus_TM, Term_NT, ExpressionLine_NT });
	ruleTable.emplace(ExpressionLine_NT, vector<int>{ Minus_TM, Term_NT, ExpressionLine_NT });
	ruleTable.emplace(ExpressionLine_NT, vector<int>{ Plus_TM, Term_NT });
	ruleTable.emplace(ExpressionLine_NT, vector<int>{ Minus_TM, Term_NT });

	ruleTable.emplace(Term_NT, vector<int>{ Factor_NT });
	ruleTable.emplace(Term_NT, vector<int>{ Factor_NT, TermLine_NT });
	ruleTable.emplace(TermLine_NT, vector<int>{ Mult_TM, Factor_NT, TermLine_NT });
	ruleTable.emplace(TermLine_NT, vector<int>{ Division_TM, Factor_NT, TermLine_NT });
	ruleTable.emplace(TermLine_NT, vector<int>{ DivRemander_TM, Factor_NT, TermLine_NT });
	ruleTable.emplace(TermLine_NT, vector<int>{ Mult_TM, Factor_NT });
	ruleTable.emplace(TermLine_NT, vector<int>{ Division_TM, Factor_NT });
	ruleTable.emplace(TermLine_NT, vector<int>{ DivRemander_TM, Factor_NT });

	ruleTable.emplace(Factor_NT, vector<int>{ LBracket_TM, Expression_NT, RBracket_TM });
	ruleTable.emplace(Factor_NT, vector<int>{ Constant_NT });
	ruleTable.emplace(Factor_NT, vector<int>{ Var_NT });
	ruleTable.emplace(Factor_NT, vector<int>{ Digit_TM, LBracket_TM, CenterFunc_NT, RBracket_TM });

	ruleTable.emplace(CenterFunc_NT, vector<int>{ Var_NT, Comma_TM, Constant_NT });
	ruleTable.emplace(Constant_NT, vector<int>{ Constant_TM });
	ruleTable.emplace(Constant_NT, vector<int>{ LongConstant_TM });
	ruleTable.emplace(Var_NT, vector<int>{ Variable_TM });
	ruleTable.emplace(LabelEnd_NT, vector<int>{ Label_TM });

	// Инициализация таблицы отношений
	for (int i = 0; i < NONTERM_COUNT + TERM_COUNT; i++)
	{
		for (int j = 0; j < NONTERM_COUNT + TERM_COUNT; j++)
		{
			compareTable[i][j] = ComparePrecedence::Error;
		}
	}
}

// Проход по правилам с конца
void ParserSynt::GetBeginSet()
{
	beginSet[Loop_1_NT].insert(Colon_TM);
	beginSet[Loop_2_NT].insert(Colon_TM);

	for (auto iter = ruleTable.rbegin(); iter != ruleTable.rend(); iter++)
	{
		if (iter->second[0] >= 40)
		{
			if (!beginSet[iter->second[0]].empty())
			{
				beginSet[iter->first].insert(beginSet[iter->second[0]].begin(), beginSet[iter->second[0]].end());
			}
		}
		beginSet[iter->first].insert(iter->second[0]);
	}
}

void ParserSynt::GetEndSet()
{
	for (auto iter = ruleTable.rbegin(); iter != ruleTable.rend(); iter++)
	{
		int sizeVec = iter->second.size();

		if (iter->second[sizeVec - 1] >= 40 && iter->first != iter->second[sizeVec - 1])
		{
			if (!endSet[iter->second[sizeVec - 1]].empty())
			{
				endSet[iter->first].insert(endSet[iter->second[sizeVec - 1]].begin(), endSet[iter->second[sizeVec - 1]].end());
			}
		}
		endSet[iter->first].insert(iter->second[sizeVec - 1]);
	}
}

void ParserSynt::GetEqualPrecedence()
{
	// Получение отношения равенства
	for (auto iter = ruleTable.begin(); iter != ruleTable.end(); iter++)
	{
		for (int i = 0; i < iter->second.size() - 1; i++)
		{    // На пересечении двух элементов правила
			compareTable[iter->second[i]][iter->second[i + 1]] = ComparePrecedence::Equal;
		}
	}
}

void ParserSynt::GetLessPrecedence()
{
	// Получение отношения меньше
	for (auto iter = ruleTable.begin(); iter != ruleTable.end(); iter++)
	{
		for (int i = 1; i < iter->second.size(); i++) // Идем до последнего элемента
		{
			if (iter->second[i] >= 40) // Если нетерминал
			{
				for (auto setIter = beginSet[iter->second[i]].begin(); setIter != beginSet[iter->second[i]].end(); setIter++)
				{
					if (compareTable[iter->second[i - 1]][*setIter] == ComparePrecedence::Bigger || compareTable[iter->second[i - 1]][*setIter] == ComparePrecedence::Equal)
						throw "Error";

					compareTable[iter->second[i - 1]][*setIter] = ComparePrecedence::Less;
				}
			}
		}
	}
}

void ParserSynt::GetBiggerPrecedence()
{
	// Получение отношения больше
	for (auto iter = ruleTable.begin(); iter != ruleTable.end(); iter++)
	{
		for (int i = 0; i < iter->second.size() - 1; i++) // Идем до предпоследнего элемента
		{
			if (iter->second[i] >= 40 && iter->second[i + 1] >= 40) // Если два нетерминала подряд
			{
				for (auto endSetIter = endSet[iter->second[i]].begin(); endSetIter != endSet[iter->second[i]].end(); endSetIter++)
				{
					for (auto beginSetIter = beginSet[iter->second[i + 1]].begin(); beginSetIter != beginSet[iter->second[i + 1]].end(); beginSetIter++)
					{
						if (compareTable[*endSetIter][*beginSetIter] == ComparePrecedence::Less || compareTable[*endSetIter][*beginSetIter] == ComparePrecedence::Equal)
							throw "Error";

						compareTable[*endSetIter][*beginSetIter] = ComparePrecedence::Bigger;
					}
				}
			}
			else if (iter->second[i] >= 40) // Если первый нетерминал и второй терминал
			{
				for (auto setIter = endSet[iter->second[i]].begin(); setIter != endSet[iter->second[i]].end(); setIter++)
				{
					if (compareTable[*setIter][iter->second[i + 1]] == ComparePrecedence::Less || compareTable[*setIter][iter->second[i + 1]] == ComparePrecedence::Equal)
						throw "Error";

					compareTable[*setIter][iter->second[i + 1]] = ComparePrecedence::Bigger;
				}
			}
		}
	}
}

bool ParserSynt::FillCompareTable()
{
	// Получение множеств НАЧ и КОН
	this->GetBeginSet();
	this->GetEndSet();
	
	// Получение отношений предшествования
	try
	{
		this->GetEqualPrecedence();
		this->GetLessPrecedence();
		this->GetBiggerPrecedence();
	}
	catch (...)
	{
		cout << "Filling compare table error\n";
		return false;
	}

	// Конец магазина < НАЧ(Start_NT)
	for (auto iter = beginSet[Start_NT].begin(); iter != beginSet[Start_NT].end(); iter++)
	{
		compareTable[StackEnd][*iter] = ComparePrecedence::Less;
	}
	// КОН(Start_NT) > конца магазина
	for (auto iter = endSet[Start_NT].begin(); iter != endSet[Start_NT].end(); iter++)
	{
		compareTable[*iter][StackEnd] = ComparePrecedence::Bigger;
	}

	return true;
}

void ParserSynt::TranslateToken(int& classTok, int& val)
{
	switch (classTok)
	{
	case 30:
	{
		switch (val)
		{
		case '+': classTok = 30; break;
		case '-': classTok = 31; break;
		case '*': classTok = 32; break;
		case '/': classTok = 33; break;
		case '%': classTok = 34; break;
		}
		break;
	}
	case 31: classTok = 35; break;
	case 32: classTok = 36; break;
	case 33: classTok = 37; break;
	case 34: classTok = 38; break;
	case 35: classTok = 39; break;
	case 37: classTok = 78; break;
	}
}

 // Поиск head нужного правила
bool ParserSynt::GetRuleHead()
{
	el_stack tmpEl;
	for (auto& it : ruleTable)
	{
		if (it.second == ruleVec)
		{
			tmpEl.el_rule = it.first;
			tmpEl.cmp = compareTable[stack.back().el_rule][tmpEl.el_rule];
			stack.push_back(tmpEl);

			return true;
		}
	}
	return false;
}

 // С помощью ключевых слов отмечаем класс атома и передаем атому нужные атрибуты
void ParserSynt::TranslateToAtom(const Token tok, int index, ParsedProgramLex& program)
{
	switch (tok.tokenClass)
	{
	case Declare_T: 
	{
		tmpAtom.classAtom = Atom::Declare;
		break;
	}
	case Set_T:
	{
		tmpAtom.classAtom = Atom::Set;
		break;
	}
	case Input_T:
	{
		if (program.tokens[index - 1].tokenClass == Variable_T)
		{
			tmpAtom.atr1 = *(string*)program.nameTable[program.tokens[index - 1].value].namePointer;

			if (declaredVars[tmpAtom.atr1] == ValueType::Integer)
			{
				tmpAtom.classAtom = Atom::CinI;
			}
			else
			{
				tmpAtom.classAtom = Atom::CinL;
			}

			atomsTable.push_back(tmpAtom);
			tmpAtom.classAtom = Atom::None;
		}
		break;
	}
	case Output_T:
	{
		tmpAtom.classAtom = Atom::Cout;
		atomsTable.push_back(tmpAtom);
		tmpAtom.classAtom = Atom::None;
		tmpAtom.atr4.clear();
		break;
	}
	case Colon_T:
	{
		if (program.tokens[index + 1].tokenClass == Label_T && program.tokens[index + 2].tokenClass == For_T)
		{
			tmpAtom.atr2 = *(string*)program.nameTable[program.tokens[index + 1].value].namePointer;
			declaredLabels[tmpAtom.atr2] = -1;
		}
		else if (program.tokens[index + 1].tokenClass == Label_T)
		{
			tmpAtom.classAtom = Atom::Label;
			tmpAtom.atr1 = *(string*)program.nameTable[program.tokens[index + 1].value].namePointer;

			declaredLabels[tmpAtom.atr1] = -1; // Объявление пока пустой метки

			atomsTable.push_back(tmpAtom);
			tmpAtom.classAtom = Atom::None;
		}
		else if (program.tokens[index - 1].tokenClass == Constant_T || program.tokens[index - 1].tokenClass == LongConstant_T || program.tokens[index - 1].tokenClass == Default_T)
		{
			atomsTable.push_back(tmpAtom);
			tmpAtom.classAtom = Atom::None;
			tmpAtom.atr5.clear();
		}
		break;
	}
	case Goto_T:
	{
		if (tmpAtom.classAtom == Atom::If || tmpAtom.classAtom == Atom::IfElse)
			break;

		tmpAtom.classAtom = Atom::Goto;
		break;
	}
	case If_T: 
	{
		tmpAtom.classAtom = Atom::If;
		break;
	}
	case Else_T:
	{
		tmpAtom.classAtom = Atom::IfElse;
		break;
	}
	case For_T:
	{
		toFlag = false;
		tmpAtom.classAtom = Atom::For;
		break;
	}
	case To_T: toFlag = true; break;
	case Adding_T: tmpAtom.classAtom = Atom::ForAdding; break;
	case Next_T:
	{
		if (program.tokens[index + 1].tokenClass == Label_T)
		{
			tmpAtom.classAtom = Atom::Next;
			tmpAtom.atr1 = *(string*)program.nameTable[program.tokens[index + 1].value].namePointer;
		}
		break;
	}
	case Switch_T:
	{
		if (program.tokens[index + 1].tokenClass != Semicolon_T)
		{
			tmpAtom.classAtom = Atom::Switch;
		}
		break;
	}
	case Case_T:
	{
		tmpAtom.classAtom = Atom::Case;
		break;
	}
	case Break_T:
	{
		tmpAtom.classAtom = Atom::Break;
		break;
	}
	case Default_T:
	{
		tmpAtom.classAtom = Atom::Default;
		break;
	}
	case End_T:
	{
		tmpAtom.classAtom = Atom::EndSwitch;
		break;
	}
	case Fail_T:
	{
		tmpAtom.classAtom = Atom::Fail;
		break;
	}
	case CmpOp_T:
	{
		cmpFlag = true;

		switch (tok.value)
		{
		case Equal: tmpAtom.atr3 = "="; break;
		case NotEqual: tmpAtom.atr3 = "!="; break;
		case Less: tmpAtom.atr3 = "<"; break;
		case Bigger: tmpAtom.atr3 = ">"; break;
		case LessOrEqual: tmpAtom.atr3 = "<="; break;
		case BiggerOrEqual: tmpAtom.atr3 = ">="; break;
		}
		break;
	}
	case Variable_T:
	{
		if (tmpAtom.classAtom != Atom::Declare)
		{
			if (declaredVars.find(*(string*)program.nameTable[tok.value].namePointer) == declaredVars.end())
				throw 1;
		}

		switch (tmpAtom.classAtom)
		{
		case Atom::Declare:
		{
			if (program.tokens[index + 2].tokenClass == Int_T)
			{
				declaredVars[*(string*)program.nameTable[tok.value].namePointer] = ValueType::Integer;
			}
			else if (program.tokens[index + 2].tokenClass == Long_T)
			{
				declaredVars[*(string*)program.nameTable[tok.value].namePointer] = ValueType::LongNat;
			}
			break;
		}
		case Atom::Set:
		{
			if (program.tokens[index + 1].tokenClass == Comma_T || program.tokens[index + 1].tokenClass == To_T)
			{
				if (digitFlag)
				{
					tmpAtom.atr5.push_back(Token(Digit_T, 0, 0));
					tmpAtom.atr5.push_back(tok);
				}
				else
					tmpAtom.atr4.push_back(tok);
			}
			else
			{
				if (digitFlag)
				{
					tmpAtom.atr5.push_back(Token(Digit_T, 0, 0));
					tmpAtom.atr5.push_back(tok);
				}
				else
					tmpAtom.atr5.push_back(tok);
			}
			break;
		}
		case Atom::If:
		{
			if (!cmpFlag)
			{
				if (digitFlag)
				{
					tmpAtom.atr4.push_back(Token(Digit_T, 0, 0));
					tmpAtom.atr4.push_back(tok);
				}
				else
					tmpAtom.atr4.push_back(tok);
			}
			else
			{
				if (digitFlag)
				{
					tmpAtom.atr5.push_back(Token(Digit_T, 0, 0));
					tmpAtom.atr5.push_back(tok);
				}
				else
					tmpAtom.atr5.push_back(tok);
			}
			break;
		}
		case Atom::For:
		{
			if (program.tokens[index - 1].tokenClass == For_T)
			{
				tmpAtom.atr1 = *(string*)program.nameTable[tok.value].namePointer;
			}
			else if (toFlag)
			{
				if (digitFlag)
				{
					tmpAtom.atr5.push_back(Token(Digit_T, 0, 0));
					tmpAtom.atr5.push_back(tok);
				}
				else
					tmpAtom.atr5.push_back(tok);

				if (program.tokens[index + 1].tokenClass != LBracket_T && program.tokens[index + 1].tokenClass != RBracket_T && program.tokens[index + 1].tokenClass != ArithmeticOp_T && 
					program.tokens[index + 1].tokenClass != Adding_T && program.tokens[index + 1].tokenClass != Comma_T)
				{
					atomsTable.push_back(tmpAtom);
					tmpAtom.classAtom = Atom::None;
					tmpAtom.atr4.clear();
					tmpAtom.atr5.clear();
				}
			}
			else
			{
				if (digitFlag)
				{
					tmpAtom.atr4.push_back(Token(Digit_T, 0, 0));
					tmpAtom.atr4.push_back(tok);
				}
				else
					tmpAtom.atr4.push_back(tok);
			}

			break;
		}
		case Atom::ForAdding:
		{
			if (digitFlag)
			{
				tmpAtom.atr6.push_back(Token(Digit_T, 0, 0));
				tmpAtom.atr6.push_back(tok);
			}
			else
				tmpAtom.atr6.push_back(tok);

			if (program.tokens[index + 1].tokenClass != LBracket_T && program.tokens[index + 1].tokenClass != RBracket_T 
				&& program.tokens[index + 1].tokenClass != ArithmeticOp_T && program.tokens[index + 1].tokenClass != Comma_T)
			{
				atomsTable.push_back(tmpAtom);
				tmpAtom.classAtom = Atom::None;
				tmpAtom.atr4.clear();
				tmpAtom.atr5.clear();
				tmpAtom.atr6.clear();
			}

			break;
		}
		case Atom::Switch:
		{
			if (digitFlag)
			{
				tmpAtom.atr4.push_back(Token(Digit_T, 0, 0));
				tmpAtom.atr4.push_back(tok);
			}
			else
				tmpAtom.atr4.push_back(tok);
			break;
		}
		default:
		{
			if (digitFlag)
			{
				tmpAtom.atr4.push_back(Token(Digit_T, 0, 0));
				tmpAtom.atr4.push_back(tok);
			}
			else
				tmpAtom.atr4.push_back(tok);
			break;
		}
		}
		break;
	}
	case Label_T:
	{
		if (program.tokens[index - 1].tokenClass == Colon_T)
			break;

		switch (tmpAtom.classAtom)
		{
		case Atom::If:
		{
			tmpAtom.atr1 = *(string*)program.nameTable[tok.value].namePointer;
			break;
		}
		case Atom::IfElse:
		{
			tmpAtom.atr2 = *(string*)program.nameTable[tok.value].namePointer;
			break;
		}
		case Atom::Next:
		{
			tmpAtom.atr1 = *(string*)program.nameTable[tok.value].namePointer;
			atomsTable.push_back(tmpAtom);
			tmpAtom.classAtom = Atom::None;
			break;
		}
		case Atom::Goto:
		{
			tmpAtom.atr1 = *(string*)program.nameTable[tok.value].namePointer;
			break;
		}
		}
		break;
	}
	case Constant_T:
	{
		switch (tmpAtom.classAtom)
		{
		case Atom::Set:
		{
			tmpAtom.atr5.push_back(tok);
			break;
		}
		case Atom::If:
		{
			if (!cmpFlag)
				tmpAtom.atr4.push_back(tok);
			else
				tmpAtom.atr5.push_back(tok);
			break;
		}
		case Atom::For:
		{
			if (toFlag)
			{
				tmpAtom.atr5.push_back(tok);

				if (program.tokens[index + 1].tokenClass != LBracket_T && program.tokens[index + 1].tokenClass != RBracket_T && program.tokens[index + 1].tokenClass != ArithmeticOp_T &&
					program.tokens[index + 1].tokenClass != Adding_T)
				{
					atomsTable.push_back(tmpAtom);
					tmpAtom.classAtom = Atom::None;
					tmpAtom.atr4.clear();
					tmpAtom.atr5.clear();
				}
			}
			else
				tmpAtom.atr4.push_back(tok);

			break;
		}
		case Atom::ForAdding:
		{
			tmpAtom.atr6.push_back(tok);

			if (program.tokens[index + 1].tokenClass != LBracket_T && program.tokens[index + 1].tokenClass != RBracket_T 
				&& program.tokens[index + 1].tokenClass != ArithmeticOp_T)
			{
				atomsTable.push_back(tmpAtom);
				tmpAtom.classAtom = Atom::None;
				tmpAtom.atr4.clear();
				tmpAtom.atr5.clear();
				tmpAtom.atr6.clear();
			}

			break;
		}
		case Atom::Switch:
		{
			tmpAtom.atr4.push_back(tok);
			break;
		}
		case Atom::Case:
		{
			tmpAtom.atr5.push_back(tok);
			break;
		}
		default:
		{
			tmpAtom.atr4.push_back(tok);
			break;
		}
		}
		break;
	}
	case LongConstant_T:
	{
		switch (tmpAtom.classAtom)
		{
		case Atom::Set:
		{
			tmpAtom.atr5.push_back(tok);
			break;
		}
		case Atom::If:
		{
			if (!cmpFlag)
				tmpAtom.atr4.push_back(tok);
			else
				tmpAtom.atr5.push_back(tok);
			break;
		}
		case Atom::For:
		{
			if (toFlag)
			{
				tmpAtom.atr5.push_back(tok);

				if (program.tokens[index + 1].tokenClass != LBracket_T && program.tokens[index + 1].tokenClass != RBracket_T && program.tokens[index + 1].tokenClass != ArithmeticOp_T &&
					program.tokens[index + 1].tokenClass != Adding_T)
				{
					atomsTable.push_back(tmpAtom);
					tmpAtom.classAtom = Atom::None;
					tmpAtom.atr4.clear();
					tmpAtom.atr5.clear();
				}
			}
			else
				tmpAtom.atr4.push_back(tok);

			break;
		}
		case Atom::ForAdding:
		{
			tmpAtom.atr6.push_back(tok);

			if (program.tokens[index + 1].tokenClass != LBracket_T && program.tokens[index + 1].tokenClass != RBracket_T 
				&& program.tokens[index + 1].tokenClass != ArithmeticOp_T)
			{
				atomsTable.push_back(tmpAtom);
				tmpAtom.classAtom = Atom::None;
				tmpAtom.atr4.clear();
				tmpAtom.atr5.clear();
				tmpAtom.atr6.clear();
			}

			break;
		}
		case Atom::Switch:
		{
			tmpAtom.atr4.push_back(tok);
			break;
		}
		case Atom::Case:
		{
			tmpAtom.atr5.push_back(tok);
			break;
		}
		default:
		{
			tmpAtom.atr4.push_back(tok);
			break;
		}
		}
		break;
	}
	case LBracket_T:
	{
		if (digitFlag)
			break;

		switch (tmpAtom.classAtom)
		{
		case Atom::Set:
		{
			tmpAtom.atr5.push_back(tok);
			break;
		}
		case Atom::If:
		{
			if (!cmpFlag)
				tmpAtom.atr4.push_back(tok);
			else
				tmpAtom.atr5.push_back(tok);
			break;
		}
		case Atom::For:
		{
			if (toFlag)
				tmpAtom.atr5.push_back(tok);
			else
				tmpAtom.atr4.push_back(tok);

			break;
		}
		case Atom::ForAdding:
		{
			tmpAtom.atr6.push_back(tok);
			break;
		}
		case Atom::Switch:
		{
			if (program.tokens[index - 1].tokenClass == Switch_T)
				break;

			tmpAtom.atr4.push_back(tok);
			break;
		}
		default:
		{
			tmpAtom.atr4.push_back(tok);
			break;
		}
		}
		break;
	}
	case RBracket_T:
	{
		if (digitFlag)
		{
			if (tmpAtom.classAtom != Atom::For && tmpAtom.classAtom != Atom::ForAdding)
			{
				digitFlag = false;
				break;
			}
		}
		
		switch (tmpAtom.classAtom)
		{
		case Atom::Set:
		{
			tmpAtom.atr5.push_back(tok);
			break;
		}
		case Atom::If:
		{
			if (!cmpFlag)
				tmpAtom.atr4.push_back(tok);
			else
				tmpAtom.atr5.push_back(tok);
			break;
		}
		case Atom::For:
		{
			if (toFlag)
			{
				if (!digitFlag)
					tmpAtom.atr5.push_back(tok);

				if (program.tokens[index + 1].tokenClass != ArithmeticOp_T && program.tokens[index + 1].tokenClass != Adding_T)
				{
					atomsTable.push_back(tmpAtom);
					tmpAtom.classAtom = Atom::None;
					tmpAtom.atr4.clear();
					tmpAtom.atr5.clear();
				}
			}
			else if (!digitFlag)
				tmpAtom.atr4.push_back(tok);

			digitFlag = false;
			break;
		}
		case Atom::ForAdding:
		{
			if (!digitFlag)
				tmpAtom.atr6.push_back(tok);

			if (program.tokens[index + 1].tokenClass != ArithmeticOp_T)
			{
				atomsTable.push_back(tmpAtom);
				tmpAtom.classAtom = Atom::None;
				tmpAtom.atr4.clear();
				tmpAtom.atr5.clear();
				tmpAtom.atr6.clear();
			}

			digitFlag = false;
			break;
		}
		case Atom::Switch:
		{
			if (program.tokens[index + 1].tokenClass == Case_T)
			{
				atomsTable.push_back(tmpAtom);
				tmpAtom.classAtom = Atom::None;
				tmpAtom.atr4.clear();
				break;
			}
				
			tmpAtom.atr4.push_back(tok);
			break;
		}
		default:
		{
			tmpAtom.atr4.push_back(tok);
			break;
		}
		}
		break;
	}
	case ArithmeticOp_T:
	{
		switch (tmpAtom.classAtom)
		{
		case Atom::Set:
		{
			tmpAtom.atr5.push_back(tok);
			break;
		}
		case Atom::If:
		{
			if (!cmpFlag)
				tmpAtom.atr4.push_back(tok);
			else
				tmpAtom.atr5.push_back(tok);
			break;
		}
		case Atom::For:
		{
			if (toFlag)
				tmpAtom.atr5.push_back(tok);
			else
				tmpAtom.atr4.push_back(tok);

			break;
		}
		case Atom::ForAdding:
		{
			tmpAtom.atr6.push_back(tok);
			break;
		}
		case Atom::Switch:
		{
			tmpAtom.atr4.push_back(tok);
			break;
		}
		default:
		{
			tmpAtom.atr4.push_back(tok);
			break;
		}
		}
		break;
	}
	case Digit_T: digitFlag = true; break;
	case Semicolon_T:
	{
		atomsTable.push_back(tmpAtom);
		tmpAtom.classAtom = Atom::None;
		tmpAtom.atr1.clear();
		tmpAtom.atr2.clear();
		tmpAtom.atr3.clear();
		tmpAtom.atr4.clear();
		tmpAtom.atr5.clear();
		tmpAtom.atr6.clear();
		cmpFlag = false;
		break;
	}
	}
}

bool ParserSynt::run(ParsedProgramLex& program)
{
	list<int> tmpList;
	el_stack tmpEl;
	bool lastBigger = false;
	cmpFlag = false;
	digitFlag = false;

	tmpEl.el_rule = StackEnd;
	stack.push_back(tmpEl);

	// Проход по входному слову (токенам)
	for (int i = 0; i < program.tokens.size(); i++)
	{
		int inputEl = program.tokens[i].tokenClass;
		// Трансляция некоторых токенов в терминалы
		if (inputEl >= 30) TranslateToken(inputEl, program.tokens[i].value);
		
		try
		{	// Трансляция текущего элемента в атом и получение атрибутов
			if (!lastBigger) TranslateToAtom(program.tokens[i], i, program);
		}
		catch (...)
		{
			cout << "Ошибка при получении атомов или атрибутов\n";
			return false;
		}

		// Проверка на допуск
		if (stack.size() == 2 && stack.back().el_rule == 40 && inputEl == 78)
			return true;


		// Перенос если равно или меньше
		if (compareTable[stack.back().el_rule][inputEl] == ComparePrecedence::Equal)
		{
			tmpEl.el_rule = inputEl;
			tmpEl.cmp = ComparePrecedence::Equal;
			stack.push_back(tmpEl);
			lastBigger = false;
		}
		else if (compareTable[stack.back().el_rule][inputEl] == ComparePrecedence::Less)
		{
			tmpEl.el_rule = inputEl;
			tmpEl.cmp = ComparePrecedence::Less;
			stack.push_back(tmpEl);
			lastBigger = false;

		} // Свертка если больше
		else if (compareTable[stack.back().el_rule][inputEl] == ComparePrecedence::Bigger)
		{
			// Копируем правило до меньше
			for (int j = stack.size() - 1; stack[j].cmp != ComparePrecedence::Less; j--)
			{
				tmpList.push_front(stack[j].el_rule);
				stack.pop_back();
			}

			// Помещаем элемент с меньше
			tmpList.push_front(stack.back().el_rule);
			stack.pop_back();

			// Копируем правило из листа в вектор
			for (int& m : tmpList)
			{
				ruleVec.push_back(m);
			}

			if (!this->GetRuleHead())
				return false;

			tmpList.clear();
			ruleVec.clear();
			i--;
			lastBigger = true;
		}
		else  // Синтаксическая ошибка в программе
			return false;
	}
}

 // Приоритеты арифметических операций
int getPriority(int symb)
{
	switch (symb)
	{
	case '*': return 3;
	case '/': return 3;
	case '%': return 3;
	case '+': return 2;
	case '-': return 2;
	default: return 1;
	}
}

// Перевод "обычных" выражений в выражения с обратной польской записью
int ParserSynt::TranslateExpression(std::vector<Token>& vect)
{
	vector<Token> sourceVec(vect);
	std::stack<Token> tmpStack;
	vect.clear();

	int size = 0;
	
	for (int i = 0; i < sourceVec.size(); i++)
	{
		switch (sourceVec[i].tokenClass)
		{
		case Variable_T:
		{
			vect.push_back(sourceVec[i]);
			size++;
			break;
		}
		case Constant_T:
		{
			vect.push_back(sourceVec[i]);
			size++;
			break;
		}
		case LongConstant_T:
		{
			vect.push_back(sourceVec[i]);
			size++;
			break;
		}
		case Digit_T:
		{
			vect.push_back(sourceVec[i]);
			vect.push_back(sourceVec[i + 1]);
			vect.push_back(sourceVec[i + 2]);
			i = i + 2;
			size++;
			break;
		}
		case ArithmeticOp_T:
		{
			if (tmpStack.empty())
			{
				tmpStack.push(sourceVec[i]);
				size++;
				break;
			}

			while (!tmpStack.empty() && getPriority(tmpStack.top().value) >= getPriority(sourceVec[i].value))
			{
				vect.push_back(tmpStack.top());
				tmpStack.pop();
			}

			tmpStack.push(sourceVec[i]);
			size++;

			break;
		}
		case LBracket_T:
		{
			tmpStack.push(sourceVec[i]);
			break;
		}
		case RBracket_T:
		{
			while (tmpStack.top().tokenClass != LBracket_T)
			{
				vect.push_back(tmpStack.top());
				tmpStack.pop();
			}

			tmpStack.pop();
			break;
		}
		}
	}

	while (!tmpStack.empty())
	{
		vect.push_back(tmpStack.top());
		tmpStack.pop();
	}

	return size;
}

// Печать выражений из полученной обратной польской записи в стэковый язык
bool ParserSynt::PrintExpression(std::vector<Token>& vect, std::ofstream& out, ParsedProgramLex& program)
{

	for (int i = 0; i < vect.size(); i++)
	{
		switch (vect[i].tokenClass)
		{
		case Variable_T:
		{
			if (isInt || isLong)
			{
				if (declaredVars[*(string*)program.nameTable[vect[i].value].namePointer] == ValueType::Integer && isLong == true)
					return false;
				else if (declaredVars[*(string*)program.nameTable[vect[i].value].namePointer] == ValueType::LongNat && isInt == true)
					return false;
			}
			
			out << "push " << *(string*)program.nameTable[vect[i].value].namePointer << "\n";
			break;
		}
		case Constant_T:
		{
			if (isLong)
				return false;

			out << "push " << *(int*)program.nameTable[vect[i].value].namePointer << "\n";
			break;
		}
		case LongConstant_T:
		{
			if (isInt)
				return false;

			out << "push $" << *(LongNat*)program.nameTable[vect[i].value].namePointer << "\n";
			break;
		}
		case Digit_T:
		{
			if (isLong) // результат digit это число типа int
				return false;

			if (declaredVars[*(string*)program.nameTable[vect[i + 1].value].namePointer] == ValueType::Integer)
				return false;

			out << "dig " << *(string*)program.nameTable[vect[i + 1].value].namePointer << ", ";

			if (program.nameTable[vect[i + 2].value].type == ObjectType::Constant)
				out << *(int*)program.nameTable[vect[i + 2].value].namePointer << "\n";
			else
				out << *(LongNat*)program.nameTable[vect[i + 2].value].namePointer << "\n";

			i = i + 2;
			break;
		}
		case ArithmeticOp_T:
		{
			out << (char)vect[i].value << "\n";
			break;
		}
		}
	}
	return true;
}

// Нахождение строки первой стэковой команды для всех атомов
void ParserSynt::CountLines()
{
	int curStr = 1;

	for (int i = 0; i < atomsTable.size(); i++)
	{
		atomLine[i] = curStr;

		switch (atomsTable[i].classAtom)
		{
		case Atom::Declare: break;
		case Atom::CinL: curStr = curStr + 2; break;
		case Atom::CinI: curStr = curStr + 2; break;
		case Atom::Cout:
		{
			curStr = curStr + TranslateExpression(atomsTable[i].atr4) + 1;
			break;
		}
		case Atom::Set:
		{
			curStr = curStr + atomsTable[i].atr4.size() * 2 + TranslateExpression(atomsTable[i].atr5) + 1;
			break;
		}
		case Atom::Label: curStr++; break;
		case Atom::If:
		{
			curStr = curStr + TranslateExpression(atomsTable[i].atr4) + TranslateExpression(atomsTable[i].atr5) + 2;
			break;
		}
		case Atom::IfElse:
		{
			curStr = curStr + TranslateExpression(atomsTable[i].atr4) + TranslateExpression(atomsTable[i].atr5) + 3;
			break;
		}
		case Atom::Goto: curStr++; break;
		case Atom::For:
		{
			curStr = curStr + TranslateExpression(atomsTable[i].atr4) + TranslateExpression(atomsTable[i].atr5) + 11;
			break;
		}
		case Atom::ForAdding:
		{
			curStr = curStr + TranslateExpression(atomsTable[i].atr4) + TranslateExpression(atomsTable[i].atr5) + TranslateExpression(atomsTable[i].atr6) + 10;
			break;
		}
		case Atom::Next: curStr++; break;
		case Atom::Switch:
		{
			curStr = curStr + TranslateExpression(atomsTable[i].atr4) + 1;
			break;
		}
		case Atom::Case: curStr = curStr + 4; break;
		case Atom::Break: curStr++; break;
		case Atom::Default: curStr++; break;
		case Atom::EndSwitch: curStr++; break;
		case Atom::Fail: curStr++; break;
		}
	}

	endLine = curStr;
}

// Перевод атомов и печать их на стэковый язык
bool ParserSynt::TranslateToStackLang(string filename, ParsedProgramLex& program)
{
	ofstream out(filename);

	if (!out)
	{
		cout << "Не удалось открыть файл " << filename << endl;
		return false;
	}

	list<string> switchTechNames;

	this->CountLines();

	for (int i = 0; i < atomsTable.size(); i++)
	{
		switch (atomsTable[i].classAtom)
		{
		case Atom::Declare: break;
		case Atom::CinL:
		{
			out << "rlong\n";
			out << "pop " << atomsTable[i].atr1 << "\n";
			break;
		}
		case Atom::CinI:
		{
			out << "read\n";
			out << "pop " << atomsTable[i].atr1 << "\n";
			break;
		}
		case Atom::Cout:
		{
			if (!PrintExpression(atomsTable[i].atr4, out, program))
				return false;

			out << "write\n";
			break;
		}
		case Atom::Set:
		{
			// Проверка, все ли переменные одинакового типа

			if (declaredVars[*(string*)program.nameTable[atomsTable[i].atr4[0].value].namePointer] == ValueType::Integer)
				isInt = true;
			else
				isLong = true;

			for (Token& tok : atomsTable[i].atr4)
			{
				if (declaredVars[*(string*)program.nameTable[tok.value].namePointer] == ValueType::Integer && isLong == true)
					return false;
				else if (declaredVars[*(string*)program.nameTable[tok.value].namePointer] == ValueType::LongNat && isInt == true)
					return false;
			}
				// В функции проверяется также тип выражения
			if (!PrintExpression(atomsTable[i].atr5, out, program))
				return false;

			out << "pop setTechVar\n";

			for (int j = 0; j < atomsTable[i].atr4.size(); j++)
			{
				out << "push setTechVar\n";
				out << "pop " << *(string*)program.nameTable[atomsTable[i].atr4[j].value].namePointer << "\n";
			}
			break;
		}
		case Atom::Label:
		{
			declaredLabels[atomsTable[i].atr1] = atomLine[i];
			out << ";\n";
			break;
		}
		case Atom::If:
		{
			if (!PrintExpression(atomsTable[i].atr4, out, program))
				return false;
			if (!PrintExpression(atomsTable[i].atr5, out, program))
				return false;

			out << atomsTable[i].atr3 << "\n";

			if (declaredLabels[atomsTable[i].atr1] != -1 && declaredLabels[atomsTable[i].atr1] != 0)
			{
				out << "ji " << declaredLabels[atomsTable[i].atr1] << "\n";
				break;
			}

			bool isNotDecl = true;

			// если метка не объявлена ранее
			for (int j = i; j < atomsTable.size(); j++)
			{
				if (atomsTable[j].classAtom == Atom::Label && atomsTable[i].atr1 == atomsTable[j].atr1)
				{
					out << "ji " << atomLine[j] << "\n";
					isNotDecl = false;
					break;
				}
				else if (atomsTable[j].classAtom == Atom::For && atomsTable[i].atr1 == atomsTable[j].atr2)
				{
					out << "ji " << atomLine[j] << "\n";
					isNotDecl = false;
					break;
				}
				else if (atomsTable[j].classAtom == Atom::ForAdding && atomsTable[i].atr1 == atomsTable[j].atr2)
				{
					out << "ji " << atomLine[j] << "\n";
					isNotDecl = false;
					break;
				}
			}
			// переход на не существующую метку, ошибка
			if (isNotDecl)
				return false;
			break;
		}
		case Atom::IfElse:
		{
			if (!PrintExpression(atomsTable[i].atr4, out, program))
				return false;
			if (!PrintExpression(atomsTable[i].atr5, out, program))
				return false;

			out << atomsTable[i].atr3 << "\n";

			if (declaredLabels[atomsTable[i].atr1] != -1 && declaredLabels[atomsTable[i].atr1] != 0)
			{
				out << "ji " << declaredLabels[atomsTable[i].atr1] << "\n";
			}
			else // если первая метка не объявлена ранее
			{
				bool isNotDecl_1 = true;
				for (int j = i; j < atomsTable.size(); j++)
				{
					if (atomsTable[j].classAtom == Atom::Label && atomsTable[i].atr1 == atomsTable[j].atr1)
					{
						out << "ji " << atomLine[j] << "\n";
						isNotDecl_1 = false;
						break;
					}
					else if (atomsTable[j].classAtom == Atom::For && atomsTable[i].atr1 == atomsTable[j].atr2)
					{
						out << "ji " << atomLine[j] << "\n";
						isNotDecl_1 = false;
						break;
					}
					else if (atomsTable[j].classAtom == Atom::ForAdding && atomsTable[i].atr1 == atomsTable[j].atr2)
					{
						out << "ji " << atomLine[j] << "\n";
						isNotDecl_1 = false;
						break;
					}
				}

				if (isNotDecl_1) // Если первой метки нет
					return false;
			}

			if (declaredLabels[atomsTable[i].atr2] != -1 && declaredLabels[atomsTable[i].atr2] != 0)
			{
				out << "jmp " << declaredLabels[atomsTable[i].atr2] << "\n";
				break;
			}

			bool isNotDecl_2 = true;
			// если вторая метка не объявлена ранее
			for (int j = i; j < atomsTable.size(); j++)
			{
				if (atomsTable[j].classAtom == Atom::Label && atomsTable[i].atr2 == atomsTable[j].atr1)
				{
					out << "jmp " << atomLine[j] << "\n";
					isNotDecl_2 = false;
					break;
				}
				else if (atomsTable[j].classAtom == Atom::For && atomsTable[i].atr2 == atomsTable[j].atr2)
				{
					out << "jmp " << atomLine[j] << "\n";
					isNotDecl_2 = false;
					break;
				}
				else if (atomsTable[j].classAtom == Atom::ForAdding && atomsTable[i].atr2 == atomsTable[j].atr2)
				{
					out << "jmp " << atomLine[j] << "\n";
					isNotDecl_2 = false;
					break;
				}
			}

			if (isNotDecl_2) // Если второй метки нет
				return false;
			break;
		}
		case Atom::Goto:
		{
			if (declaredLabels[atomsTable[i].atr1] != -1 && declaredLabels[atomsTable[i].atr1] != 0)
			{
				out << "jmp " << declaredLabels[atomsTable[i].atr1] << "\n";
				break;
			}
			
			bool isNotDecl = true;

			// если метка не объявлена ранее
			for (int j = i; j < atomsTable.size(); j++)
			{
				if (atomsTable[j].classAtom == Atom::Label && atomsTable[i].atr1 == atomsTable[j].atr1)
				{
					out << "jmp " << atomLine[j] << "\n";
					isNotDecl = false;
					break;
				}
				else if (atomsTable[j].classAtom == Atom::For && atomsTable[i].atr1 == atomsTable[j].atr2)
				{
					out << "jmp " << atomLine[j] << "\n";
					isNotDecl = false;
					break;
				}
				else if (atomsTable[j].classAtom == Atom::ForAdding && atomsTable[i].atr1 == atomsTable[j].atr2)
				{
					out << "jmp " << atomLine[j] << "\n";
					isNotDecl = false;
					break;
				}
			}
			// переход на не существующую метку, ошибка
			if (isNotDecl)
				return false;
			break;
		}
		case Atom::For:
		{
			// проверка типа переменной параметра
			if (declaredVars[atomsTable[i].atr1] == ValueType::Integer)
				isInt = true;
			else
				isLong = true;


			if (!PrintExpression(atomsTable[i].atr4, out, program))
				return false;

			int sizeAtr4 = 0; // размер с учетом что digit занимает 1 стр вместо 3
			for (int j = 0; j < atomsTable[i].atr4.size(); j++)
			{
				sizeAtr4++;
				if (atomsTable[i].atr4[j].tokenClass == Digit_T)
					j = j + 2;
			}

			declaredLabels[atomsTable[i].atr2] = atomLine[i] + sizeAtr4 + 2;

			out << "pop " << atomsTable[i].atr1 << "\n";
			out << "jmp " << declaredLabels[atomsTable[i].atr2] + 5 << "\n"; // в первый проход пропуск adding
			out << ";\n"; // переход сюда после next

			if (isInt)
				out << "push 1\n";
			else if (isLong)
				out << "push $1\n";
		
			out << "push " << atomsTable[i].atr1 << "\n";
			out << "+\n";
			out << "pop " << atomsTable[i].atr1 << "\n";
			out << ";\n";

			if (!PrintExpression(atomsTable[i].atr5, out, program))
				return false;

			out << "push " << atomsTable[i].atr1 << "\n";
			out << "=\n";

			bool nextNotDecl = true;
			// считаем строчки до атома next
			for (int j = i + 1; j < atomsTable.size(); j++)
			{
				if (atomsTable[j].classAtom == Atom::Next && atomsTable[i].atr2 == atomsTable[j].atr1)
				{
					out << "ji " << atomLine[j] + 1 << "\n"; // Выход из цикла
					nextNotDecl = false;
					break;
				}
			}

			if (nextNotDecl)
				return false;
			break;
		}
		case Atom::ForAdding:
		{
			// проверка типа переменной параметра
			if (declaredVars[atomsTable[i].atr1] == ValueType::Integer)
				isInt = true;
			else
				isLong = true;


			if (!PrintExpression(atomsTable[i].atr4, out, program))
				return false;

			int sizeAtr4 = 0;
			for (int j = 0; j < atomsTable[i].atr4.size(); j++)
			{
				sizeAtr4++;
				if (atomsTable[i].atr4[j].tokenClass == Digit_T)
					j = j + 2;
			}

			int sizeAtr6 = 0;
			for (int j = 0; j < atomsTable[i].atr6.size(); j++)
			{
				sizeAtr6++;
				if (atomsTable[i].atr6[j].tokenClass == Digit_T)
					j = j + 2;
			}

			declaredLabels[atomsTable[i].atr2] = atomLine[i] + sizeAtr4 + 2;

			out << "pop " << atomsTable[i].atr1 << "\n";
			out << "jmp " << declaredLabels[atomsTable[i].atr2] + sizeAtr6 + 4 << "\n"; // в первый проход пропуск adding
			out << ";\n"; // переход сюда после next

			if (!PrintExpression(atomsTable[i].atr6, out, program))
				return false;

			out << "push " << atomsTable[i].atr1 << "\n";
			out << "+\n";
			out << "pop " << atomsTable[i].atr1 << "\n";
			out << ";\n";

			if (!PrintExpression(atomsTable[i].atr5, out, program))
				return false;

			out << "push " << atomsTable[i].atr1 << "\n";
			out << "=\n";

			// считаем строчки до атома next
			for (int j = i + 1; j < atomsTable.size(); j++)
			{
				if (atomsTable[j].classAtom == Atom::Next && atomsTable[i].atr2 == atomsTable[j].atr1)
				{
					out << "ji " << atomLine[j] + 1 << "\n"; // Выход из цикла
				}
			}
			break;
		}
		case Atom::Next:
		{
			out << "jmp " << declaredLabels[atomsTable[i].atr1] << "\n";
			break;
		}
		case Atom::Switch:
		{
			if(!PrintExpression(atomsTable[i].atr4, out, program))
				return false;

			string name = (string)"switchTechVar" + std::to_string(switchTechNames.size());

			switchTechNames.push_back(name);

			out << "pop " << switchTechNames.back() << "\n";
			break;
		}
		case Atom::Case:
		{
			out << "push " << switchTechNames.back() << "\n";

			if (program.nameTable[atomsTable[i].atr5.back().value].type == ObjectType::Constant)
				out << "push " << *(int*)program.nameTable[atomsTable[i].atr5.back().value].namePointer << "\n";
			else
				out << "push $" << *(LongNat*)program.nameTable[atomsTable[i].atr5.back().value].namePointer << "\n";

			out << "!=\n";

			for (int j = i + 1; j < atomsTable.size(); j++)
			{
				if (atomsTable[j].classAtom == Atom::Switch)
				{
					while (atomsTable[j].classAtom != Atom::EndSwitch)
						j++;
					continue;
				}

				if (atomsTable[j].classAtom == Atom::Case || atomsTable[j].classAtom == Atom::Default)
				{
					out << "ji " << atomLine[j] << "\n"; // на другой кейс или дефолт
					break;
				}
			}
			break;
		}
		case Atom::Break:
		{
			for (int j = i + 1; j < atomsTable.size(); j++)
			{
				if (atomsTable[j].classAtom == Atom::Switch)
				{
					while (atomsTable[j].classAtom != Atom::EndSwitch)
						j++;
					continue;
				}

				if (atomsTable[j].classAtom == Atom::EndSwitch)
				{
					out << "jmp " << atomLine[j] << "\n"; // в конец свича
					break;
				}
			}
			break;
		}
		case Atom::Default:
		{
			out << ";\n"; // метка
			break;
		}
		case Atom::EndSwitch:
		{
			switchTechNames.pop_back();
			out << ";\n"; // метка куда прыгают после отработки
			break;
		}
		case Atom::Fail:
		{
			out << "jmp " << endLine << "\n"; // переход на end
			break;
		}
		}
		isInt = false;
		isLong = false;
	}

	out << "end\n";

	out.close();

	return true;
}