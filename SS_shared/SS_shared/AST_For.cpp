#include "stdafx.h"
#include "AST_For.h"
#include "Table.h"
using namespace std;

AST_For::AST_For(SharedAssignment _init_assignm, SharedBinExpr _cmp, SharedAssignment _final_assignm,
	std::vector<SharedExpr>& _body)
{
	SetType(AST_Type::For);
	initial_assignment = _init_assignm;
	comparison = _cmp;
	final_assignment = _final_assignm;
	body = _body;
}

AST_For::~AST_For()
{
}

void AST_For::PrintSelf(int level)
{
	PrintAlignment(level);
	cout << "FOR" << endl;

	PrintAlignment(level + 1);
	cout << "initial assignment:" << endl;
	if (initial_assignment) initial_assignment->PrintSelf(level + 2);
	else
	{
		PrintAlignment(level + 2);
		cout << "<emptyassignment>" << endl;
	}

	PrintAlignment(level + 1);
	cout << "cond:" << endl;
	if (comparison) comparison->PrintSelf(level + 2);
	else
	{
		PrintAlignment(level + 2);
		cout << "<emptycomparison>" << endl;
	}

	PrintAlignment(level + 1);
	cout << "last assignment:" << endl;
	if (final_assignment) final_assignment->PrintSelf(level + 2);
	else
	{
		PrintAlignment(level + 2);
		cout << "<emptyassignment>" << endl;
	}

	PrintAlignment(level + 1);
	cout << "body:" << endl;
	for (auto &expr : body)
	{
		if (expr) expr->PrintSelf(level + 2);
		else
		{
			PrintAlignment(level + 2);
			cout << "<emptyexpr>" << endl;
		}
	}
}

bool AST_For::Evaluate(Table & obj, VarPtr &return_var, IMsgHandler &errhandler)
{


	VarPtr cmpresult;

	if (!initial_assignment) errhandler.OnMsg("For loop: empty init assignment");
	if (!comparison) errhandler.OnMsg("For loop: empty comparison");
	if (!final_assignment) errhandler.OnMsg("For loop: empty final assignment");
	if (!initial_assignment || !comparison || !final_assignment) return false;

	for (initial_assignment->Evaluate(obj, VarPtr(), errhandler);
		comparison->Evaluate(obj, cmpresult, errhandler) && cmpresult->GetIntRef() != 0;
			final_assignment->Evaluate(obj, VarPtr(), errhandler))
		{
			obj.GetVarContainer().PushNewContainer();

			for (auto &expr : body)
			{
				bool success = expr->Evaluate(obj, return_var, errhandler);
				if (expr->GetType() == AST_Type::Return || !success)
				{
					obj.GetVarContainer().PopContainer();
					return false;
				}
			}

			obj.GetVarContainer().PopContainer();
		}

		return true;
}
