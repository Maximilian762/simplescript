#include "stdafx.h"
#include "AST_Expr.h"
#include <iostream>
using namespace std;
AST_Expr::AST_Expr()
{
}


AST_Expr::~AST_Expr()
{
}

void AST_Expr::PrintSelf(int level)
{
	PrintAlignment(level);
	cout << "emptyexpr" << endl;
}

AST_Type AST_Expr::GetType()
{
	return _type;
}

bool AST_Expr::Is(AST_Type t)
{
	return _type == t;
}

bool AST_Expr::Evaluate(Table & obj, VarPtr &return_var, IMsgHandler &errhandler)
{
	errhandler.OnMsg("EmptyExpr evaluated for some reason");
	return false;
}

void AST_Expr::PrintAlignment(int level)
{
	for (int i = 0; i < level; ++i) cout << "  ";
}

void AST_Expr::SetType(AST_Type t)
{
	_type = t;
}
