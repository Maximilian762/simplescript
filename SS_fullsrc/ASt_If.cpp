#include "stdafx.h"
#include "ASt_If.h"
#include "Variable.h"
using namespace std;

ASt_If::ASt_If()
{
	SetType(AST_Type::If);
}


ASt_If::~ASt_If()
{
}

void ASt_If::SetCondition(SharedExpr _ast_cond)
{
	ast_cond = _ast_cond;
}

void ASt_If::SetCode(std::vector<SharedExpr> _code)
{
	code = _code;
}

void ASt_If::SetElseCode(std::vector<SharedExpr> _code)
{
	else_code = _code;
}

void ASt_If::PrintSelf(int level)
{
	PrintAlignment(level);
	cout << "IF" << endl;

	PrintAlignment(level+1);
	cout << "cond:" << endl;

	if (ast_cond) ast_cond->PrintSelf(level + 2);
	else
	{
		PrintAlignment(level + 2);
		cout << "<emptycondition>" << endl;
	}

	PrintAlignment(level + 1);
	cout << "body:" << endl;

	for (auto &expr : code)
	{
		if (expr) expr->PrintSelf(level + 2);
		else
		{
			PrintAlignment(level + 2);
			cout << "<emptyexpr>" << endl;
		}
	}

	if (!else_code.empty())
	{
		PrintAlignment(level + 1);
		cout << "else body:" << endl;

		for (auto &expr : else_code)
		{
			if (expr) expr->PrintSelf(level + 2);
			else
			{
				PrintAlignment(level + 2);
				cout << "<emptyexpr>" << endl;
			}
		}
	}
}

bool ASt_If::Evaluate(Table & obj, VarPtr &return_var, IMsgHandler &errhandler)
{
	VarPtr res;
	VarPtr cmpresult;

	if (!ast_cond) errhandler.OnMsg("If has empty condition");

	if (ast_cond->Evaluate(obj, cmpresult, errhandler))
	{
		bool condition = cmpresult->GetIntRef() != 0;

		std::vector<SharedExpr> &codebranch = condition ? code : else_code;

		for (auto &expr : codebranch)
		{
			if (!expr)
			{
				errhandler.OnMsg("Empty expr in if body");
				return false;
			}

			bool success = expr->Evaluate(obj, res, errhandler);
			if (expr->GetType() == AST_Type::Return || !success)
			{
				return_var = res;
				return false;
			}
		}

		return true;
	}
	else
	{
		errhandler.OnMsg("Cant evaluate if condition");
		return false;
	}
}
