#include "stdafx.h"
#include "AST_StringLiteral.h"
#include "Variable.h"
using namespace std;

AST_StringLiteral::AST_StringLiteral(std::string val)
{
	SetType(AST_Type::StringLiteral);
	value = val;
}

AST_StringLiteral::~AST_StringLiteral()
{
}

void AST_StringLiteral::PrintSelf(int level)
{
	PrintAlignment(level);
	cout << "string literal: " << value << endl;
}

bool AST_StringLiteral::Evaluate(Table & obj, VarPtr &return_var, IMsgHandler &errhandler)
{
	return_var = VarPtr(new Variable(value));
	return true;
}
