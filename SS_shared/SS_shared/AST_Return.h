#pragma once
#include "AST_Expr.h"
#include "Variable.h"

class AST_Return :
	public AST_Expr
{
public:
	AST_Return(SharedExpr _retval);
	AST_Return() = delete;
	~AST_Return();

	virtual void PrintSelf(int level);

	virtual bool Evaluate(Table &obj, VarPtr &return_var, IMsgHandler &errhandler);
private:
	SharedExpr retval;
};

typedef std::shared_ptr<AST_Return> SharedReturn;