#include "stdafx.h"
#include "AST_BinaryExpr.h"
#include "OperatorMgr.h"
using namespace std;


AST_BinaryExpr::AST_BinaryExpr(Token _optoken, SharedExpr _left, SharedExpr _right)
{
	SetType(AST_Type::Binary);

	optok = _optoken;
	left = _left;
	right = _right;
}

AST_BinaryExpr::~AST_BinaryExpr()
{
}

void AST_BinaryExpr::SetOperands(SharedExpr _left, SharedExpr _right)
{
	left = _left;
	right = _right;
}

void AST_BinaryExpr::PrintSelf(int level)
{
	PrintAlignment(level);
	cout << "op:" << optok.Content() << endl;

	if (left) left->PrintSelf(level + 1);
	else
	{
		PrintAlignment(level + 1);
		cout << "<left operand empty>" << endl;
	}

	if (right) right->PrintSelf(level + 1);
	else
	{
		PrintAlignment(level + 1);
		cout << "<right operand empty>" << endl;
	}
}

bool AST_BinaryExpr::Evaluate(Table & obj, VarPtr &return_var, IMsgHandler &errhandler)
{
	VarPtr leftval, rightval;
	bool leftsuccess = false;
	bool rightsuccess = false;

	if (left)
	{
		leftsuccess = left->Evaluate(obj, leftval, errhandler);
	}
	if (right)
	{
		rightsuccess = right->Evaluate(obj, rightval, errhandler);
	}

	if (!leftsuccess || !rightsuccess)
	{
		if (leftsuccess)
		{
			return_var = leftval; return true;
		}
		else if (rightsuccess)
		{
			return_var = rightval; return true;
		}
		else
		{
			errhandler.OnMsg("BinaryExpr couldnt evaluate both operands");
			return false;
		}
	}
	
	return_var = VarPtr(new Variable(OperatorMgr::Instance().RunOperation(optok.Content(), *leftval, *rightval)));
	return true;
}
