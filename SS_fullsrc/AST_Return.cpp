#include "stdafx.h"
#include "AST_Return.h"
#include <string>
using namespace std;

AST_Return::AST_Return(SharedExpr _retval)
{
	SetType(AST_Type::Return);
	retval = _retval;
}

AST_Return::~AST_Return()
{
}

void AST_Return::PrintSelf(int level)
{
	PrintAlignment(level);
	cout << "RETURN" + string(retval?":" : " (void version)") << endl;

	if (retval) retval->PrintSelf(level + 1);
}

bool AST_Return::Evaluate(Table & obj, VarPtr &return_var, IMsgHandler &errhandler)
{
	bool success = retval->Evaluate(obj, return_var, errhandler);
	if (!success)
	{
		errhandler.OnMsg("Return couldnt evaluate result");
	}
	return success;
}
